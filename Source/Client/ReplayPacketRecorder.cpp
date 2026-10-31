#include "Client/Mode/ModeDragState.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/PacketId.h"
#include "UI/Window/WindowPolicy.h"

#include <algorithm>
#include <cstring>
#include <vector>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00B86880
// FUNCTION: fn_19332
bool ReplayPacketRecorder::Record(std::uint16_t bytes, const void* packet, int event, std::uint32_t delay) {
  if (bytes == 0 || !packet) {
    return false;
  }

  std::uint16_t id;
  std::memcpy(&id, packet, sizeof(id));

  if (id != ID_REPLAY_WINDOW_RESPONSE) {
    if (this->lengths.IsRecorded(id) == 0) {
      return false;
    }

    switch (id) {
    case ID_ZC_NOTIFY_CHAT:
    case ID_ZC_NOTIFY_PLAYERCHAT:
    case ID_ZC_WHISPER:
    case ID_ZC_NOTIFY_CHAT_PARTY:
    case ID_ZC_GUILD_CHAT:
    case ID_ZC_BATTLEFIELD_CHAT:
    case ID_ZC_ALLY_CHAT:
      if (this->recordRestricted == 0) {
        return false;
      }

      break;
    default:
      break;
    }
  }

  std::vector<unsigned char> record(static_cast<std::size_t>(bytes) + 10, 0);
  std::memcpy(record.data() + 8, &bytes, sizeof(bytes));

  if (event == 42 || event == 43) {
    const std::uint32_t frameDelta = this->frame - this->baseFrame;
    const std::uint32_t tick = event == 43 ? delay - this->started + timeGetTime() : timeGetTime() - this->started;
    std::memcpy(record.data(), &frameDelta, sizeof(frameDelta));
    std::memcpy(record.data() + 4, &tick, sizeof(tick));
  }

  std::memcpy(record.data() + 10, packet, bytes);
  this->cipherKey = this->key1 >> 3;
  this->cipherIndex = 0;
  this->cipherBase = this->key0 >> 5;

  for (unsigned index = 0; index < bytes / 4u; ++index) {
    std::uint32_t word;
    std::memcpy(&word, record.data() + 10 + index * 4, sizeof(word));
    word ^= this->cipherKey * (++this->cipherIndex + this->cipherBase);
    std::memcpy(record.data() + 10 + index * 4, &word, sizeof(word));
  }

  std::fseek(this->file, static_cast<long>(this->fileOffset), SEEK_SET);
  std::fwrite(record.data(), record.size(), 1, this->file);
  this->fileOffset += static_cast<std::uint32_t>(record.size());
  ++this->records;

  return true;
}

// OFFSET: 0x00A82F00
// FUNCTION: fn_16999
void WindowResponseRecorder::Record(int windowId, int response, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (this->mode != 2) {
    return;
  }

#pragma pack(push, 1)
  struct Response {
    std::uint32_t marker;
    std::uint16_t bytes;
    std::int32_t windowId;
    std::int32_t response;
    std::uint32_t arguments[3];
  };
#pragma pack(pop)
  static_assert(sizeof(Response) == 26);
  const Response packet{0x038408AEu, 26, windowId, response, {static_cast<std::uint32_t>(b), static_cast<std::uint32_t>(c), static_cast<std::uint32_t>(d)}};
  this->packets.Record(sizeof(packet), &packet, 42, 0);
}

// OFFSET: 0x00B86AB0
// FUNCTION: fn_19334
bool Replay::SetBaseFrame(std::uint32_t baseFrame) {
  if (!baseFrame) {
    return false;
  }
  this->baseFrame = baseFrame;
  return true;
}

// OFFSET: 0x00B86AD0
// FUNCTION: fn_19335
void Replay::UpdateDuration() {
  this->header.duration = timeGetTime() - this->started;
}

// OFFSET: 0x00B86A80
// FUNCTION: fn_19333
bool Replay::WriteFinalSection() {
  if (this->records == 0) {
    this->sections[1].id = 1;
    this->sections[1].bytes = 0;
    this->sections[1].fileOffset = this->fileOffset;
  }
  return true;
}

// OFFSET: 0x00B86660
// FUNCTION: fn_19331
bool Replay::RecordPacket(const std::vector<unsigned char>& packet, int event, std::uint32_t delay) {
  const std::uint16_t bytes = static_cast<std::uint16_t>(packet.size());
  const unsigned char* data = packet.empty() ? nullptr : &packet[0];
  if (bytes == 0 || !data) {
    return false;
  }

  std::uint16_t id;
  std::memcpy(&id, data, sizeof(id));

  if (id != ID_REPLAY_WINDOW_RESPONSE) {
    if (g_packetLengths.IsRecorded(id) == 0) {
      return false;
    }

    switch (id) {
    case ID_ZC_NOTIFY_CHAT:
    case ID_ZC_NOTIFY_PLAYERCHAT:
    case ID_ZC_WHISPER:
    case ID_ZC_NOTIFY_CHAT_PARTY:
    case ID_ZC_GUILD_CHAT:
    case ID_ZC_BATTLEFIELD_CHAT:
    case ID_ZC_ALLY_CHAT:
      if (this->header.options.recordRestricted == 0) {
        return false;
      }

      break;
    default:
      break;
    }
  }

  std::vector<unsigned char> record(static_cast<std::size_t>(bytes) + 10, 0);
  std::memcpy(record.data() + 8, &bytes, sizeof(bytes));
  std::memset(record.data(), 0, 8);

  if (event == 43) {
    const std::uint32_t frameDelta = this->frame - this->baseFrame;
    const std::uint32_t tick = timeGetTime() + (delay - this->started);
    std::memcpy(record.data(), &frameDelta, sizeof(frameDelta));
    std::memcpy(record.data() + 4, &tick, sizeof(tick));
  } else if (event == 42) {
    const std::uint32_t frameDelta = this->frame - this->baseFrame;
    const std::uint32_t tick = timeGetTime() - this->started;
    std::memcpy(record.data(), &frameDelta, sizeof(frameDelta));
    std::memcpy(record.data() + 4, &tick, sizeof(tick));
  }

  std::memcpy(record.data() + 10, data, bytes);

  std::uint32_t* words = reinterpret_cast<std::uint32_t*>(record.data() + 10);
  if (words) {
    this->cipherKey = this->header.key1 >> 3;
    this->cipherIndex = 0;
    this->cipherBase = this->header.key0 >> 5;
    for (int index = 0; index < bytes / 4; ++index) {
      words[index] ^= this->cipherKey * (++this->cipherIndex + this->cipherBase);
    }
  }

  std::fseek(this->file, static_cast<long>(this->fileOffset), SEEK_SET);
  std::fwrite(record.data(), record.size(), 1, this->file);
  this->fileOffset += static_cast<std::uint32_t>(record.size());
  ++this->records;

  return true;
}

// OFFSET: 0x00B86500
// FUNCTION: fn_19330
void Replay::RecordCachedPackets() {
  ReplayCachedPacket* packets = this->cachedPackets.data();
  Replay::Instance().RecordPacket(packets[2459].bytes, 41, 0);
  Replay::Instance().RecordPacket(packets[470].bytes, 41, 0);

  {
    std::vector<unsigned char> packet = g_session.BuildSkillListReplayPacket(0x10F);
    Replay::Instance().RecordPacket(packet, 41, 0);
  }
  {
    std::vector<unsigned char> packet = g_session.BuildSkillListReplayPacket(0x235);
    Replay::Instance().RecordPacket(packet, 41, 0);
  }
  {
    std::vector<unsigned char> packet = g_session.BuildSkillListReplayPacket(0x29D);
    Replay::Instance().RecordPacket(packet, 41, 0);
  }
  if (g_session.unknown_5C1C != 0) {
    std::vector<unsigned char> packet = g_session.BuildStatusReplayPacket();
    Replay::Instance().RecordPacket(packet, 41, 0);
  }
}

namespace {
#pragma pack(push, 1)
  struct DropRecord {
    /* 0000 */ std::uint32_t marker;
    /* 0004 */ std::uint16_t bytes;
    /* 0006 */ std::int32_t windowId;
    /* 000A */ std::int32_t x;
    /* 000E */ std::int32_t y;
    /* 0012 */ std::int32_t source;
    /* 0016 */ std::uint32_t id;
    /* 001A */ std::uint32_t unknown_001A;
    /* 001E */ std::uint32_t unknown_001E;
    /* 0022 */ std::int32_t quantity;
    /* 0026 */ std::int32_t slot;
    /* 002A */ std::uint32_t type;
    /* 002E */ std::uint32_t refine;
    /* 0032 */ unsigned char identified;
    /* 0033 */ char resource[100];
    /* 0097 */ char name[100];
    /* 00FB */ char skillName[100];
    /* 015F */ std::int32_t skillLevel;
    /* 0163 */ unsigned char cards[16];
    /* 0173 */ unsigned char skill;
    /* 0174 */ std::int32_t optionCount;
    /* 0178 */ unsigned char options[25];
  };

  struct ValuesRecord {
    /* 0000 */ std::uint32_t marker;
    /* 0004 */ std::uint16_t bytes;
    /* 0006 */ std::int32_t windowId;
    /* 000A */ std::int32_t arguments[4];
    /* 001A */ std::int32_t value;
  };

  struct TextRecord {
    /* 0000 */ std::uint32_t marker;
    /* 0004 */ std::uint16_t bytes;
    /* 0006 */ std::int32_t windowId;
    /* 000A */ std::int32_t event;
    /* 000E */ char text[100];
  };
#pragma pack(pop)

  static_assert(sizeof(DropRecord) == 0x191);
  static_assert(sizeof(ValuesRecord) == 0x1E);
  static_assert(sizeof(TextRecord) == 0x72);
}

// OFFSET: 0x00A82F80
// FUNCTION: fn_17000
void WindowReplayRecorder::RecordDrop(int x, int y, const ModeDragState& drag, int target) {
  if (Replay::Instance().state != 2) {
    return;
  }

  if (WindowPolicy(this->policy, this->restricted, 5, this->windowId) == 0) {
    return;
  }

  DropRecord packet;
  std::memset(&packet, 0, sizeof(packet));
  packet.marker = 0x038508AEu;
  packet.bytes = sizeof(packet);
  packet.windowId = target == -1 ? this->windowId : target;
  packet.x = x;
  packet.y = y;
  packet.source = drag.source;
  packet.id = drag.id;
  packet.unknown_001A = drag.unknown_0310;
  packet.unknown_001E = drag.unknown_0314;
  packet.quantity = drag.quantity;
  packet.slot = drag.slot;
  packet.type = drag.type;
  packet.refine = drag.refine;
  packet.identified = drag.identified;
  packet.skillLevel = drag.skillLevel;
  packet.skill = drag.skill;
  std::memcpy(packet.resource, drag.resource.c_str(), std::min(static_cast<int>(drag.resource.size()), 100));
  std::memcpy(packet.name, drag.name.c_str(), std::min(static_cast<int>(drag.name.size()), 100));
  std::memcpy(packet.skillName, drag.skillName.c_str(), std::min(static_cast<int>(drag.skillName.size()), 100));
  std::memcpy(packet.cards, drag.cards, sizeof(packet.cards));
  packet.optionCount = drag.optionCount;
  std::memcpy(packet.options, drag.options, sizeof(packet.options));

  this->packets.Record(packet.bytes, &packet, 42, 0);
}

// OFFSET: 0x00A83180
// FUNCTION: fn_17001
int WindowReplayRecorder::RecordData(std::uint16_t type, std::vector<unsigned char> data) {
  if (Replay::Instance().state != 2 || data.empty()) {
    return 0;
  }

  std::vector<unsigned char> record(data.size() + 6);
  std::memcpy(record.data() + 6, data.data(), data.size());

  const std::uint16_t header[2] = {ID_REPLAY_WINDOW_RESPONSE, type};
  const std::uint16_t bytes = static_cast<std::uint16_t>(record.size());
  std::memcpy(record.data(), header, sizeof(header));
  std::memcpy(record.data() + 4, &bytes, sizeof(bytes));

  Replay::Instance().RecordPacket(record, 41, 0);
  return static_cast<int>(record.size());
}

// OFFSET: 0x00A83300
// FUNCTION: fn_17002
void WindowReplayRecorder::RecordValues(int value, int argument1, int argument2, int argument3, int argument4) {
  if (g_replayAnimationScaling != 0 || Replay::Instance().state != 2) {
    return;
  }

  ValuesRecord packet;
  packet.marker = 0x039108AEu;
  packet.bytes = sizeof(packet);
  packet.windowId = this->windowId;
  packet.value = value;
  packet.arguments[0] = argument1;
  packet.arguments[1] = argument2;
  packet.arguments[2] = argument3;
  packet.arguments[3] = argument4;
  this->packets.Record(packet.bytes, &packet, 42, 0);
}

// OFFSET: 0x00A83440
// FUNCTION: fn_17003
void WindowReplayRecorder::RecordText(int event, const char* text) {
  if (Replay::Instance().state != 2) {
    return;
  }

  TextRecord packet;
  std::memset(packet.text, 0, sizeof(packet.text));
  packet.marker = 0x038B08AEu;
  packet.bytes = sizeof(packet);
  packet.windowId = this->windowId;
  packet.event = event;
  std::memcpy(packet.text, text, std::min(static_cast<int>(std::strlen(text)), 100));
  this->packets.Record(packet.bytes, &packet, 42, 0);
}
