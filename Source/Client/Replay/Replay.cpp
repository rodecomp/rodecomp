#include "File/CommonObject.h"
#include "Client/Replay/Replay.h"
#include "UI/Window/WindowId.h"
#include "Network/Packets/PacketId.h"
#include "Contents/Configuration/ServiceConfiguration.h"
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

template __declspec(noinline) void std::vector<unsigned char>::assign<const unsigned char*, 0>(const unsigned char*, const unsigned char*);
template __declspec(noinline) void std::list<ITEM_INFO>::_Construct_range_unchecked<ITEM_INFO*>(ITEM_INFO*, ITEM_INFO*);

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
  ReplayMetadataTag() : id(0), bytes(0) {}
  // INLINED
  ~ReplayMetadataTag() {
    this->id = 0;
    this->bytes = 0;
  }

  // INLINED
  void Read(const unsigned char* source) {
    this->bytes = 0;
    this->id = 0;
    std::memcpy(this, source, sizeof(*this));
  }
};
#pragma pack(pop)

static_assert(sizeof(ReplayTaggedRecordHeader) == 6);
static_assert(sizeof(ReplayMetadataTag) == 6);
static_assert(sizeof(ReplayRecordHeader) == 10);

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
std::vector<unsigned char> BuildEntryQueueReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(6500, nullptr, 0));
  if (g_entryQueueMgr->current != nullptr) {
    const int id = g_entryQueueMgr->current->id;
    AppendRecord(records, MakeReplayRecord(6511, &id, sizeof(id)));
  }
  const unsigned char ready = g_entryQueueMgr->ready;
  AppendRecord(records, MakeReplayRecord(6512, &ready, sizeof(ready)));
  std::string text = g_entryQueueMgr->queueName.text;
  if (text.size() != 0) {
    AppendRecord(records, BuildReplayTaggedRecord(6513, text.c_str(), text.size()));
  }
  text = g_entryQueueMgr->characterName.text;
  if (text.size() != 0) {
    AppendRecord(records, BuildReplayTaggedRecord(6514, text.c_str(), text.size()));
  }
  if (UIEntryQueueWnd* window = dynamic_cast<UIEntryQueueWnd*>(g_windowMgr.LookupWindow(WID_JOINTO_BATTLEFIELDWND))) {
    const int selected = window->SelectedIndex();
    AppendRecord(records, MakeReplayRecord(6515, &selected, sizeof(selected)));
  }
  if (UIEntryQueueStandByWnd* window = dynamic_cast<UIEntryQueueStandByWnd*>(g_windowMgr.LookupWindow(WID_ENTRYQUEUESTANDBYWND))) {
    AppendRecord(records, MakeReplayRecord(6517, &window->m_waiting, sizeof(window->m_waiting)));
    AppendRecord(records, MakeReplayRecord(6518, &window->m_ranking, sizeof(window->m_ranking)));
    AppendRecord(records, BuildReplayTaggedRecord(6516, window->m_queueName.c_str(), window->m_queueName.size()));
  }
  AppendRecord(records, MakeReplayRecord(6501, nullptr, 0));
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

// OFFSET: 0x00B855B0
// FUNCTION: fn_19326
bool Replay::WriteSection(int id, const std::vector<unsigned char>& bytes, int event) {
  if (bytes.empty()) {
    return false;
  }

  const std::uint32_t size = static_cast<std::uint32_t>(bytes.size());
  unsigned char* data = const_cast<unsigned char*>(bytes.data());
  if (id <= 0 || id >= 26 || size == 0 || data == nullptr) {
    return false;
  }

  if (event == 14) {
    this->sections[id].id = static_cast<std::uint16_t>(id);
    this->sections[id].bytes = size;
    this->sections[id].fileOffset = this->fileOffset;
    this->fileOffset += size;
  }

  const std::uint32_t sectionBytes = this->sections[id].bytes;
  if (sectionBytes != 0) {
    auto* words = reinterpret_cast<std::uint32_t*>(data);
    this->cipherBase = this->header.key0 >> 5;
    const int count = sectionBytes / 4;
    this->cipherIndex = 0;
    this->cipherKey = this->header.key1 >> 3;

    for (int index = 0; index < count; ++index) {
      ++this->cipherIndex;
      words[index] ^= (this->cipherBase + this->cipherIndex) * this->cipherKey;
    }
  }

  std::fseek(this->file, static_cast<long>(this->sections[id].fileOffset), SEEK_SET);
  return std::fwrite(data, this->sections[id].bytes, 1, this->file) != 0;
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
  AppendRecord(metadata, MakeReplayRecord(950, nullptr, 0));
  AppendRecord(metadata, MakeReplayRecord(961, &this->header.service, sizeof(this->header.service)));
  AppendRecord(metadata, MakeReplayRecord(962, &this->header.serviceType, sizeof(this->header.serviceType)));
  AppendRecord(metadata, MakeReplayRecord(963, &this->header.sessionSyncValue, sizeof(this->header.sessionSyncValue)));
  AppendRecord(metadata, MakeReplayRecord(964, this->header.characterName, sizeof(this->header.characterName)));
  AppendRecord(metadata, MakeReplayRecord(965, this->header.mapName, sizeof(this->header.mapName)));
  AppendRecord(metadata, MakeReplayRecord(966, &this->header.accountId, sizeof(this->header.accountId)));
  AppendRecord(metadata, MakeReplayRecord(967, &this->header.cellX, sizeof(this->header.cellX)));
  AppendRecord(metadata, MakeReplayRecord(968, &this->header.cellY, sizeof(this->header.cellY)));
  AppendRecord(metadata, MakeReplayRecord(969, &this->header.playerDirection, sizeof(this->header.playerDirection)));
  AppendRecord(metadata, MakeReplayRecord(970, &this->header.duration, sizeof(this->header.duration)));
  AppendRecord(metadata, MakeReplayRecord(971, &this->header.records, sizeof(this->header.records)));
  AppendRecord(metadata, MakeReplayRecord(972, &this->header.fileOffset, sizeof(this->header.fileOffset)));
  AppendRecord(metadata, MakeReplayRecord(973, &this->header.options, sizeof(this->header.options)));
  AppendRecord(metadata, MakeReplayRecord(960, &this->header.closingSignature, sizeof(this->header.closingSignature)));
  AppendRecord(metadata, MakeReplayRecord(951, nullptr, 0));
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
    if (type == 0 || type == 13 || type == 1 || type == 2) {
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
    return 1;
  }

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
bool Replay::Cipher(std::uint32_t bytes, void* data) {
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
  this->header.Reset();
  this->records = 0;
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
    data.resize(this->sections[index].bytes, 0);
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
  std::uint32_t offset = 6;
  tag.Read(&data[0]);

  if (0x3B6 != tag.id) {
    offset = 0xFFFFFFFF;
  } else {
    while (data.size() > offset) {
      tag.Read(&data[offset]);
      offset += 6;

      switch (tag.id) {
      case 0x3B7:
        return offset;
      case 0x3C1:
        std::memcpy(&this->header.service, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C2:
        std::memcpy(&this->header.serviceType, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C3:
        std::memcpy(&this->header.sessionSyncValue, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C6:
        std::memcpy(&this->header.accountId, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C4:
        std::memcpy(this->header.characterName, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C5:
        std::memcpy(this->header.mapName, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C7:
        std::memcpy(&this->header.cellX, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C8:
        std::memcpy(&this->header.cellY, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C9:
        std::memcpy(&this->header.playerDirection, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3CA:
        std::memcpy(&this->header.duration, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3CB:
        std::memcpy(&this->header.records, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3CC:
        std::memcpy(&this->header.fileOffset, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3CD:
        std::memcpy(&this->header.options, &data[offset], tag.bytes);
        offset += tag.bytes;
        break;
      case 0x3C0:
        std::memcpy(&this->header.closingSignature, &data[offset], tag.bytes);
        offset += tag.bytes;

        if (this->header.signature != this->header.closingSignature) {
          std::uint32_t fileOffset = this->fileOffset;
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

          this->header.duration = record.tick;
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

  ReplayRecordHeader record;
  bool result = false;
  std::fseek(this->file, static_cast<long>(this->fileOffset), SEEK_SET);
  std::fread(&record, sizeof(record), 1, this->file);

  if (this->header.records > this->records && this->elapsed >= record.tick) {
    std::fread(buffer, record.bytes, 1, this->file);
    this->Cipher(record.bytes, buffer);
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

  if (search != INVALID_HANDLE_VALUE) {
    while (more) {
      if ((found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        const char* extension = found.cFileName + std::strlen(found.cFileName) - 4;
        const BOOL isReplay = extension[3] == 'f' && extension[2] == 'r' && extension[1] == 'r' && extension[0] == '.';

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
    const ReplayFileEntry& entry = this->files[this->selected];
    name.append(entry.name, std::strlen(entry.name));
    std::memset(this->path, 0, sizeof(this->path));
    std::memcpy(this->path, name.data(), name.size());

    Replay* replay = this->replay;
    replay->fileMode = 0x20;
    replay->file = std::fopen(this->path, "rb");

    if (replay->file) {
      std::fseek(replay->file, 100, SEEK_SET);
      std::memset(&replay->header, 0, 0xC);
      std::fread(&replay->header, 0xC, 1, replay->file);

      if (replay->header.signature == 0x52414705u || replay->header.signature == 0x51404605u) {
        for (int index = 0; index <= 0x1A; ++index) {
          ReplaySection section = {};
          std::fread(&section, sizeof(section), 1, replay->file);

          if (section.id < 0 || section.id > 0x1A) {
            break;
          }

          replay->sections[section.id].id = section.id;
          replay->sections[section.id].bytes = section.bytes;
          replay->sections[section.id].fileOffset = section.fileOffset;
        }

        if (replay->ParseMetadata(replay->ReadSection(2)) != 0xFFFFFFFFu) {
          return 1;
        }

        std::memset(&replay->header, 0, sizeof(replay->header));
      }

      if (replay->file) {
        std::fclose(replay->file);
        replay->file = nullptr;
      }

      std::memset(&replay->header, 0, sizeof(replay->header));
      std::memset(replay->sections, 0, sizeof(replay->sections));
    }

    return 0;
  }

  this->Refresh();

  if (!this->pathEnabled) {
    int index = 1;

    for (std::size_t i = 0; i < this->files.size(); ++i) {
      if (this->files[i].index == index) {
        ++index;
      }
    }

    std::memset(this->path, 0, sizeof(this->path));
    std::sprintf(this->path, g_replayNewFileFormat, index);
  } else {
    if (this->rejectExistingName) {
      char name[0x104];
      std::memset(name, 0, sizeof(name));
      std::sprintf(name, g_replayExtensionFormat, this->path);

      for (std::size_t i = 0; i < this->files.size(); ++i) {
        if (std::strcmp(name, this->files[i].name) == 0) {
          std::memset(this->path, 0, sizeof(this->path));
          return -2;
        }
      }
    }

    char name[0x104];
    std::memset(name, 0, sizeof(name));
    std::sprintf(name, g_replayPathFormat, this->path);
    std::memcpy(this->path, name, sizeof(this->path));
  }

  this->replay->fileMode = 0x1F;
  this->replay->file = std::fopen(this->path, "wb");

  if (!this->replay->file) {
    return 0;
  }

  this->replay->fileOffset = 0x174;
  return 1;
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
  unsigned char* data = packet.data();
  *reinterpret_cast<std::uint16_t*>(data) = 0x983;
  *reinterpret_cast<std::uint16_t*>(data + 2) = 0x2A1;
  *reinterpret_cast<std::uint32_t*>(data + 9) = 9999;
  *reinterpret_cast<std::uint32_t*>(data + 4) = this->m_aid;
  data[8] = 1;
  *reinterpret_cast<std::uint32_t*>(data + 13) = 9999;
  *reinterpret_cast<std::uint32_t*>(data + 17) = this->replayPacketMode;
  return packet;
}

// OFFSET: 0x00E31240
// FUNCTION: fn_24172
std::vector<unsigned char> BuildSkillInfoListPacket(std::uint16_t id, const std::list<CSkillInfo>& skills) {
  const int bytes = static_cast<int>(skills.size()) * 37 + 4;
  std::vector<unsigned char> packet(bytes, 0);
  unsigned char* data = packet.data();
  *reinterpret_cast<std::uint16_t*>(data) = id;
  *reinterpret_cast<std::uint16_t*>(data + 2) = static_cast<std::uint16_t>(bytes);
  unsigned char* record = data + 4;
  for (const CSkillInfo& skill : skills) {
    *reinterpret_cast<std::uint16_t*>(record) = static_cast<std::uint16_t>(skill.id);
    *reinterpret_cast<std::uint32_t*>(record + 2) = skill.type;
    *reinterpret_cast<std::uint16_t*>(record + 6) = static_cast<std::uint16_t>(skill.level);
    *reinterpret_cast<std::uint16_t*>(record + 8) = static_cast<std::uint16_t>(skill.spCost);
    record[36] = skill.upgradable != 0;
    *reinterpret_cast<std::uint16_t*>(record + 10) = static_cast<std::uint16_t>(skill.range);
    const char* name = skill.name;
    std::memcpy(record + 12, name, (std::min)(std::strlen(name), std::size_t{24}));
    record[35] = 0;
    record += 37;
  }
  return packet;
}

// OFFSET: 0x00E44640
// FUNCTION: fn_24205
std::vector<unsigned char> BuildRenewQuestReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(3500, nullptr, 0));
  {
    const int version = 51;
    AppendRecord(records, MakeReplayRecord(3502, &version, sizeof(version)));
  }
  const int size = g_renewQuestMgr->GetOngoingQuestsSize();
  char* buffer = new char[size];
  g_renewQuestMgr->WriteOngoingQuests(buffer);
  AppendRecord(records, BuildReplayTaggedRecord(3510, buffer, size));
  if (buffer != nullptr) {
    delete buffer;
  }
  AppendRecord(records, MakeReplayRecord(3501, nullptr, 0));
  return records;
}

// OFFSET: 0x00E449C0
// FUNCTION: fn_24206
std::vector<unsigned char> CSession::BuildShortcutReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(7000, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(7100, this->m_shortcutSlots, sizeof(this->m_shortcutSlots)));
  AppendRecord(records, MakeReplayRecord(7101, &this->quickSlot, sizeof(this->quickSlot) + sizeof(this->alternateQuickSlot)));
  AppendRecord(records, MakeReplayRecord(7001, nullptr, 0));
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
  AppendRecord(records, MakeReplayRecord(4500, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(4701, &this->exchangeMoney, sizeof(this->exchangeMoney)));
  AppendRecord(records, MakeReplayRecord(4702, &this->otherExchangeMoney, sizeof(this->otherExchangeMoney)));
  AppendRecord(records, MakeFramedMessage(4510, SerializeItems(this->m_inventoryItems)));
  AppendRecord(records, MakeFramedMessage(4511, SerializeItems(this->shopItems)));
  AppendRecord(records, MakeFramedMessage(4512, SerializeItems(this->purchaseItems)));
  AppendRecord(records, MakeFramedMessage(4513, SerializeItems(this->sellItems)));
  AppendRecord(records, MakeFramedMessage(4514, SerializeItems(this->storageItems)));
  AppendRecord(records, MakeFramedMessage(4515, SerializeItems(this->cartItems)));
  AppendRecord(records, MakeFramedMessage(4516, SerializeItems(this->merchantShopItems)));
  AppendRecord(records, MakeFramedMessage(4517, SerializeItems(this->merchantMirrorItems)));
  AppendRecord(records, MakeFramedMessage(4518, SerializeItems(this->merchantMyShopItems)));
  AppendRecord(records, MakeFramedMessage(4519, SerializeItems(this->merchantPurchaseItems)));
  AppendRecord(records, MakeFramedMessage(4520, SerializeItems(this->merchantHopeToSellItems)));
  AppendRecord(records, MakeFramedMessage(4521, SerializeItems(this->merchantPurchaseSavedItems)));
  AppendRecord(records, MakeFramedMessage(4522, SerializeItems(this->filteredInventoryItems)));
  AppendRecord(records, MakeFramedMessage(4601, SerializeItems(std::list<ITEM_INFO>(std::begin(this->equippedItems), std::end(this->equippedItems)))));
  AppendRecord(records, MakeFramedMessage(4602, SerializeItems(std::list<ITEM_INFO>(std::begin(this->alternateEquippedItems), std::end(this->alternateEquippedItems)))));
  AppendRecord(records, MakeFramedMessage(4603, SerializeItems(std::list<ITEM_INFO>(std::begin(this->costumeItems), std::end(this->costumeItems)))));
  AppendRecord(records, MakeFramedMessage(4604, SerializeItems(std::list<ITEM_INFO>(std::begin(this->alternateCostumeItems), std::end(this->alternateCostumeItems)))));
  AppendRecord(records, MakeFramedMessage(4605, SerializeItems(std::list<ITEM_INFO>(std::begin(this->m_exchangeItems), std::end(this->m_exchangeItems)))));
  AppendRecord(records, MakeFramedMessage(4606, SerializeItems(std::list<ITEM_INFO>(std::begin(this->m_itemSlots), std::end(this->m_itemSlots)))));
  AppendRecord(records, MakeReplayRecord(4501, nullptr, 0));
  return records;
}

// OFFSET: 0x00E42F20
// FUNCTION: fn_24204
std::vector<unsigned char> CSession::BuildCompanionReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(5000, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(5101, &this->m_homunAid, sizeof(this->m_homunAid)));
  AppendRecord(records, MakeReplayRecord(5102, this->m_homunName, sizeof(this->m_homunName)));
  AppendRecord(records, MakeReplayRecord(5103, &this->m_homunStats[0], sizeof(this->m_homunStats[0])));
  AppendRecord(records, MakeReplayRecord(5104, &this->m_homunStats[1], sizeof(this->m_homunStats[1])));
  AppendRecord(records, MakeReplayRecord(5105, &this->m_homunStats[2], sizeof(this->m_homunStats[2])));
  AppendRecord(records, MakeReplayRecord(5106, &this->m_homunStats[3], sizeof(this->m_homunStats[3])));
  AppendRecord(records, MakeReplayRecord(5107, &this->m_homunStats[4], sizeof(this->m_homunStats[4])));
  AppendRecord(records, MakeReplayRecord(5108, &this->m_homunStats[5], sizeof(this->m_homunStats[5])));
  AppendRecord(records, MakeReplayRecord(5109, &this->m_homunStats[6], sizeof(this->m_homunStats[6])));
  AppendRecord(records, MakeReplayRecord(5110, &this->m_homunStats[7], sizeof(this->m_homunStats[7])));
  AppendRecord(records, MakeReplayRecord(5111, &this->m_homunJob, sizeof(this->m_homunJob)));
  AppendRecord(records, MakeReplayRecord(5112, &this->m_homunLevel, sizeof(this->m_homunLevel)));
  AppendRecord(records, MakeReplayRecord(5113, &this->m_homunHp, sizeof(this->m_homunHp)));
  AppendRecord(records, MakeReplayRecord(5114, &this->m_homunMaxHp, sizeof(this->m_homunMaxHp)));
  AppendRecord(records, MakeReplayRecord(5115, &this->m_homunSp, sizeof(this->m_homunSp)));
  AppendRecord(records, MakeReplayRecord(5116, &this->homunMaxSp, sizeof(this->homunMaxSp)));
  AppendRecord(records, MakeReplayRecord(5117, &this->homunFriendly, sizeof(this->homunFriendly)));
  AppendRecord(records, MakeReplayRecord(5118, &this->homunAccessory, sizeof(this->homunAccessory)));
  AppendRecord(records, MakeReplayRecord(5119, &this->m_homunExp, sizeof(this->m_homunExp)));
  AppendRecord(records, MakeReplayRecord(5120, &this->m_homunMaxExp, sizeof(this->m_homunMaxExp)));
  AppendRecord(records, MakeReplayRecord(5121, &this->homunHunger, sizeof(this->homunHunger)));
  AppendRecord(records, MakeReplayRecord(5122, &this->homunMaxHunger, sizeof(this->homunMaxHunger)));
  AppendRecord(records, MakeReplayRecord(5123, &this->homunNameModified, sizeof(this->homunNameModified)));
  AppendRecord(records, MakeReplayRecord(5124, &this->homunEnemyGid, sizeof(this->homunEnemyGid)));
  AppendRecord(records, MakeReplayRecord(5125, &this->homunEnemyX, sizeof(this->homunEnemyX)));
  AppendRecord(records, MakeReplayRecord(5126, &this->homunEnemyY, sizeof(this->homunEnemyY)));
  AppendRecord(records, MakeReplayRecord(5127, &this->moveHomunEnemy, sizeof(this->moveHomunEnemy)));
  AppendRecord(records, MakeReplayRecord(5128, &this->selectHomunEnemy, sizeof(this->selectHomunEnemy)));
  AppendRecord(records, MakeReplayRecord(5129, &this->attackHomunEnemy, sizeof(this->attackHomunEnemy)));
  AppendRecord(records, MakeReplayRecord(5130, &this->homunAlive, sizeof(this->homunAlive)));
  AppendRecord(records, MakeReplayRecord(5131, &this->m_traceAi, sizeof(this->m_traceAi)));
  AppendRecord(records, MakeReplayRecord(5132, &this->m_homunActionState, sizeof(this->m_homunActionState)));
  AppendRecord(records, MakeReplayRecord(5133, &this->m_homunHungerNotified, sizeof(this->m_homunHungerNotified)));
  AppendRecord(records, MakeReplayRecord(5134, &this->m_homunAttackRange, sizeof(this->m_homunAttackRange)));
  AppendRecord(records, MakeReplayRecord(5201, &this->m_mercenaryAid, sizeof(this->m_mercenaryAid)));
  AppendRecord(records, MakeReplayRecord(5202, this->m_mercenaryName, sizeof(this->m_mercenaryName)));
  AppendRecord(records, MakeReplayRecord(5203, &this->m_mercenaryStats[0], sizeof(this->m_mercenaryStats[0])));
  AppendRecord(records, MakeReplayRecord(5204, &this->m_mercenaryStats[1], sizeof(this->m_mercenaryStats[1])));
  AppendRecord(records, MakeReplayRecord(5205, &this->m_mercenaryStats[2], sizeof(this->m_mercenaryStats[2])));
  AppendRecord(records, MakeReplayRecord(5206, &this->m_mercenaryStats[3], sizeof(this->m_mercenaryStats[3])));
  AppendRecord(records, MakeReplayRecord(5207, &this->m_mercenaryStats[4], sizeof(this->m_mercenaryStats[4])));
  AppendRecord(records, MakeReplayRecord(5208, &this->m_mercenaryStats[5], sizeof(this->m_mercenaryStats[5])));
  AppendRecord(records, MakeReplayRecord(5209, &this->m_mercenaryStats[6], sizeof(this->m_mercenaryStats[6])));
  AppendRecord(records, MakeReplayRecord(5210, &this->m_mercenaryStats[7], sizeof(this->m_mercenaryStats[7])));
  AppendRecord(records, MakeReplayRecord(5211, &this->m_mercenaryHp, sizeof(this->m_mercenaryHp)));
  AppendRecord(records, MakeReplayRecord(5212, &this->m_mercenaryMaxHp, sizeof(this->m_mercenaryMaxHp)));
  AppendRecord(records, MakeReplayRecord(5213, &this->m_mercenaryClass, sizeof(this->m_mercenaryClass)));
  AppendRecord(records, MakeReplayRecord(5214, &this->m_mercenaryLevel, sizeof(this->m_mercenaryLevel)));
  AppendRecord(records, MakeReplayRecord(5215, &this->m_mercenarySp, sizeof(this->m_mercenarySp)));
  AppendRecord(records, MakeReplayRecord(5216, &this->m_mercenaryMaxSp, sizeof(this->m_mercenaryMaxSp)));
  AppendRecord(records, MakeReplayRecord(5217, &this->m_mercenaryLoyalty, sizeof(this->m_mercenaryLoyalty)));
  AppendRecord(records, MakeReplayRecord(5218, &this->m_mercenarySummons, sizeof(this->m_mercenarySummons)));
  AppendRecord(records, MakeReplayRecord(5219, &this->m_mercenaryKills, sizeof(this->m_mercenaryKills)));
  AppendRecord(records, MakeReplayRecord(5220, &this->m_mercenaryAttackRange, sizeof(this->m_mercenaryAttackRange)));
  AppendRecord(records, MakeReplayRecord(5221, &this->m_mercenaryExpiration, sizeof(this->m_mercenaryExpiration)));
  AppendRecord(records, MakeReplayRecord(5222, &this->mercenaryEnemyGid, sizeof(this->mercenaryEnemyGid)));
  AppendRecord(records, MakeReplayRecord(5223, &this->mercenaryEnemyX, sizeof(this->mercenaryEnemyX)));
  AppendRecord(records, MakeReplayRecord(5224, &this->mercenaryEnemyY, sizeof(this->mercenaryEnemyY)));
  AppendRecord(records, MakeReplayRecord(5225, &this->moveMercenaryEnemy, sizeof(this->moveMercenaryEnemy)));
  AppendRecord(records, MakeReplayRecord(5226, &this->selectMercenaryEnemy, sizeof(this->selectMercenaryEnemy)));
  AppendRecord(records, MakeReplayRecord(5227, &this->attackMercenaryEnemy, sizeof(this->attackMercenaryEnemy)));
  AppendRecord(records, MakeReplayRecord(5228, &this->mercenaryAlive, sizeof(this->mercenaryAlive)));
  AppendRecord(records, MakeReplayRecord(5229, &this->m_mercenaryActionState, sizeof(this->m_mercenaryActionState)));
  AppendRecord(records, MakeReplayRecord(5301, &this->petGid, sizeof(this->petGid)));
  AppendRecord(records, MakeReplayRecord(5302, &this->m_petNameModified, sizeof(this->m_petNameModified)));
  AppendRecord(records, MakeReplayRecord(5303, this->petName, 32));
  AppendRecord(records, MakeReplayRecord(5304, &this->petAccessory, sizeof(this->petAccessory)));
  AppendRecord(records, MakeReplayRecord(5305, &this->petJob, sizeof(this->petJob)));
  AppendRecord(records, MakeReplayRecord(5306, &this->petLevel, sizeof(this->petLevel)));
  AppendRecord(records, MakeReplayRecord(5307, &this->petHunger, sizeof(this->petHunger)));
  AppendRecord(records, MakeReplayRecord(5308, &this->m_petFriendly, sizeof(this->m_petFriendly)));
  AppendRecord(records, MakeReplayRecord(5309, &this->petEggIndex, sizeof(this->petEggIndex)));
  AppendRecord(records, MakeReplayRecord(5310, &this->petPreviousHunger, sizeof(this->petPreviousHunger)));
  AppendRecord(records, MakeReplayRecord(5401, &this->elementalGid, sizeof(this->elementalGid)));
  AppendRecord(records, MakeReplayRecord(5402, &this->elementalHp, sizeof(this->elementalHp)));
  AppendRecord(records, MakeReplayRecord(5403, &this->elementalSp, sizeof(this->elementalSp)));
  AppendRecord(records, MakeReplayRecord(5404, &this->elementalMaxHp, sizeof(this->elementalMaxHp)));
  AppendRecord(records, MakeReplayRecord(5405, &this->elementalMaxSp, sizeof(this->elementalMaxSp)));
  AppendRecord(records, MakeReplayRecord(5001, nullptr, 0));
  return records;
}

namespace {
#pragma pack(push, 1)
struct ReplayShopNameRecord {
  /* 0000 */ std::int16_t padding_0000;
  /* 0002 */ char name[16];
  /* 0012 */ int shopId;
};
#pragma pack(pop)

struct ReplayShopItemRecord {
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
  AppendRecord(records, MakeReplayRecord(1000, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(1010, &this->m_aid, sizeof(this->m_aid)));
  AppendRecord(records, MakeReplayRecord(1011, &this->m_gid, sizeof(this->m_gid)));
  AppendRecord(records, MakeReplayRecord(1012, &this->m_navigationPlayerMap[0x14], 4));
  AppendRecord(records, MakeReplayRecord(1013, &this->m_navigationPlayerMap[0x18], 4));
  AppendRecord(records, MakeReplayRecord(1014, &this->m_job, sizeof(this->m_job)));
  AppendRecord(records, MakeReplayRecord(1015, &this->m_exp, 8));
  AppendRecord(records, MakeReplayRecord(1016, &this->m_level, sizeof(this->m_level)));
  AppendRecord(records, MakeReplayRecord(1017, &this->m_statusPoint, sizeof(this->m_statusPoint)));
  AppendRecord(records, MakeReplayRecord(1018, &this->m_nextExp, 8));
  AppendRecord(records, MakeReplayRecord(1019, &this->m_jobLevel, sizeof(this->m_jobLevel)));
  AppendRecord(records, MakeReplayRecord(1020, &this->m_skillPoint, 4));
  AppendRecord(records, MakeReplayRecord(1021, &this->m_guildSkillPoint, sizeof(this->m_guildSkillPoint)));
  AppendRecord(records, MakeReplayRecord(1022, &this->m_homunSkillPoint, sizeof(this->m_homunSkillPoint)));
  AppendRecord(records, MakeReplayRecord(1023, &this->m_mercenarySkillPoint, sizeof(this->m_mercenarySkillPoint)));
  AppendRecord(records, MakeReplayRecord(1024, &this->m_standardStats[0], sizeof(this->m_standardStats[0])));
  AppendRecord(records, MakeReplayRecord(1025, &this->m_standardStats[1], sizeof(this->m_standardStats[1])));
  AppendRecord(records, MakeReplayRecord(1026, &this->m_standardStats[2], sizeof(this->m_standardStats[2])));
  AppendRecord(records, MakeReplayRecord(1027, &this->m_standardStats[3], sizeof(this->m_standardStats[3])));
  AppendRecord(records, MakeReplayRecord(1028, &this->m_standardStats[4], sizeof(this->m_standardStats[4])));
  AppendRecord(records, MakeReplayRecord(1029, &this->m_standardStats[5], sizeof(this->m_standardStats[5])));
  AppendRecord(records, MakeReplayRecord(1030, &this->m_statusPointCost[0], sizeof(this->m_statusPointCost[0])));
  AppendRecord(records, MakeReplayRecord(1031, &this->m_statusPointCost[1], sizeof(this->m_statusPointCost[1])));
  AppendRecord(records, MakeReplayRecord(1032, &this->m_statusPointCost[2], sizeof(this->m_statusPointCost[2])));
  AppendRecord(records, MakeReplayRecord(1033, &this->m_statusPointCost[3], sizeof(this->m_statusPointCost[3])));
  AppendRecord(records, MakeReplayRecord(1034, &this->m_statusPointCost[4], sizeof(this->m_statusPointCost[4])));
  AppendRecord(records, MakeReplayRecord(1035, &this->m_statusPointCost[5], sizeof(this->m_statusPointCost[5])));
  AppendRecord(records, MakeReplayRecord(1036, &this->m_aspd, sizeof(this->m_aspd)));
  AppendRecord(records, MakeReplayRecord(1037, &this->m_atk1, sizeof(this->m_atk1)));
  AppendRecord(records, MakeReplayRecord(1038, &this->m_mdef1, sizeof(this->m_mdef1)));
  AppendRecord(records, MakeReplayRecord(1039, &this->m_sp36, sizeof(this->m_sp36)));
  AppendRecord(records, MakeReplayRecord(1040, &this->m_def1, sizeof(this->m_def1)));
  AppendRecord(records, MakeReplayRecord(1041, &this->m_def2, sizeof(this->m_def2)));
  AppendRecord(records, MakeReplayRecord(1042, &this->m_atk2, sizeof(this->m_atk2)));
  AppendRecord(records, MakeReplayRecord(1043, &this->m_matk1, sizeof(this->m_matk1)));
  AppendRecord(records, MakeReplayRecord(1044, &this->m_matk2, sizeof(this->m_matk2)));
  AppendRecord(records, MakeReplayRecord(1045, &this->m_mdef2, sizeof(this->m_mdef2)));
  AppendRecord(records, MakeReplayRecord(1046, &this->m_hit, sizeof(this->m_hit)));
  AppendRecord(records, MakeReplayRecord(1047, &this->m_flee1, sizeof(this->m_flee1)));
  AppendRecord(records, MakeReplayRecord(1048, &this->m_critical, sizeof(this->m_critical)));
  AppendRecord(records, MakeReplayRecord(1049, &this->m_flee2, sizeof(this->m_flee2)));
  AppendRecord(records, MakeReplayRecord(1050, &this->equippedArrowIndex, sizeof(this->equippedArrowIndex)));
  AppendRecord(records, MakeReplayRecord(1051, &this->m_zeny, sizeof(this->m_zeny)));
  AppendRecord(records, MakeReplayRecord(1052, &this->m_moveSpeed, sizeof(this->m_moveSpeed)));
  AppendRecord(records, MakeReplayRecord(1053, &this->m_manner, sizeof(this->m_manner)));
  AppendRecord(records, MakeReplayRecord(1054, &this->m_nextJobExp, 4));
  AppendRecord(records, MakeReplayRecord(1055, &this->m_jobExp, 4));
  AppendRecord(records, MakeReplayRecord(1056, &this->m_karma, sizeof(this->m_karma)));
  AppendRecord(records, MakeReplayRecord(1057, &this->monsterInfoEnabled, sizeof(this->monsterInfoEnabled)));
  AppendRecord(records, MakeReplayRecord(1058, &this->m_clientTimeOffset, sizeof(this->m_clientTimeOffset)));
  AppendRecord(records, MakeReplayRecord(1059, &this->m_characterFlags, sizeof(this->m_characterFlags)));
  AppendRecord(records, MakeReplayRecord(1060, &this->m_head, sizeof(this->m_head)));
  AppendRecord(records, MakeReplayRecord(1061, &this->m_weaponLook, sizeof(this->m_weaponLook)));
  AppendRecord(records, MakeReplayRecord(1062, &this->m_shield, sizeof(this->m_shield)));
  AppendRecord(records, MakeReplayRecord(1063, &this->m_bodyStyle, sizeof(this->m_bodyStyle)));
  AppendRecord(records, MakeReplayRecord(1064, &this->m_headPalette, sizeof(this->m_headPalette)));
  AppendRecord(records, MakeReplayRecord(1065, &this->m_accessory, sizeof(this->m_accessory)));
  AppendRecord(records, MakeReplayRecord(1066, &this->m_accessory2, sizeof(this->m_accessory2)));
  AppendRecord(records, MakeReplayRecord(1067, &this->m_accessory3, sizeof(this->m_accessory3)));
  AppendRecord(records, MakeReplayRecord(1068, &this->m_bodyState, sizeof(this->m_bodyState)));
  AppendRecord(records, MakeReplayRecord(1069, &this->m_effectState, sizeof(this->m_effectState)));
  AppendRecord(records, MakeReplayRecord(1070, &this->m_jobFlags, sizeof(this->m_jobFlags)));
  AppendRecord(records, MakeReplayRecord(1071, &this->m_robe, sizeof(this->m_robe)));
  AppendRecord(records, MakeReplayRecord(1072, &this->m_equipViewLook[0], 4));
  AppendRecord(records, MakeReplayRecord(1073, &this->m_equipViewLook[1], 4));
  AppendRecord(records, MakeReplayRecord(1074, &this->m_equipViewLook[2], 4));
  AppendRecord(records, MakeReplayRecord(1075, &this->m_equipViewLook[3], 4));
  AppendRecord(records, MakeReplayRecord(1076, &this->m_equipViewLook[4], 4));
  AppendRecord(records, MakeReplayRecord(1077, &this->m_equipViewLook[5], 4));
  AppendRecord(records, MakeReplayRecord(1078, &this->m_equipViewLook[6], 4));
  AppendRecord(records, MakeReplayRecord(1079, &this->m_equipViewLook[7], 4));
  AppendRecord(records, MakeReplayRecord(1080, &this->m_equipViewLook[8], 4));
  AppendRecord(records, MakeReplayRecord(1081, this->m_equipViewName, 0x18));
  AppendRecord(records, MakeReplayRecord(1082, this->playerTeam, 1));
  AppendRecord(records, MakeReplayRecord(1083, &this->m_score1, sizeof(this->m_score1)));
  AppendRecord(records, MakeReplayRecord(1084, &this->m_score2, sizeof(this->m_score2)));
  AppendRecord(records, MakeReplayRecord(1085, &this->chatFont, 1));
  AppendRecord(records, MakeReplayRecord(1086, &this->cartCount, sizeof(this->cartCount)));
  AppendRecord(records, MakeReplayRecord(1087, &this->cartMaxCount, sizeof(this->cartMaxCount)));
  AppendRecord(records, MakeReplayRecord(1088, &this->cartWeight, sizeof(this->cartWeight)));
  AppendRecord(records, MakeReplayRecord(1089, &this->cartMaxWeight, sizeof(this->cartMaxWeight)));
  AppendRecord(records, MakeReplayRecord(1090, &this->m_vendingSlotCount, 4));
  AppendRecord(records, MakeReplayRecord(1091, &this->m_buyingStoreSlotCount, 4));
  AppendRecord(records, MakeReplayRecord(1092, &this->m_chatTarget, sizeof(this->m_chatTarget)));
  AppendRecord(records, MakeReplayRecord(1093, &this->unknown_549C, sizeof(this->unknown_549C)));
  AppendRecord(records, MakeReplayRecord(1094, &this->partyExpShare, sizeof(this->partyExpShare)));
  AppendRecord(records, MakeReplayRecord(1095, &this->partyItemCollect, sizeof(this->partyItemCollect)));
  AppendRecord(records, MakeReplayRecord(1096, &this->partyItemDistribute, sizeof(this->partyItemDistribute)));
  AppendRecord(records, BuildReplayTaggedRecord(1097, this->storeTitle.c_str(), this->storeTitle.size()));
  const unsigned char option = g_serviceOptions.Get(0x40);
  AppendRecord(records, MakeReplayRecord(1098, &option, sizeof(option)));
  AppendRecord(records, MakeReplayRecord(1099, &this->adoptionSkillAvailable, sizeof(this->adoptionSkillAvailable)));
  AppendRecord(records, MakeReplayRecord(1100, &this->m_bodyCostume, sizeof(this->m_bodyCostume)));
  AppendRecord(records, MakeReplayRecord(1101, &this->extraInventorySlots, sizeof(this->extraInventorySlots)));
  AppendRecord(records, MakeReplayRecord(1001, nullptr, 0));
  return records;
}

// OFFSET: 0x00E41E60
// FUNCTION: fn_24201
std::vector<unsigned char> CSession::BuildShopReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(7500, nullptr, 0));
  for (auto it = this->m_nameToId.begin(); it != this->m_nameToId.end(); ++it) {
    ReplayShopNameRecord value;
    value.padding_0000 = 0;
    value.shopId = it->second;
    std::memset(value.name, 0, sizeof(value.name));
    const std::size_t length = it->first.size();
    std::memcpy(value.name, it->first.c_str(), length > sizeof(value.name) ? sizeof(value.name) : length);
    AppendRecord(records, MakeReplayRecord(7502, &value, sizeof(value)));
  }
  for (auto shop = this->m_shops.begin(); shop != this->m_shops.end(); ++shop) {
    for (unsigned int index = 0; index < shop->second.items.size(); ++index) {
      const std::map<int, std::pair<int, int>>& items = shop->second.items[index];
      for (auto item = items.begin(); item != items.end(); ++item) {
        ReplayShopItemRecord value;
        value.shopId = shop->first;
        value.index = index;
        value.itemId = item->first;
        value.first = item->second.first;
        value.second = item->second.second;
        AppendRecord(records, MakeReplayRecord(7504, &value, sizeof(value)));
      }
    }
    ReplayShopFlagRecord value;
    value.shopId = shop->first;
    value.flag = shop->second.flag;
    AppendRecord(records, MakeReplayRecord(7505, &value, sizeof(value)));
  }
  AppendRecord(records, MakeReplayRecord(7501, nullptr, 0));
  return records;
}

// OFFSET: 0x00E42600
// FUNCTION: fn_24202
std::vector<unsigned char> CSession::BuildExtraReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(5500, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(5510, &this->memorialWindowOpen, sizeof(this->memorialWindowOpen)));
  AppendRecord(records, MakeReplayRecord(5511, &this->memorialWindowX, sizeof(this->memorialWindowX)));
  AppendRecord(records, MakeReplayRecord(5512, &this->memorialWindowY, sizeof(this->memorialWindowY)));
  AppendRecord(records, MakeReplayRecord(5513, &this->memorialDungeonState, sizeof(this->memorialDungeonState)));
  AppendRecord(records, MakeReplayRecord(5514, &this->memorialReplayValue, sizeof(this->memorialReplayValue)));
  AppendRecord(records, BuildReplayTaggedRecord(5515, this->memorialDungeonName.c_str(), this->memorialDungeonName.size()));
  AppendRecord(records, MakeReplayRecord(5516, &this->memorialQueuePosition, sizeof(this->memorialQueuePosition)));
  AppendRecord(records, MakeReplayRecord(5517, &this->memorialCreationTime, sizeof(this->memorialCreationTime)));
  AppendRecord(records, MakeReplayRecord(5518, &this->memorialExpirationTime, sizeof(this->memorialExpirationTime)));
  AppendRecord(records, MakeReplayRecord(5501, nullptr, 0));
  return records;
}

// OFFSET: 0x00E3FF30
// FUNCTION: fn_24198
std::vector<unsigned char> CSession::BuildStatsReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(2000, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(2010, &this->strengthBonus, sizeof(this->strengthBonus)));
  AppendRecord(records, MakeReplayRecord(2011, &this->agilityBonus, sizeof(this->agilityBonus)));
  AppendRecord(records, MakeReplayRecord(2012, &this->vitalityBonus, sizeof(this->vitalityBonus)));
  AppendRecord(records, MakeReplayRecord(2013, &this->intelligenceBonus, sizeof(this->intelligenceBonus)));
  AppendRecord(records, MakeReplayRecord(2014, &this->dexterityBonus, sizeof(this->dexterityBonus)));
  AppendRecord(records, MakeReplayRecord(2015, &this->luckBonus, sizeof(this->luckBonus)));
  AppendRecord(records, BuildReplayTaggedRecord(2016, &this->storeItemLimit, sizeof(this->storeItemLimit)));
  AppendRecord(records, BuildReplayTaggedRecord(2017, &this->m_weight, sizeof(this->m_weight)));
  AppendRecord(records, BuildReplayTaggedRecord(2018, &this->m_maxWeight, sizeof(this->m_maxWeight)));
  AppendRecord(records, BuildReplayTaggedRecord(2019, &this->randomStatIndexA, sizeof(this->randomStatIndexA)));
  AppendRecord(records, BuildReplayTaggedRecord(2020, &this->randomStatIndexB, sizeof(this->randomStatIndexB)));
  AppendRecord(records, BuildReplayTaggedRecord(2021, &this->randomStatIndexC, sizeof(this->randomStatIndexC)));
  AppendRecord(records, BuildReplayTaggedRecord(2022, &this->randomStatIndexD, sizeof(this->randomStatIndexD)));
  AppendRecord(records, BuildReplayTaggedRecord(2023, &this->randomStatIndexE, sizeof(this->randomStatIndexE)));
  AppendRecord(records, BuildReplayTaggedRecord(2024, this->randomStatValues, sizeof(this->randomStatValues)));
  AppendRecord(records, BuildReplayTaggedRecord(2025, this->replayStatValuesA, sizeof(this->replayStatValuesA)));
  AppendRecord(records, BuildReplayTaggedRecord(2026, this->replayStatValuesB, sizeof(this->replayStatValuesB)));
  AppendRecord(records, BuildReplayTaggedRecord(2027, this->replayStatValuesC, sizeof(this->replayStatValuesC)));
  AppendRecord(records, BuildReplayTaggedRecord(2028, this->replayStatValuesD, 0x30));
  AppendRecord(records, BuildReplayTaggedRecord(2029, &this->hp, sizeof(this->hp)));
  AppendRecord(records, BuildReplayTaggedRecord(2030, &this->maxHp, sizeof(this->maxHp)));
  AppendRecord(records, BuildReplayTaggedRecord(2031, &this->m_sp, sizeof(this->m_sp)));
  AppendRecord(records, BuildReplayTaggedRecord(2032, &this->m_maxSp, sizeof(this->m_maxSp)));
  AppendRecord(records, BuildReplayTaggedRecord(2033, &this->m_prohibitionTarget, sizeof(this->m_prohibitionTarget)));
  AppendRecord(records, BuildReplayTaggedRecord(2034, &this->quitBlocked, sizeof(this->quitBlocked)));
  AppendRecord(records, BuildReplayTaggedRecord(2035, &this->battleChat, sizeof(this->battleChat)));
  AppendRecord(records, BuildReplayTaggedRecord(2036, &this->quitAllowed, sizeof(this->quitAllowed)));
  AppendRecord(records, BuildReplayTaggedRecord(2037, &this->sessionFlag, sizeof(this->sessionFlag)));
  AppendRecord(records, BuildReplayTaggedRecord(2038, this->m_accountName.c_str(), this->m_accountName.size()));
  AppendRecord(records, BuildReplayTaggedRecord(2052, &this->madogearType, sizeof(this->madogearType)));
  AppendRecord(records, MakeReplayRecord(2001, nullptr, 0));
  return records;
}

// OFFSET: 0x00E40C60
// FUNCTION: fn_24199
std::vector<unsigned char> CSession::BuildMessengerReplayRecords() {
  std::vector<unsigned char> records;
  AppendRecord(records, MakeReplayRecord(4000, nullptr, 0));
  AppendRecord(records, MakeReplayRecord(4010, &this->replaySessionFlag, 1));
  AppendRecord(records, MakeReplayRecord(4011, &this->partyBlocked, sizeof(this->partyBlocked)));
  AppendRecord(records, MakeReplayRecord(4012, &this->partyLeader, sizeof(this->partyLeader)));
  AppendRecord(records, BuildReplayTaggedRecord(4013, this->partyRequestName.c_str(), this->partyRequestName.size()));
  AppendRecord(records, BuildReplayTaggedRecord(4014, this->partyName.c_str(), this->partyName.size()));
  AppendRecord(records, MakeFramedMessage(4015, SerializeMessengerMembers(this->m_partyMembers)));
  AppendRecord(records, MakeFramedMessage(4016, SerializeMessengerMembers(this->m_friends)));
  AppendRecord(records, MakeReplayRecord(4001, nullptr, 0));
  return records;
}
