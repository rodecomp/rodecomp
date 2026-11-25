#include "File/CommonObject.h"
#include "Client/Replay/Replay.h"
#include "UI/GameSettings/TalkType.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/ReplayChunkTag.h"
#include "Network/Packets/PacketId.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Skills/SkillInfo.h"
#include "Client/Session/Session.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <windows.h>
#include <timeapi.h>
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/ClientState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Messenger/MessengerMember.h"

// GLOBAL: gl_24549
int g_replayAnimationScaling = 0;

// GLOBAL: gl_24630
static int g_replayFlushCounter = 0;

// STRING: st_16831
static const char g_replayHeaderText[] = "<< Ragnarok Replay File Version 0.%02d >>\r\n \xA1\xD8 Do not modify file! ";

// STRING: st_16832
static const char g_replayHeaderPrefix[] = "\r\n";

#pragma pack(push, 1)
struct ReplayTaggedRecordHeader {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t bytes;
};

struct ReplayRecordHeader {
  /* 0000 */ std::uint32_t frame;
  /* 0004 */ std::uint32_t tick;
  /* 0008 */ std::uint16_t bytes;
  // INLINED
  ReplayRecordHeader() : frame(0), tick(0), bytes(0) {}
  ~ReplayRecordHeader();
};

struct ReplayMetadataTag {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t bytes;

  // INLINED
  ReplayMetadataTag() {
    this->Clear();
  }
  // INLINED
  ~ReplayMetadataTag() {
    this->Clear();
  }

  // INLINED
  void Clear() {
    this->id = 0;
    this->bytes = 0;
  }
};

struct ReplayStatusPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t status;
  /* 0004 */ std::uint32_t accountId;
  /* 0008 */ unsigned char enabled;
  /* 0009 */ std::uint32_t remaining;
  /* 000D */ std::uint32_t total;
  /* 0011 */ std::uint32_t mode;
  /* 0015 */ unsigned char padding_0015[8];
};

struct ReplaySkillInfo {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ int type;
  /* 0006 */ std::int16_t level;
  /* 0008 */ std::int16_t spCost;
  /* 000A */ std::int16_t range;
  /* 000C */ char name[24];
  /* 0024 */ unsigned char upgradable;
};

struct ReplaySkillInfoListHeader {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t bytes;
};

#pragma pack(pop)

static_assert(sizeof(ReplayTaggedRecordHeader) == 6);
static_assert(sizeof(ReplaySkillInfo) == 0x25);
static_assert(sizeof(ReplaySkillInfoListHeader) == 4);
static_assert(sizeof(ReplayMetadataTag) == 6);
static_assert(sizeof(ReplayRecordHeader) == 10);
static_assert(sizeof(ReplayStatusPacket) == 29);
static_assert(offsetof(ReplayStatusPacket, mode) == 17);

// OFFSET: 0x006DA340
// FUNCTION: fn_06623
std::vector<unsigned char> BuildReplayTaggedRecord(std::uint16_t id, const void* data, std::uint32_t bytes) {
  std::vector<unsigned char> record(bytes + sizeof(ReplayTaggedRecordHeader));
  ReplayTaggedRecordHeader* header = reinterpret_cast<ReplayTaggedRecordHeader*>(record.data());
  header->id = id;
  header->bytes = bytes;
  if (bytes != 0) {
    std::memcpy(record.data() + sizeof(ReplayTaggedRecordHeader), data, bytes);
  }
  return record;
}

// INLINED
static std::vector<unsigned char> MakeReplayRecord(std::uint16_t id, const void* data, std::uint32_t bytes) {
  std::vector<unsigned char> record(bytes + sizeof(ReplayTaggedRecordHeader), 0);
  ReplayTaggedRecordHeader* header = reinterpret_cast<ReplayTaggedRecordHeader*>(record.data());
  header->id = id;
  header->bytes = bytes;
  if (bytes != 0) {
    std::memcpy(record.data() + sizeof(ReplayTaggedRecordHeader), data, bytes);
  }
  return record;
}

// INLINED
template <class Record> static void AppendRecord(std::vector<unsigned char>& records, const Record& record) {
  records.insert(records.end(), record.begin(), record.end());
}

// OFFSET: 0x00E3DBA0
// FUNCTION: fn_24195
std::vector<unsigned char> CSession::BuildEntryQueueReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ENTRY_QUEUE_BEGIN, nullptr, 0));
  if (g_entryQueueMgr->current != nullptr) {
    const int id = g_entryQueueMgr->current->id;
    AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ENTRY_QUEUE_ID, &id, sizeof(id)));
  }
  const unsigned char ready = g_entryQueueMgr->ready;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ENTRY_QUEUE_READY, &ready, sizeof(ready)));
  std::string text = g_entryQueueMgr->queueName.text;
  if (text.size() != 0) {
    AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_ENTRY_QUEUE_NAME, text.c_str(), text.size()));
  }
  text = g_entryQueueMgr->characterName.text;
  if (text.size() != 0) {
    AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_ENTRY_QUEUE_CHARACTER_NAME, text.c_str(), text.size()));
  }
  if (UIEntryQueueWnd* window = dynamic_cast<UIEntryQueueWnd*>(g_windowMgr.LookupWindow(WID_JOINTO_BATTLEFIELDWND))) {
    const int selected = window->SelectedIndex();
    AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ENTRY_QUEUE_SELECTION, &selected, sizeof(selected)));
  }
  if (UIEntryQueueStandByWnd* window = dynamic_cast<UIEntryQueueStandByWnd*>(g_windowMgr.LookupWindow(WID_ENTRYQUEUESTANDBYWND))) {
    AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ENTRY_QUEUE_WAITING, &window->m_waiting, sizeof(window->m_waiting)));
    AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ENTRY_QUEUE_RANKING, &window->m_ranking, sizeof(window->m_ranking)));
    AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_ENTRY_QUEUE_STAND_BY_NAME, window->m_queueName.c_str(), window->m_queueName.size()));
  }
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ENTRY_QUEUE_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00B83920
// FUNCTION: fn_19301
ReplayCachedPacket::ReplayCachedPacket(const ReplayCachedPacket& other)
    : enabled(other.enabled), bytes(other.bytes) {
}

// OFFSET: 0x00B839D0
// FUNCTION: fn_19302
ReplaySection::ReplaySection() : id(0), bytes(0), fileOffset(0) {
}

// OFFSET: 0x00B83D00
// FUNCTION: fn_19305
inline ReplayRecordHeader::~ReplayRecordHeader() {
  this->frame = 0;
  this->tick = 0;
  this->bytes = 0;
}

// OFFSET: 0x00B83D20
// FUNCTION: fn_19306
ReplaySection::~ReplaySection() {
  this->id = 0;
  this->bytes = 0;
  this->fileOffset = 0;
}

// OFFSET: 0x00B83540
// FUNCTION: fn_19300
Replay::Replay() : cipherIndex(0), cipherBase(0), cipherKey(0), recordingState(0), playbackState(0) {
  this->state = 1;
  this->baseFrame = 0;
  this->started = 0;
  this->frame = 0;
  this->elapsed = 0;
  this->speed = 1.0f;
  std::memset(&this->header, 0, sizeof(this->header));
  std::memset(&this->header.options, 1, sizeof(this->header.options));
  const ReplayOptions savedOptions = this->header.options;
  std::memset(&this->header, 0, sizeof(this->header));
  this->header.options = savedOptions;
  this->header.signature = 0x52414705;
  this->header.closingSignature = 0x52414705;
  this->records = 0;
  this->fileMode = -1;
  this->file = nullptr;
  this->fileOffset = 0;
  this->previousTick = timeGetTime();
  this->playbackState = 0;
  this->recordingState = 0;
  this->cachedPackets.resize(3143, ReplayCachedPacket{});
  this->cachedPackets[2459].enabled = true;
  this->cachedPackets[470].enabled = true;
}

// INLINED
static __forceinline std::size_t WriteReplaySection(Replay& replay, int id, std::uint32_t size, unsigned char* data, int event) {
  if (event == 14) {
    replay.sections[id].id = static_cast<std::uint16_t>(id);
    replay.sections[id].bytes = size;
    replay.sections[id].fileOffset = replay.fileOffset;
    replay.fileOffset += size;
  }

  const std::uint32_t sectionBytes = replay.sections[id].bytes;
  if (sectionBytes != 0) {
    replay.cipherBase = replay.header.key0 >> 5;
    const int count = sectionBytes / 4;
    replay.cipherIndex = 0;
    replay.cipherKey = replay.header.key1 >> 3;

    for (int index = 0; index < count; ++index) {
      ++replay.cipherIndex;
      std::uint32_t word;
      std::memcpy(&word, data + index * sizeof(word), sizeof(word));
      word ^= (replay.cipherBase + replay.cipherIndex) * replay.cipherKey;
      std::memcpy(data + index * sizeof(word), &word, sizeof(word));
    }
  }

  std::fseek(replay.file, static_cast<long>(replay.sections[id].fileOffset), SEEK_SET);
  return std::fwrite(data, replay.sections[id].bytes, 1, replay.file);
}

// OFFSET: 0x00B856D0
// FUNCTION: fn_46477
bool Replay::WriteSection(int id, std::uint32_t size, unsigned char* data, int event) {
  if (id > 0 && id < 26 && size != 0 && data != nullptr) {
    if (WriteReplaySection(*this, id, size, data, event) != 0) {
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00B855B0
// FUNCTION: fn_19326
bool Replay::WriteSection(int id, const std::vector<unsigned char>& bytes, int event) {
  if (bytes.end() == bytes.begin()) {
    return false;
  }

  const std::uint32_t size = static_cast<std::uint32_t>(bytes.size());
  unsigned char* data = const_cast<unsigned char*>(bytes.data());
  if (id <= 0 || id >= 26 || size == 0 || data == nullptr) {
    return false;
  }

  return WriteReplaySection(*this, id, size, data, event) != 0;
}

// OFFSET: 0x00B857E0
// FUNCTION: fn_19327
__declspec(noinline) int Replay::WriteHeader(int event) {
  this->header.records = this->records;
  this->header.fileOffset = this->fileOffset;
  this->header.closingSignature = 0x52414705;
  this->header.signature = 0x52414705;
  if (event == 11) {
    this->sections[25].id = 111;
    if (!this->WriteSection(2, this->BuildMetadata(), 14)) {
      return 0;
    }
    char text[100] = {};
    std::sprintf(text, g_replayHeaderText, 5);
    std::sprintf(text + 95, g_replayHeaderPrefix);
    std::fseek(this->file, 0, SEEK_SET);
    std::fwrite(text, sizeof(text), 1, this->file);
    std::fwrite(&this->header, 12, 1, this->file);
    return 1;
  } else if (event == 12) {
    if (++g_replayFlushCounter == 1200) {
      this->header.closingSignature = 0x51404605;
      this->header.signature = 0x51404605;
      if (!this->WriteSection(2, this->BuildMetadata(), 15)) {
        return 0;
      }
      std::fseek(this->file, 112, SEEK_SET);
      std::fwrite(this->sections, sizeof(this->sections), 1, this->file);
      g_replayFlushCounter = 0;
    }
    return 1;
  } else if (event == 13) {
    if (!this->WriteSection(2, this->BuildMetadata(), 15)) {
      return 0;
    }
    std::fseek(this->file, 112, SEEK_SET);
    std::fwrite(this->sections, sizeof(this->sections), 1, this->file);
    return 1;
  }
  return 0;
}

// OFFSET: 0x00B85A20
// FUNCTION: fn_19328
std::vector<unsigned char> Replay::BuildMetadata() {
  std::vector<unsigned char> metadata;
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_BEGIN, nullptr, 0));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_SERVICE, &this->header.service, sizeof(this->header.service)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_SERVICE_TYPE, &this->header.serviceType, sizeof(this->header.serviceType)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_SESSION_SYNC_VALUE, &this->header.sessionSyncValue, sizeof(this->header.sessionSyncValue)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_CHARACTER_NAME, this->header.characterName, sizeof(this->header.characterName)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_MAP_NAME, this->header.mapName, sizeof(this->header.mapName)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_ACCOUNT_ID, &this->header.accountId, sizeof(this->header.accountId)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_CELL_X, &this->header.cellX, sizeof(this->header.cellX)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_CELL_Y, &this->header.cellY, sizeof(this->header.cellY)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_PLAYER_DIRECTION, &this->header.playerDirection, sizeof(this->header.playerDirection)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_DURATION, &this->header.duration, sizeof(this->header.duration)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_RECORDS, &this->header.records, sizeof(this->header.records)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_FILE_OFFSET, &this->header.fileOffset, sizeof(this->header.fileOffset)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_OPTIONS, &this->header.options, sizeof(this->header.options)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_CLOSING_SIGNATURE, &this->header.closingSignature, sizeof(this->header.closingSignature)));
  AppendRecord(metadata, MakeReplayRecord(REPLAY_CHUNK_METADATA_END, nullptr, 0));
  return metadata;
}

// OFFSET: 0x00B875E0
// FUNCTION: fn_19354
Replay& Replay::Instance() {
  // GLOBAL: gl_24631
  static Replay g_replay;
  return g_replay;
}

// OFFSET: 0x00B83D50
// FUNCTION: fn_19307
bool Replay::MatchesService(int mode) const {
  const int type = this->header.serviceType;
  if (mode == 2) {
    if (type == ServerPrimary || type == ServerStage || type == ServerSakray || type == ServerLocal) {
      return true;
    }
  } else if (g_serviceMode == type) {
    return true;
  }

  return false;
}

// OFFSET: 0x00B83D90
// FUNCTION: fn_19308
int Replay::AdvanceFrame() {
  if (!g_replayAnimationScaling) {
    ++this->frame;
  } else {
    const std::uint32_t now = timeGetTime();

    if (this->state != 6) {
      ++this->frame;
      this->elapsed += static_cast<std::uint32_t>(static_cast<float>(now - this->previousTick) * this->speed);

      if (static_cast<int>(this->elapsed) >= static_cast<int>(this->header.duration)) {
        this->previousTick = now;
        this->elapsed = this->header.duration;
        this->state = 6;
        return 0;
      }
    }

    this->previousTick = now;
  }
  return 1;
}

// OFFSET: 0x00B83E10
// FUNCTION: fn_19309
void Replay::ResetPlayback() {
  if (this->state != 3 && this->state != 8) {
    this->records = 0;
    this->frame = 0;
    this->elapsed = 0;
  }
}

// OFFSET: 0x00B83E40
// FUNCTION: fn_19310
__declspec(noinline) bool Replay::Cipher(std::uint32_t bytes, void* data) {
  if (!data || !bytes) {
    return false;
  }

  auto* words = static_cast<std::uint32_t*>(data);
  this->cipherBase = this->header.key0 >> 5;
  const int count = bytes / 4;
  this->cipherIndex = 0;
  this->cipherKey = this->header.key1 >> 3;

  for (int index = 0; index < count; ++index) {
    ++this->cipherIndex;
    words[index] ^= (this->cipherBase + this->cipherIndex) * this->cipherKey;
  }

  return true;
}

// INLINED
static bool Decipher(Replay& replay, std::uint32_t bytes, void* data) {
  if (!data || !bytes) {
    return false;
  }

  auto* words = static_cast<std::uint32_t*>(data);
  const std::uint32_t key = replay.header.key1 >> 3;
  const int count = bytes / 4;
  const std::uint32_t base = replay.header.key0 >> 5;
  replay.cipherKey = key;
  replay.cipherIndex = 0;
  replay.cipherBase = base;

  for (int index = 0; index < count; ++index) {
    ++replay.cipherIndex;
    words[index] ^= (replay.cipherBase + replay.cipherIndex) * replay.cipherKey;
  }

  return true;
}

// OFFSET: 0x00B83F20
// FUNCTION: fn_19312
void Replay::CloseFile() {
  if (this->file) {
    if (this->fileMode == 31) {
      this->WriteHeader(13);
    }

    std::fclose(this->file);
    this->file = nullptr;
  }
}

// OFFSET: 0x00B84340
// FUNCTION: fn_19314
std::uint32_t Replay::Duration() const {
  const unsigned seconds = static_cast<int>(this->header.duration) / 1000;
  const unsigned minutes = seconds / 60;
  const unsigned hours = minutes / 60;
  return MAKELONG(hours, MAKEWORD(minutes % 60, seconds % 60));
}

// OFFSET: 0x00B843A0
// FUNCTION: fn_19315
int Replay::FindSection(int id) const {
  if (id < 1 || id > 25) {
    return -1;
  }

  if (id == this->sections[id].id) {
    return id;
  }

  for (int index = 0; index < 26; ++index) {
    if (this->sections[index].id == id) {
      return index;
    }
  }

  return -1;
}

// OFFSET: 0x00B84620
// FUNCTION: fn_19318
int Replay::SpeedLevel() const {
  if (0.25f == this->speed) {
    return 1;
  }
  if (0.5f == this->speed) {
    return 2;
  }
  if (1.0f == this->speed) {
    return 3;
  }
  if (2.0f == this->speed) {
    return 4;
  }
  if (4.0f == this->speed) {
    return 5;
  }
  if (8.0f == this->speed) {
    return 6;
  }
  if (16.0f == this->speed) {
    return 7;
  }
  return 0;
}

// OFFSET: 0x00B846D0
// FUNCTION: fn_19319
void Replay::ResetFile() {
  const ReplayOptions savedOptions = this->header.options;
  std::memset(&this->header.key0, 0, offsetof(ReplayHeader, options) - offsetof(ReplayHeader, key0));
  this->header.signature = 0x52414705;
  this->header.closingSignature = 0x52414705;
  this->records = 0;
  this->header.options = savedOptions;
  this->fileMode = -1;
  this->file = nullptr;
  this->fileOffset = 0;
}

// OFFSET: 0x00B84930
// FUNCTION: fn_19320
std::vector<unsigned char> Replay::ReadSection(int id) {
  std::vector<unsigned char> data;

  if (id < 1 || id > 25) {
    return data;
  }

  const int index = this->FindSection(id);
  if (index != -1 && this->sections[index].fileOffset != 0 && this->sections[index].bytes != 0) {
    const int bytes = static_cast<int>(this->sections[index].bytes);
    data.resize(bytes, 0);
    std::fseek(this->file, static_cast<long>(this->sections[index].fileOffset), SEEK_SET);
    std::fread(data.data(), this->sections[index].bytes, 1, this->file);
    this->Cipher(this->sections[index].bytes, data.data());
  }

  return data;
}

// OFFSET: 0x00B84E40
// FUNCTION: fn_19322
std::uint32_t Replay::ParseMetadata(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFF;
  }

  ReplayMetadataTag tag;
  std::uint32_t offset = 0;
  tag.Clear();
  tag = *reinterpret_cast<const ReplayMetadataTag*>(&data[offset]);
  offset += sizeof(tag);

  if (REPLAY_CHUNK_METADATA_BEGIN != tag.id) {
    offset = 0xFFFFFFFF;
  } else {
    while (data.size() > offset) {
      tag.Clear();
      tag = *reinterpret_cast<const ReplayMetadataTag*>(&data[offset]);
      offset += 6;

      switch (tag.id) {
      case REPLAY_CHUNK_METADATA_END:
        return offset;
      case REPLAY_CHUNK_METADATA_SERVICE:
        std::memcpy(&this->header.service, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_SERVICE_TYPE:
        std::memcpy(&this->header.serviceType, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_SESSION_SYNC_VALUE:
        std::memcpy(&this->header.sessionSyncValue, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_ACCOUNT_ID:
        std::memcpy(&this->header.accountId, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_CHARACTER_NAME:
        std::memcpy(this->header.characterName, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_MAP_NAME:
        std::memcpy(this->header.mapName, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_CELL_X:
        std::memcpy(&this->header.cellX, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_CELL_Y:
        std::memcpy(&this->header.cellY, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_PLAYER_DIRECTION:
        std::memcpy(&this->header.playerDirection, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_DURATION:
        std::memcpy(&this->header.duration, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_RECORDS:
        std::memcpy(&this->header.records, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_FILE_OFFSET:
        std::memcpy(&this->header.fileOffset, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_OPTIONS:
        std::memcpy(&this->header.options, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case REPLAY_CHUNK_METADATA_CLOSING_SIGNATURE:
        std::memcpy(&this->header.closingSignature, &data[offset], tag.bytes);
        offset += tag.bytes;

        if (this->header.signature != this->header.closingSignature) {
          std::uint32_t fileOffset = this->fileOffset;
          std::uint32_t tick;
          {
            ReplayRecordHeader record;
            int count = static_cast<int>(this->header.records) - 1;

            if (count > 0) {
              do {
                std::fseek(this->file, static_cast<long>(fileOffset), SEEK_SET);
                std::fread(&record, sizeof(record), 1, this->file);
                fileOffset += record.bytes + sizeof(record);
                --count;
              } while (count != 0);
            }

            tick = record.tick;
          }

          this->header.duration = tick;
        }
        break;
      default:
        offset += tag.bytes;
        break;
      }
    }
  }

  return offset;
}

// OFFSET: 0x00B85350
// FUNCTION: fn_19323
bool Replay::ReadRecord(unsigned char* buffer, std::uint16_t& id, bool force) {
  if (!force && this->state != 7) {
    return false;
  }

  bool result = false;
  ReplayRecordHeader record;
  std::fseek(this->file, static_cast<long>(this->fileOffset), SEEK_SET);
  std::fread(&record, sizeof(record), 1, this->file);

  if (this->header.records > this->records && this->elapsed >= record.tick) {
    std::fread(buffer, record.bytes, 1, this->file);
    Decipher(*this, record.bytes, buffer);
    std::memcpy(&id, buffer, sizeof(id));
    this->fileOffset += record.bytes + sizeof(record);
    result = true;
    ++this->records;
  }

  return result;
}

// OFFSET: 0x00B854C0
// FUNCTION: fn_19324
bool Replay::AllowsEvent(int event) const {
  if (!g_replayAnimationScaling) {
    return false;
  }

  if (event == 271 || event == 510 || event == 201 || event == 286 || event == 580) {
    return false;
  }

  if (event == 195) {
    return this->header.options.Get(ReplayRecordOption::Equip) == 0;
  }
  if (event == 194) {
    return this->header.options.Get(ReplayRecordOption::Item) == 0;
  }
  if (event == 196) {
    return this->header.options.Get(ReplayRecordOption::Skill) == 0;
  }
  if (event == 199) {
    return this->header.options.Get(ReplayRecordOption::Party) == 0;
  }
  if (event == 192) {
    return this->header.options.Get(ReplayRecordOption::Status) == 0;
  }

  return true;
}

// OFFSET: 0x00B85580
// FUNCTION: fn_19325
void Replay::Rewind() {
  this->records = 0;
  this->fileOffset = this->sections[this->FindSection(1)].fileOffset;
}

// OFFSET: 0x00B83EC0
// FUNCTION: fn_19311
__declspec(noinline) void ReplayFileList::Clear() {
  this->selected = -1;
  this->files.erase(this->files.begin(), this->files.end());
}

// OFFSET: 0x00B84B60
// FUNCTION: fn_19321
bool ReplayFileList::Refresh() {
  this->Clear();

  BOOL more = TRUE;
  char directory[260] = {};
  GetCurrentDirectoryA(sizeof(directory), directory);

  std::string pattern(directory);
  pattern.append("\\Replay\\*.rrf", 13);

  WIN32_FIND_DATAA found;
  HANDLE search = FindFirstFileA(pattern.c_str(), &found);

  while (search != INVALID_HANDLE_VALUE && more) {
    if ((found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
      const std::size_t nameLength = std::strlen(found.cFileName);
      BOOL isReplay = FALSE;
      if (found.cFileName[nameLength - 1] == 'f' && found.cFileName[nameLength - 2] == 'r' && found.cFileName[nameLength - 3] == 'r') {
        isReplay = found.cFileName[nameLength - 4] == '.';
      }

      if (isReplay) {
        ReplayFileEntry entry = {};
        const std::size_t length = std::strlen(found.cFileName);
        std::memcpy(entry.name, found.cFileName, length);
        entry.name[length] = '\0';
        entry.index = 0;

        if (length == 23) {
          char number[6] = {};
          std::memcpy(number, entry.name + 15, 4);
          entry.index = std::atoi(number);
        }

        this->files.push_back(entry);
      }
    }

    more = FindNextFileA(search, &found);
  }

  FindClose(search);
  return true;
}

// OFFSET: 0x00B84300
// FUNCTION: fn_19313
const char* ReplayFileList::FileName(int index) const {
  if (this->files.size() <= static_cast<unsigned>(index)) {
    return nullptr;
  }

  return this->files[index].name;
}

// OFFSET: 0x00B86AF0
// FUNCTION: fn_19336
char* ReplayFileList::SetPath(const char* value) {
  if (!value || !this->pathEnabled) {
    return nullptr;
  }

  std::memset(this->path, 0, sizeof(this->path));
  std::memcpy(this->path, value, std::strlen(value));
  return this->path;
}

// OFFSET: 0x00B870D0
// FUNCTION: fn_19338
bool ReplayFileList::Select(int index) {
  if (index < 0) {
    return false;
  }

  const int count = static_cast<int>(this->files.size());

  if (count <= index) {
    index = count - 1;
  }

  this->selected = index;
  return true;
}

// STRING: st_10400
static const char g_replayExtensionFormat[] = "%s.rrf";

// STRING: st_10401
static const char g_replayPathFormat[] = "Replay\\%s.rrf";

// STRING: st_10402
static const char g_replayNewFileFormat[] = "Replay\\RagnarokReplay_%04d.rrf";

// OFFSET: 0x00B87360
// FUNCTION: fn_19344
bool Replay::SetOptions(const void* data, int bytes) {
  std::memset(&this->header.options, 0, bytes);
  std::memcpy(&this->header.options, data, bytes);
  return true;
}

// INLINED
static __forceinline bool OpenReplayFile(Replay* replay, const char* path, int mode) {
  if (!path) {
    return false;
  }

  replay->fileMode = mode;

  if (mode == 0x1F) {
    replay->file = std::fopen(path, "wb");
    if (!replay->file) {
      return false;
    }

    replay->fileOffset = 0x174;
    return true;
  }

  replay->file = std::fopen(path, "rb");
  if (!replay->file) {
    return false;
  }

  std::fseek(replay->file, 100, SEEK_SET);
  std::memset(&replay->header, 0, 0xC);
  std::fread(&replay->header, 0xC, 1, replay->file);

  if (replay->header.signature == 0x52414705u || replay->header.signature == 0x51404605u) {
    ReplaySection section;

    bool reading = true;

    for (int index = 0; reading && index <= 0x1A; ++index) {
      std::fread(&section, sizeof(section), 1, replay->file);

      if (section.id < 0 || section.id > 0x1A) {
        reading = false;
      } else {
        replay->sections[section.id].id = section.id;
        replay->sections[section.id].bytes = section.bytes;
        replay->sections[section.id].fileOffset = section.fileOffset;
        section.id = 0;
        section.bytes = 0;
        section.fileOffset = 0;
      }
    }

    if (replay->ParseMetadata(replay->ReadSection(2)) == 0xFFFFFFFFu) {
      std::memset(&replay->header, 0, sizeof(replay->header));
    } else {
      return true;
    }
  }

  if (replay->file) {
    std::fclose(replay->file);
    replay->file = nullptr;
  }

  std::memset(&replay->header, 0, sizeof(replay->header));
  std::memset(replay->sections, 0, sizeof(replay->sections));
  return false;
}

// OFFSET: 0x00B86B50
// FUNCTION: fn_19337
int ReplayFileList::Open(int mode, int argument1, int argument2) {
  if (!this->replay) {
    return 0;
  }

  if (mode == 0x20) {
    if (this->selected < 0 || this->files.empty()) {
      return 0;
    }

    std::string name("Replay\\");
    name += this->files[this->selected].name;
    std::memset(this->path, 0, sizeof(this->path));
    std::memcpy(this->path, name.data(), name.size());
    if (OpenReplayFile(this->replay, this->path, 0x20)) {
      return 1;
    }
  } else {
    this->Refresh();

    if (this->pathEnabled) {
      if (this->rejectExistingName) {
        auto file = this->files.begin();
        const auto end = this->files.end();
        char name[0x104];
        std::memset(name, 0, sizeof(name));
        std::sprintf(name, g_replayExtensionFormat, this->path);

        for (; file != end; ++file) {
          if (std::strcmp(name, file->name) == 0) {
            std::memset(this->path, 0, sizeof(this->path));
            return -2;
          }
        }
      }

      char name[0x104];
      std::memset(name, 0, sizeof(name));
      std::sprintf(name, g_replayPathFormat, this->path);
      std::memcpy(this->path, name, sizeof(this->path));
    } else {
      int index = 1;

      for (const ReplayFileEntry& file : this->files) {
        if (file.index == index) {
          ++index;
        }
      }

      std::memset(this->path, 0, sizeof(this->path));
      std::sprintf(this->path, g_replayNewFileFormat, index);
    }

    if (OpenReplayFile(this->replay, this->path, 0x1F)) {
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00B87680
// FUNCTION: fn_19355
ReplayFileList& ReplayFileList::Instance() {
  // GLOBAL: gl_24867
  static ReplayFileList g_replayFileList;
  return g_replayFileList;
}

// OFFSET: 0x00B87110
// FUNCTION: fn_19339
bool Replay::SetHeaderInfo(
  std::uint32_t key0, std::uint32_t key1, const char* characterName, const char* mapName, int accountId, int cellX, int cellY, int playerDirection, int service,
  int serviceType, int sessionSyncValue
) {
  if (!characterName || !mapName) {
    return false;
  }
  std::memcpy(this->header.mapName, mapName, sizeof(this->header.mapName));
  std::memcpy(this->header.characterName, characterName, sizeof(this->header.characterName));
  this->header.service = service;
  this->header.serviceType = serviceType;
  this->header.sessionSyncValue = sessionSyncValue;
  this->header.key0 = key0;
  this->header.key1 = key1;
  this->header.accountId = accountId;
  this->header.cellX = cellX;
  this->header.cellY = cellY;
  this->header.playerDirection = playerDirection;
  this->header.signature = 0x52414705;
  this->header.closingSignature = 0x52414705;
  return true;
}

// OFFSET: 0x00B871C0
// FUNCTION: fn_19340
void Replay::IncreaseSpeed() {
  if (0.25f == this->speed) {
    this->speed = 0.5f;
  } else if (0.5f == this->speed) {
    this->speed = 1.0f;
  } else if (1.0f == this->speed) {
    this->speed = 2.0f;
  } else if (2.0f == this->speed || 4.0f == this->speed) {
    this->speed = 4.0f;
  }
}

// OFFSET: 0x00B87240
// FUNCTION: fn_19341
void Replay::DecreaseSpeed() {
  if (0.25f == this->speed || 0.5f == this->speed) {
    this->speed = 0.25f;
  } else if (1.0f == this->speed) {
    this->speed = 0.5f;
  } else if (2.0f == this->speed) {
    this->speed = 1.0f;
  } else if (4.0f == this->speed) {
    this->speed = 2.0f;
  }
}

// OFFSET: 0x00B872C0
// FUNCTION: fn_19342
void Replay::RestartClock() {
  if (this->state != 3 && this->state != 8) {
    this->baseFrame = this->frame;
    this->started = this->previousTick = timeGetTime();
  }
}

// OFFSET: 0x00E3E460
// FUNCTION: fn_24196
std::vector<unsigned char> CSession::BuildStatusReplayPacket() {
  std::vector<unsigned char> packet(29, 0);
  auto* data = reinterpret_cast<ReplayStatusPacket*>(packet.data());
  data->id = ID_ZC_MSG_STATE_CHANGE3;
  data->status = 0x2A1;
  data->remaining = 9999;
  data->accountId = this->m_aid;
  data->enabled = 1;
  data->total = 9999;
  data->mode = this->replayPacketMode;
  return packet;
}

// OFFSET: 0x00E31240
// FUNCTION: fn_24172
std::vector<unsigned char> BuildSkillInfoListPacket(std::uint16_t id, const std::list<CSkillInfo>& skills) {
  const int bytes = static_cast<int>(skills.size()) * sizeof(ReplaySkillInfo) + sizeof(ReplaySkillInfoListHeader);
  std::vector<unsigned char> packet(bytes, 0);
  ReplaySkillInfoListHeader* header = reinterpret_cast<ReplaySkillInfoListHeader*>(packet.data());
  header->id = id;
  header->bytes = static_cast<std::uint16_t>(bytes);
  ReplaySkillInfo* records = reinterpret_cast<ReplaySkillInfo*>(header + 1);
  int index = 0;
  for (auto skill = skills.begin(); skill != skills.end(); ++skill) {
    ReplaySkillInfo& record = records[index];
    record.id = static_cast<std::uint16_t>(skill->id);
    record.type = skill->type;
    record.level = static_cast<std::int16_t>(skill->level);
    record.spCost = static_cast<std::int16_t>(skill->spCost);
    record.upgradable = skill->upgradable != 0;
    record.range = static_cast<std::int16_t>(skill->range);
    const char* name = skill->name;
    std::memcpy(record.name, name, (std::min)(sizeof(record.name), std::strlen(name)));
    record.name[sizeof(record.name) - 1] = 0;
    ++index;
  }
  return packet;
}

// OFFSET: 0x00E44640
// FUNCTION: fn_24205
std::vector<unsigned char> CSession::BuildRenewQuestReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_QUESTS_BEGIN, nullptr, 0));
  {
    const int version = 51;
    AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_QUESTS_PARAMETER, &version, sizeof(version)));
  }
  const int size = g_renewQuestMgr->GetOngoingQuestsSize();
  char* buffer = new char[size];
  g_renewQuestMgr->WriteOngoingQuests(buffer);
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_QUESTS_ONGOING, buffer, size));
  if (buffer != nullptr) {
    delete buffer;
  }
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_QUESTS_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00E449C0
// FUNCTION: fn_24206
std::vector<unsigned char> CSession::BuildShortcutReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHORTCUTS_BEGIN, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHORTCUTS_SLOTS, this->m_shortcutSlots, sizeof(this->m_shortcutSlots)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHORTCUTS_QUICK_SLOT, &this->quickSlot, sizeof(this->quickSlot) + sizeof(this->alternateQuickSlot)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHORTCUTS_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00E44DF0
// FUNCTION: fn_24207
std::vector<unsigned char> CSession::BuildSkillListReplayPacket(short id) {
  switch (id) {
  case ID_ZC_SKILLINFO_LIST:
    return BuildSkillInfoListPacket(0x10F, this->m_skillBundle.skills);
  case ID_ZC_HOSKILLINFO_LIST:
    return BuildSkillInfoListPacket(0x235, this->m_homunSkills);
  case ID_ZC_MER_SKILLINFO_LIST:
    return BuildSkillInfoListPacket(0x29D, this->m_mercSkills);
  default:
    return {};
  }
}

// OFFSET: 0x00B86440
// FUNCTION: fn_19329
void Replay::CachePacket(std::uint16_t id, std::uint16_t size, const unsigned char* data) {
  if (0 < size && 3142 >= id && data != nullptr) {
    if (this->cachedPackets[static_cast<short>(id)].enabled) {
      this->cachedPackets[static_cast<short>(id)].bytes.resize(size);
      this->cachedPackets[static_cast<short>(id)].bytes.assign(data, data + size);
    }
  }
}

// OFFSET: 0x00E42EB0
// FUNCTION: fn_24203
std::vector<unsigned char> CSession::BuildReservedReplayRecords() {
  std::vector<unsigned char> records;
  return records;
}

// OFFSET: 0x00E41290
// FUNCTION: fn_24200
std::vector<unsigned char> CSession::BuildItemReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ITEMS_BEGIN, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ITEMS_EXCHANGE_MONEY, &this->exchangeMoney, sizeof(this->exchangeMoney)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ITEMS_OTHER_EXCHANGE_MONEY, &this->otherExchangeMoney, sizeof(this->otherExchangeMoney)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_INVENTORY, SerializeItems(this->m_inventoryItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_SHOP, SerializeItems(this->m_shopItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_PURCHASE, SerializeItems(this->m_purchaseItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_SELL, SerializeItems(this->m_sellItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_STORAGE, SerializeItems(this->m_storageItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_CART, SerializeItems(this->m_cartItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_MERCHANT_SHOP, SerializeItems(this->m_merchantShopItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_MERCHANT_MIRROR, SerializeItems(this->m_merchantMirrorItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_MERCHANT_MY_SHOP, SerializeItems(this->m_merchantMyShopItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_MERCHANT_PURCHASE, SerializeItems(this->m_merchantPurchaseItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_MERCHANT_HOPE_TO_SELL, SerializeItems(this->m_merchantHopeToSellItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_MERCHANT_PURCHASE_SAVED, SerializeItems(this->m_merchantPurchaseSavedItems)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_FILTERED_INVENTORY, SerializeItems(this->m_filteredInventoryItems)));
  auto& equippedItemsRef = this->equippedItems;
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_EQUIPPED, SerializeItems(std::list<ITEM_INFO>(std::begin(equippedItemsRef), std::end(equippedItemsRef)))));
  auto& alternateEquippedItemsRef = this->alternateEquippedItems;
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_ALTERNATE_EQUIPPED, SerializeItems(std::list<ITEM_INFO>(std::begin(alternateEquippedItemsRef), std::end(alternateEquippedItemsRef)))));
  auto& costumeItemsRef = this->costumeItems;
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_COSTUME, SerializeItems(std::list<ITEM_INFO>(std::begin(costumeItemsRef), std::end(costumeItemsRef)))));
  auto& alternateCostumeItemsRef = this->alternateCostumeItems;
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_ALTERNATE_COSTUME, SerializeItems(std::list<ITEM_INFO>(std::begin(alternateCostumeItemsRef), std::end(alternateCostumeItemsRef)))));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_EXCHANGE, SerializeItems(std::list<ITEM_INFO>(std::begin(this->m_exchangeItems), std::end(this->m_exchangeItems)))));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_ITEMS_SLOTS, SerializeItems(std::list<ITEM_INFO>(std::begin(this->m_itemSlots), std::end(this->m_itemSlots)))));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_ITEMS_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00E42F20
// FUNCTION: fn_24204
std::vector<unsigned char> CSession::BuildCompanionReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_BEGIN, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_AID, &this->m_homunAid, sizeof(this->m_homunAid)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_NAME, this->m_homunName, sizeof(this->m_homunName)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_0, &this->m_homunStats[0], sizeof(this->m_homunStats[0])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_1, &this->m_homunStats[1], sizeof(this->m_homunStats[1])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_2, &this->m_homunStats[2], sizeof(this->m_homunStats[2])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_3, &this->m_homunStats[3], sizeof(this->m_homunStats[3])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_4, &this->m_homunStats[4], sizeof(this->m_homunStats[4])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_5, &this->m_homunStats[5], sizeof(this->m_homunStats[5])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_6, &this->m_homunStats[6], sizeof(this->m_homunStats[6])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_STAT_7, &this->m_homunStats[7], sizeof(this->m_homunStats[7])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_JOB, &this->m_homunJob, sizeof(this->m_homunJob)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_LEVEL, &this->m_homunLevel, sizeof(this->m_homunLevel)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_HP, &this->m_homunHp, sizeof(this->m_homunHp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_MAX_HP, &this->m_homunMaxHp, sizeof(this->m_homunMaxHp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_SP, &this->m_homunSp, sizeof(this->m_homunSp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_MAX_SP, &this->homunMaxSp, sizeof(this->homunMaxSp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_FRIENDLY, &this->homunFriendly, sizeof(this->homunFriendly)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_ACCESSORY, &this->homunAccessory, sizeof(this->homunAccessory)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_EXP, &this->m_homunExp, sizeof(this->m_homunExp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_MAX_EXP, &this->m_homunMaxExp, sizeof(this->m_homunMaxExp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_HUNGER, &this->homunHunger, sizeof(this->homunHunger)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_MAX_HUNGER, &this->homunMaxHunger, sizeof(this->homunMaxHunger)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_NAME_MODIFIED, &this->homunNameModified, sizeof(this->homunNameModified)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_ENEMY_GID, &this->homunEnemyGid, sizeof(this->homunEnemyGid)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_ENEMY_X, &this->homunEnemyX, sizeof(this->homunEnemyX)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_ENEMY_Y, &this->homunEnemyY, sizeof(this->homunEnemyY)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_MOVE_ENEMY, &this->moveHomunEnemy, sizeof(this->moveHomunEnemy)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_SELECT_ENEMY, &this->selectHomunEnemy, sizeof(this->selectHomunEnemy)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_ATTACK_ENEMY, &this->attackHomunEnemy, sizeof(this->attackHomunEnemy)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_ALIVE, &this->homunAlive, sizeof(this->homunAlive)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_TRACE_AI, &this->m_traceAi, sizeof(this->m_traceAi)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_ACTION_STATE, &this->m_homunActionState, sizeof(this->m_homunActionState)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_HUNGER_NOTIFIED, &this->m_homunHungerNotified, sizeof(this->m_homunHungerNotified)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_HOMUN_ATTACK_RANGE, &this->m_homunAttackRange, sizeof(this->m_homunAttackRange)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_AID, &this->m_mercenaryAid, sizeof(this->m_mercenaryAid)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_NAME, this->m_mercenaryName, sizeof(this->m_mercenaryName)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_0, &this->m_mercenaryStats[0], sizeof(this->m_mercenaryStats[0])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_1, &this->m_mercenaryStats[1], sizeof(this->m_mercenaryStats[1])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_2, &this->m_mercenaryStats[2], sizeof(this->m_mercenaryStats[2])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_3, &this->m_mercenaryStats[3], sizeof(this->m_mercenaryStats[3])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_4, &this->m_mercenaryStats[4], sizeof(this->m_mercenaryStats[4])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_5, &this->m_mercenaryStats[5], sizeof(this->m_mercenaryStats[5])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_6, &this->m_mercenaryStats[6], sizeof(this->m_mercenaryStats[6])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_STAT_7, &this->m_mercenaryStats[7], sizeof(this->m_mercenaryStats[7])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_HP, &this->m_mercenaryHp, sizeof(this->m_mercenaryHp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_MAX_HP, &this->m_mercenaryMaxHp, sizeof(this->m_mercenaryMaxHp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_CLASS, &this->m_mercenaryClass, sizeof(this->m_mercenaryClass)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_LEVEL, &this->m_mercenaryLevel, sizeof(this->m_mercenaryLevel)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_SP, &this->m_mercenarySp, sizeof(this->m_mercenarySp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_MAX_SP, &this->m_mercenaryMaxSp, sizeof(this->m_mercenaryMaxSp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_LOYALTY, &this->m_mercenaryLoyalty, sizeof(this->m_mercenaryLoyalty)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_SUMMONS, &this->m_mercenarySummons, sizeof(this->m_mercenarySummons)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_KILLS, &this->m_mercenaryKills, sizeof(this->m_mercenaryKills)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_ATTACK_RANGE, &this->m_mercenaryAttackRange, sizeof(this->m_mercenaryAttackRange)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_EXPIRATION, &this->m_mercenaryExpiration, sizeof(this->m_mercenaryExpiration)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_ENEMY_GID, &this->mercenaryEnemyGid, sizeof(this->mercenaryEnemyGid)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_ENEMY_X, &this->mercenaryEnemyX, sizeof(this->mercenaryEnemyX)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_ENEMY_Y, &this->mercenaryEnemyY, sizeof(this->mercenaryEnemyY)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_MOVE_ENEMY, &this->moveMercenaryEnemy, sizeof(this->moveMercenaryEnemy)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_SELECT_ENEMY, &this->selectMercenaryEnemy, sizeof(this->selectMercenaryEnemy)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_ATTACK_ENEMY, &this->attackMercenaryEnemy, sizeof(this->attackMercenaryEnemy)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_ALIVE, &this->mercenaryAlive, sizeof(this->mercenaryAlive)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_MERCENARY_ACTION_STATE, &this->m_mercenaryActionState, sizeof(this->m_mercenaryActionState)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_GID, &this->petGid, sizeof(this->petGid)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_NAME_MODIFIED, &this->m_petNameModified, sizeof(this->m_petNameModified)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_NAME, this->petName, 32));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_ACCESSORY, &this->petAccessory, sizeof(this->petAccessory)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_JOB, &this->petJob, sizeof(this->petJob)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_LEVEL, &this->petLevel, sizeof(this->petLevel)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_HUNGER, &this->petHunger, sizeof(this->petHunger)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_FRIENDLY, &this->m_petFriendly, sizeof(this->m_petFriendly)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_EGG_INDEX, &this->petEggIndex, sizeof(this->petEggIndex)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_PET_PREVIOUS_HUNGER, &this->petPreviousHunger, sizeof(this->petPreviousHunger)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_ELEMENTAL_GID, &this->elementalGid, sizeof(this->elementalGid)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_ELEMENTAL_HP, &this->elementalHp, sizeof(this->elementalHp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_ELEMENTAL_SP, &this->elementalSp, sizeof(this->elementalSp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_ELEMENTAL_MAX_HP, &this->elementalMaxHp, sizeof(this->elementalMaxHp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_ELEMENTAL_MAX_SP, &this->elementalMaxSp, sizeof(this->elementalMaxSp)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_COMPANIONS_END, nullptr, 0));
  return records;
}

namespace {
#pragma pack(push, 1)
struct ReplayShopNameRecord {
  /* 0000 */ std::int16_t reserved;
  /* 0002 */ char name[16];
  /* 0012 */ int shopId;
};
#pragma pack(pop)

struct ReplayShopItemRecord {
  // INLINED
  ReplayShopItemRecord(int shopId, unsigned int index, int itemId, int first, int second)
      : shopId(shopId), index(index), itemId(itemId), first(first), second(second) {}

  /* 0000 */ int shopId;
  /* 0004 */ unsigned int index;
  /* 0008 */ int itemId;
  /* 000C */ int first;
  /* 0010 */ int second;
};

struct ReplayShopFlagRecord {
  /* 0000 */ int shopId;
  /* 0004 */ char flag;
};
}

// OFFSET: 0x00E30A90
// FUNCTION: fn_24171
std::vector<unsigned char> MessengerMember::Serialize() const {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(251, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(253, &this->valid, sizeof(this->valid)));
  AppendRecord(records, MakeReplayRecord(254, &this->accountId, sizeof(this->accountId)));
  AppendRecord(records, MakeReplayRecord(255, &this->characterId, sizeof(this->characterId)));
  AppendRecord(records, MakeReplayRecord(258, &this->leader, sizeof(this->leader)));
  AppendRecord(records, MakeReplayRecord(259, &this->offline, sizeof(this->offline)));
  AppendRecord(records, MakeReplayRecord(260, &this->color, sizeof(this->color)));
  AppendRecord(records, BuildReplayTaggedRecord(256, this->name.c_str(), this->name.size()));
  AppendRecord(records, BuildReplayTaggedRecord(257, this->map.c_str(), this->map.size()));
  AppendRecord(records, MakeReplayRecord(252, nullptr, 0));
  return records;
}

// OFFSET: 0x00E3E550
// FUNCTION: fn_24197
std::vector<unsigned char> CSession::BuildCharacterReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_BEGIN, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_AID, &this->m_aid, sizeof(this->m_aid)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_GID, &this->m_gid, sizeof(this->m_gid)));
  AppendRecord(records, MakeReplayRecord(1012, &this->unknown_1620[0], sizeof(this->unknown_1620[0])));
  AppendRecord(records, MakeReplayRecord(1013, &this->unknown_1620[1], sizeof(this->unknown_1620[1])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_JOB, &this->m_job, sizeof(this->m_job)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EXP, &this->m_exp, 8));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_LEVEL, &this->m_level, sizeof(this->m_level)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STATUS_POINT, &this->m_statusPoint, sizeof(this->m_statusPoint)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_NEXT_EXP, &this->m_nextExp, 8));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_JOB_LEVEL, &this->m_jobLevel, sizeof(this->m_jobLevel)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_SKILL_POINT, &this->m_skillPoint, 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_GUILD_SKILL_POINT, &this->m_guildSkillPoint, sizeof(this->m_guildSkillPoint)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_HOMUN_SKILL_POINT, &this->m_homunSkillPoint, sizeof(this->m_homunSkillPoint)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_MERCENARY_SKILL_POINT, &this->m_mercenarySkillPoint, sizeof(this->m_mercenarySkillPoint)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STANDARD_STAT_0, &this->m_standardStats[0], sizeof(this->m_standardStats[0])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STANDARD_STAT_1, &this->m_standardStats[1], sizeof(this->m_standardStats[1])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STANDARD_STAT_2, &this->m_standardStats[2], sizeof(this->m_standardStats[2])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STANDARD_STAT_3, &this->m_standardStats[3], sizeof(this->m_standardStats[3])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STANDARD_STAT_4, &this->m_standardStats[4], sizeof(this->m_standardStats[4])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STANDARD_STAT_5, &this->m_standardStats[5], sizeof(this->m_standardStats[5])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_0, &this->m_statusPointCost[0], sizeof(this->m_statusPointCost[0])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_1, &this->m_statusPointCost[1], sizeof(this->m_statusPointCost[1])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_2, &this->m_statusPointCost[2], sizeof(this->m_statusPointCost[2])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_3, &this->m_statusPointCost[3], sizeof(this->m_statusPointCost[3])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_4, &this->m_statusPointCost[4], sizeof(this->m_statusPointCost[4])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_STATUS_POINT_COST_5, &this->m_statusPointCost[5], sizeof(this->m_statusPointCost[5])));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ASPD, &this->m_aspd, sizeof(this->m_aspd)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ATK1, &this->m_atk1, sizeof(this->m_atk1)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_MDEF1, &this->m_mdef1, sizeof(this->m_mdef1)));
  AppendRecord(records, MakeReplayRecord(1039, &this->m_sp36, sizeof(this->m_sp36)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_DEF1, &this->m_def1, sizeof(this->m_def1)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_DEF2, &this->m_def2, sizeof(this->m_def2)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ATK2, &this->m_atk2, sizeof(this->m_atk2)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_MATK1, &this->m_matk1, sizeof(this->m_matk1)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_MATK2, &this->m_matk2, sizeof(this->m_matk2)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_MDEF2, &this->m_mdef2, sizeof(this->m_mdef2)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_HIT, &this->m_hit, sizeof(this->m_hit)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_FLEE1, &this->m_flee1, sizeof(this->m_flee1)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CRITICAL, &this->m_critical, sizeof(this->m_critical)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_FLEE2, &this->m_flee2, sizeof(this->m_flee2)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIPPED_ARROW_INDEX, &this->equippedArrowIndex, sizeof(this->equippedArrowIndex)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ZENY, &this->m_zeny, sizeof(this->m_zeny)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_MOVE_SPEED, &this->m_moveSpeed, sizeof(this->m_moveSpeed)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_MANNER, &this->m_manner, sizeof(this->m_manner)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_NEXT_JOB_EXP, &this->m_nextJobExp, 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_JOB_EXP, &this->m_jobExp, 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_KARMA, &this->m_karma, sizeof(this->m_karma)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_MONSTER_INFO_ENABLED, &this->monsterInfoEnabled, sizeof(this->monsterInfoEnabled)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CLIENT_TIME_OFFSET, &this->m_clientTimeOffset, sizeof(this->m_clientTimeOffset)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CHARACTER_FLAGS, &this->m_characterFlags, sizeof(this->m_characterFlags)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_HEAD, &this->m_head, sizeof(this->m_head)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_WEAPON_LOOK, &this->m_weaponLook, sizeof(this->m_weaponLook)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_SHIELD, &this->m_shield, sizeof(this->m_shield)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_BODY_STYLE, &this->m_bodyStyle, sizeof(this->m_bodyStyle)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_HEAD_PALETTE, &this->m_headPalette, sizeof(this->m_headPalette)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ACCESSORY, &this->m_accessory, sizeof(this->m_accessory)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ACCESSORY2, &this->m_accessory2, sizeof(this->m_accessory2)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ACCESSORY3, &this->m_accessory3, sizeof(this->m_accessory3)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_BODY_STATE, &this->m_bodyState, sizeof(this->m_bodyState)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EFFECT_STATE, &this->m_effectState, sizeof(this->m_effectState)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_JOB_FLAGS, &this->m_jobFlags, sizeof(this->m_jobFlags)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ROBE, &this->m_robe, sizeof(this->m_robe)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_0, &this->m_equipViewLook[0], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_1, &this->m_equipViewLook[1], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_2, &this->m_equipViewLook[2], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_3, &this->m_equipViewLook[3], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_4, &this->m_equipViewLook[4], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_5, &this->m_equipViewLook[5], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_6, &this->m_equipViewLook[6], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_7, &this->m_equipViewLook[7], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_LOOK_8, &this->m_equipViewLook[8], 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EQUIP_VIEW_NAME, this->m_equipViewName, 0x18));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_PLAYER_TEAM, &this->playerTeam, 1));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_SCORE1, &this->m_score1, sizeof(this->m_score1)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_SCORE2, &this->m_score2, sizeof(this->m_score2)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CHAT_FONT, &this->chatFont, 1));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CART_COUNT, &this->cartCount, sizeof(this->cartCount)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CART_MAX_COUNT, &this->cartMaxCount, sizeof(this->cartMaxCount)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CART_WEIGHT, &this->cartWeight, sizeof(this->cartWeight)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CART_MAX_WEIGHT, &this->cartMaxWeight, sizeof(this->cartMaxWeight)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_VENDING_SLOT_COUNT, &this->m_vendingSlotCount, 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_BUYING_STORE_SLOT_COUNT, &this->m_buyingStoreSlotCount, 4));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_CHAT_TARGET, &this->m_chatTarget, sizeof(this->m_chatTarget)));
  AppendRecord(records, MakeReplayRecord(1093, &this->m_replayValue1093, sizeof(this->m_replayValue1093)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_PARTY_EXP_SHARE, &this->partyExpShare, sizeof(this->partyExpShare)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_PARTY_ITEM_COLLECT, &this->partyItemCollect, sizeof(this->partyItemCollect)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_PARTY_ITEM_DISTRIBUTE, &this->partyItemDistribute, sizeof(this->partyItemDistribute)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_CHARACTER_STORE_TITLE, this->storeTitle.c_str(), this->storeTitle.size()));
  const unsigned char option = g_serviceOptions.Get(TT_SHOW_EMBLEM_ON_OFF);
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_SHOW_EMBLEM, &option, sizeof(option)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_ADOPTION_SKILL_AVAILABLE, &this->adoptionSkillAvailable, sizeof(this->adoptionSkillAvailable)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_BODY_COSTUME, &this->m_bodyCostume, sizeof(this->m_bodyCostume)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_EXTRA_INVENTORY_SLOTS, &this->extraInventorySlots, sizeof(this->extraInventorySlots)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_CHARACTER_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00E41E60
// FUNCTION: fn_24201
std::vector<unsigned char> CSession::BuildShopReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHOPS_BEGIN, nullptr, 0));
  for (auto it = this->m_nameToId.begin(); it != this->m_nameToId.end(); ++it) {
    ReplayShopNameRecord value = {0};
    value.shopId = it->second;
    const std::size_t length = (std::min)(it->first.size(), sizeof(value.name));
    std::memcpy(value.name, it->first.c_str(), length);
    AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHOPS_NAME, &value, sizeof(value)));
  }
  for (const auto& shop : this->m_shops) {
    for (unsigned int index = 0; index < shop.second.items.size(); ++index) {
      const std::map<int, std::pair<int, int>>& items = shop.second.items[index];
      for (const auto& item : items) {
        const ReplayShopItemRecord value(shop.first, index, item.first, item.second.first, item.second.second);
        AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHOPS_ITEM, &value, sizeof(value)));
      }
    }
    ReplayShopFlagRecord value;
    value.shopId = shop.first;
    value.flag = shop.second.flag;
    AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHOPS_FLAG, &value, sizeof(value)));
  }
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_SHOPS_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00E42600
// FUNCTION: fn_24202
std::vector<unsigned char> CSession::BuildExtraReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_BEGIN, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_WINDOW_OPEN, &this->memorialWindowOpen, sizeof(this->memorialWindowOpen)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_WINDOW_X, &this->memorialWindowX, sizeof(this->memorialWindowX)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_WINDOW_Y, &this->memorialWindowY, sizeof(this->memorialWindowY)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_DUNGEON_STATE, &this->memorialDungeonState, sizeof(this->memorialDungeonState)));
  AppendRecord(records, MakeReplayRecord(5514, &this->memorialReplayValue, sizeof(this->memorialReplayValue)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_MEMORIAL_DUNGEON_NAME, this->memorialDungeonName.c_str(), this->memorialDungeonName.size()));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_QUEUE_POSITION, &this->memorialQueuePosition, sizeof(this->memorialQueuePosition)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_CREATION_TIME, &this->memorialCreationTime, sizeof(this->memorialCreationTime)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_EXPIRATION_TIME, &this->memorialExpirationTime, sizeof(this->memorialExpirationTime)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MEMORIAL_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00E3FF30
// FUNCTION: fn_24198
std::vector<unsigned char> CSession::BuildStatsReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_STATS_BEGIN, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_STATS_STRENGTH_BONUS, &this->strengthBonus, sizeof(this->strengthBonus)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_STATS_AGILITY_BONUS, &this->agilityBonus, sizeof(this->agilityBonus)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_STATS_VITALITY_BONUS, &this->vitalityBonus, sizeof(this->vitalityBonus)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_STATS_INTELLIGENCE_BONUS, &this->intelligenceBonus, sizeof(this->intelligenceBonus)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_STATS_DEXTERITY_BONUS, &this->dexterityBonus, sizeof(this->dexterityBonus)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_STATS_LUCK_BONUS, &this->luckBonus, sizeof(this->luckBonus)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_STORE_ITEM_LIMIT, &this->storeItemLimit, sizeof(this->storeItemLimit)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_WEIGHT, &this->m_weight, sizeof(this->m_weight)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_MAX_WEIGHT, &this->m_maxWeight, sizeof(this->m_maxWeight)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_A, &this->randomStatIndexA, sizeof(this->randomStatIndexA)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_B, &this->randomStatIndexB, sizeof(this->randomStatIndexB)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_C, &this->randomStatIndexC, sizeof(this->randomStatIndexC)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_D, &this->randomStatIndexD, sizeof(this->randomStatIndexD)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_RANDOM_STAT_INDEX_E, &this->randomStatIndexE, sizeof(this->randomStatIndexE)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_RANDOM_STAT_VALUES, this->randomStatValues, sizeof(this->randomStatValues)));
  AppendRecord(records, BuildReplayTaggedRecord(2025, this->replayStatValuesA, sizeof(this->replayStatValuesA)));
  AppendRecord(records, BuildReplayTaggedRecord(2026, this->replayStatValuesB, sizeof(this->replayStatValuesB)));
  AppendRecord(records, BuildReplayTaggedRecord(2027, this->replayStatValuesC, sizeof(this->replayStatValuesC)));
  AppendRecord(records, BuildReplayTaggedRecord(2028, this->replayStatValuesD, 0x30));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_HP, &this->hp, sizeof(this->hp)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_MAX_HP, &this->maxHp, sizeof(this->maxHp)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_SP, &this->m_sp, sizeof(this->m_sp)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_MAX_SP, &this->m_maxSp, sizeof(this->m_maxSp)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_PROHIBITION_TARGET, &this->m_prohibitionTarget, sizeof(this->m_prohibitionTarget)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_QUIT_BLOCKED, &this->quitBlocked, sizeof(this->quitBlocked)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_BATTLE_CHAT, &this->battleChat, sizeof(this->battleChat)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_QUIT_ALLOWED, &this->quitAllowed, sizeof(this->quitAllowed)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_SESSION_FLAG, &this->sessionFlag, sizeof(this->sessionFlag)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_ACCOUNT_NAME, this->m_accountName.c_str(), this->m_accountName.size()));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_STATS_MADOGEAR_TYPE, &this->madogearType, sizeof(this->madogearType)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_STATS_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00E40C60
// FUNCTION: fn_24199
std::vector<unsigned char> CSession::BuildMessengerReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MESSENGER_BEGIN, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MESSENGER_SESSION_FLAG, &this->replaySessionFlag, 1));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MESSENGER_PARTY_BLOCKED, &this->partyBlocked, sizeof(this->partyBlocked)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MESSENGER_PARTY_LEADER, &this->partyLeader, sizeof(this->partyLeader)));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_MESSENGER_PARTY_REQUEST_NAME, this->partyRequestName.c_str(), this->partyRequestName.size()));
  AppendRecord(records, BuildReplayTaggedRecord(REPLAY_CHUNK_MESSENGER_PARTY_NAME, this->partyName.c_str(), this->partyName.size()));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_MESSENGER_PARTY_MEMBERS, SerializeMessengerMembers(this->m_partyMembers)));
  AppendRecord(records, MakeFramedMessage(REPLAY_CHUNK_MESSENGER_FRIENDS, SerializeMessengerMembers(this->m_friends)));
  AppendRecord(records, MakeReplayRecord(REPLAY_CHUNK_MESSENGER_END, nullptr, 0));
  return records;
}

// OFFSET: 0x00B83CB0
// FUNCTION: fn_46521
ReplayCachedPacket::~ReplayCachedPacket() {
  this->bytes.clear();
}
