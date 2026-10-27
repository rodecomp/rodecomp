#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/PacketId.h"

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

struct UIWindowRecorderContext {
  unsigned char pad[0x2C];
  int windowId;

  void RecordEvent17000(int a, int b, void* c, int d);
  void RecordEvent17001(int a, void* b, void* c);
  void RecordEvent17002(int a, int b, int c, int d, int e);
  void RecordEvent17003(int event, const char* text);
};

// OFFSET: 0x00A82F80

// OFFSET: 0x00A83180

// OFFSET: 0x00A83300

// OFFSET: 0x00A83440
