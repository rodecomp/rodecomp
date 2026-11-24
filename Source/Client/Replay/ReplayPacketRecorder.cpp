#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/PacketId.h"
#include "UI/Window/UIWindow.h"

#include <algorithm>
#include <cstring>
#include <vector>
#include <windows.h>
#include <timeapi.h>

namespace {
#pragma pack(push, 1)
  struct RecordHeader {
    /* 0000 */ std::uint32_t frame;
    /* 0004 */ std::uint32_t tick;
    /* 0008 */ std::uint16_t bytes;
  };

  struct ReplayCipherWord {
    /* 0000 */ std::uint32_t value;
  };
#pragma pack(pop)
static_assert(sizeof(RecordHeader) == 10);
static_assert(sizeof(ReplayCipherWord) == 4);
}

// OFFSET: 0x00B86880
// FUNCTION: fn_19332
bool Replay::Record(std::uint16_t bytes, const void* packet, int event, std::uint32_t delay) {
  if (0 == bytes || !packet) {
    return false;
  }

  std::uint16_t id;
  std::memcpy(&id, packet, sizeof(id));

  if (ID_CC_REPLAYPACKET != id) {
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
      if (this->header.options.Get(ReplayRecordOption::Chatting) == 0) {
        return false;
      }

      break;
    default:
      break;
    }
  }

  std::vector<unsigned char> record(static_cast<std::size_t>(bytes) + 10, 0);
  RecordHeader* recordHeader = reinterpret_cast<RecordHeader*>(record.data());
  recordHeader->bytes = bytes;
  recordHeader->frame = 0;
  recordHeader->tick = 0;

  if (event == 43) {
    recordHeader->frame = this->frame - this->baseFrame;
    recordHeader->tick = timeGetTime() + (delay - this->started);
  } else if (event == 42) {
    recordHeader->frame = this->frame - this->baseFrame;
    recordHeader->tick = timeGetTime() - this->started;
  }

  std::memcpy(record.data() + 10, packet, bytes);

  auto* words = reinterpret_cast<ReplayCipherWord*>(record.data() + 10);
  if (words) {
    const std::uint32_t key = this->header.key1 >> 3;
    const int count = bytes / 4;
    const std::uint32_t base = this->header.key0 >> 5;
    this->cipherKey = key;
    this->cipherIndex = 0;
    this->cipherBase = base;
    for (int index = 0; index < count; ++index) {
      ++this->cipherIndex;
      words[index].value ^= (this->cipherBase + this->cipherIndex) * this->cipherKey;
    }
  }

  std::fseek(this->file, static_cast<long>(this->fileOffset), SEEK_SET);
  std::fwrite(record.data(), record.size(), 1, this->file);
  this->fileOffset += static_cast<std::uint32_t>(record.size());
  ++this->records;

  return true;
}

// OFFSET: 0x00B86AB0
// FUNCTION: fn_19334
bool ReplayFileList::SetReplay(Replay* value) {
  if (!value) {
    return false;
  }
  this->replay = value;
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
  if (0 == bytes || !data) {
    return false;
  }

  std::uint16_t id;
  std::memcpy(&id, data, sizeof(id));

  if (ID_CC_REPLAYPACKET != id) {
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
      if (this->header.options.Get(ReplayRecordOption::Chatting) == 0) {
        return false;
      }

      break;
    default:
      break;
    }
  }

  std::vector<unsigned char> record(static_cast<std::size_t>(bytes) + 10, 0);
  RecordHeader* recordHeader = reinterpret_cast<RecordHeader*>(record.data());
  recordHeader->bytes = bytes;
  recordHeader->frame = 0;
  recordHeader->tick = 0;

  if (event == 43) {
    recordHeader->frame = this->frame - this->baseFrame;
    recordHeader->tick = timeGetTime() + (delay - this->started);
  } else if (event == 42) {
    recordHeader->frame = this->frame - this->baseFrame;
    recordHeader->tick = timeGetTime() - this->started;
  }

  std::memcpy(record.data() + 10, data, bytes);

  auto* words = reinterpret_cast<ReplayCipherWord*>(record.data() + 10);
  if (words) {
    const std::uint32_t key = this->header.key1 >> 3;
    const int count = bytes / 4;
    const std::uint32_t base = this->header.key0 >> 5;
    this->cipherKey = key;
    this->cipherIndex = 0;
    this->cipherBase = base;
    for (int index = 0; index < count; ++index) {
      words[index].value ^= this->cipherKey * (++this->cipherIndex + this->cipherBase);
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
  Replay::Instance().RecordPacket(this->cachedPackets[2459].bytes, 41, 0);
  Replay::Instance().RecordPacket(this->cachedPackets[470].bytes, 41, 0);
  Replay::Instance().RecordPacket(g_session.BuildSkillListReplayPacket(0x10F), 41, 0);
  Replay::Instance().RecordPacket(g_session.BuildSkillListReplayPacket(0x235), 41, 0);
  Replay::Instance().RecordPacket(g_session.BuildSkillListReplayPacket(0x29D), 41, 0);
  if (g_session.replayPacketMode != 0) {
    Replay::Instance().RecordPacket(g_session.BuildStatusReplayPacket(), 41, 0);
  }
}

// OFFSET: 0x00A83180
// FUNCTION: fn_17001
int UIWindow::RecordData(std::uint16_t type, std::vector<unsigned char> data) {
  if (Replay::Instance().state != 2 || data.empty()) {
    return 0;
  }

  std::vector<unsigned char> record(data.size() + 6);
  std::memcpy(record.data() + 6, data.data(), data.size());

  const std::uint16_t header[2] = {ID_CC_REPLAYPACKET, type};
  const std::size_t bytes = record.size();
  unsigned char* buffer = record.data();
  std::memcpy(buffer, header, sizeof(header));
  *reinterpret_cast<std::uint16_t*>(buffer + 4) = static_cast<std::uint16_t>(bytes);

  Replay::Instance().RecordPacket(record, 41, 0);
  return static_cast<int>(record.size());
}

// INLINED
std::vector<unsigned char> BuildEntryQueueReplayRecords() {
  return g_session.BuildEntryQueueReplayRecords();
}

// INLINED
std::vector<unsigned char> BuildRenewQuestReplayRecords() {
  return g_session.BuildRenewQuestReplayRecords();
}
