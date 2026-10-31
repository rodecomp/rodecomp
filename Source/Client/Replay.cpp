#include "Client/Replay.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Skills/SkillInfo.h"
#include "Client/Session.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <windows.h>
#include <timeapi.h>
#include "Contents/EntryQueueMgr.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Session.h"
#include "Contents/RenewQuestMgr.h"

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
#pragma pack(pop)

static_assert(sizeof(ReplayTaggedRecordHeader) == 6);

#pragma pack(push, 1)
struct ReplayRecordHeader {
  /* 0000 */ std::uint32_t frame;
  /* 0004 */ std::uint32_t tick;
  /* 0008 */ std::uint16_t bytes;
};
#pragma pack(pop)

static_assert(sizeof(ReplayRecordHeader) == 10);

// OFFSET: 0x006DA340
// FUNCTION: fn_06623
std::vector<unsigned char> BuildReplayTaggedRecord(std::uint16_t id, const void* data, std::uint32_t bytes) {
  std::vector<unsigned char> record(bytes + 6u, 0);
  const ReplayTaggedRecordHeader header{id, bytes};
  std::memcpy(record.data(), &header, sizeof(header));
  if (bytes != 0) {
    std::memcpy(record.data() + sizeof(header), data, bytes);
  }
  return record;
}

// OFFSET: 0x00E3DBA0
// FUNCTION: fn_24195
std::vector<unsigned char> BuildEntryQueueReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(6500, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  if (g_entryQueueMgr->current != nullptr) {
    const int id = g_entryQueueMgr->current->id;
    const auto record = BuildReplayTaggedRecord(6511, &id, sizeof(id));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const unsigned char ready = g_entryQueueMgr->ready;
    const auto record = BuildReplayTaggedRecord(6512, &ready, sizeof(ready));
    records.insert(records.end(), record.begin(), record.end());
  }
  std::string text = g_entryQueueMgr->queueName.text;
  if (text.size() != 0) {
    const auto record = BuildReplayTaggedRecord(6513, text.c_str(), text.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  text = g_entryQueueMgr->characterName.text;
  if (text.size() != 0) {
    const auto record = BuildReplayTaggedRecord(6514, text.c_str(), text.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  if (UIEntryQueueWnd* window = dynamic_cast<UIEntryQueueWnd*>(g_windowMgr.LookupWindow(157))) {
    const int selected = window->SelectedIndex();
    const auto record = BuildReplayTaggedRecord(6515, &selected, sizeof(selected));
    records.insert(records.end(), record.begin(), record.end());
  }
  if (UIEntryQueueStandByWnd* window = dynamic_cast<UIEntryQueueStandByWnd*>(g_windowMgr.LookupWindow(209))) {
    {
      const auto record = BuildReplayTaggedRecord(6517, &window->m_waiting, sizeof(window->m_waiting));
      records.insert(records.end(), record.begin(), record.end());
    }
    {
      const auto record = BuildReplayTaggedRecord(6518, &window->m_ranking, sizeof(window->m_ranking));
      records.insert(records.end(), record.begin(), record.end());
    }
    const auto record = BuildReplayTaggedRecord(6516, window->m_queueName.c_str(), window->m_queueName.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(6501, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
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
void ReplaySection::Clear() {
  this->id = 0;
  this->bytes = 0;
  this->fileOffset = 0;
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
Replay::Replay() : cipherIndex(0), cipherBase(0), cipherKey(0), unknown_01F4(0), unknown_01F8(0) {
  this->state = 1;
  this->baseFrame = 0;
  this->started = 0;
  this->frame = 0;
  this->elapsed = 0;
  this->speed = 1.0f;
  std::memset(&this->header, 0, sizeof(this->header));
  std::memset(&this->header.options, 1, sizeof(this->header.options));
  this->ResetFile();
  this->previousTick = timeGetTime();
  this->unknown_01F8 = 0;
  this->unknown_01F4 = 0;
  this->cachedPackets.resize(3143, ReplayCachedPacket{});
  this->cachedPackets[2459].enabled = true;
  this->cachedPackets[470].enabled = true;
}

// OFFSET: 0x00B855B0
// FUNCTION: fn_19326
bool Replay::WriteSection(int id, std::vector<unsigned char>& bytes, int event) {
  if (bytes.empty()) {
    return false;
  }
  if (id <= 0 || id >= 26 || bytes.size() == 0 || bytes.data() == nullptr) {
    return false;
  }
  ReplaySection& section = this->sections[id];
  if (event == 14) {
    section.id = static_cast<std::uint16_t>(id);
    section.bytes = bytes.size();
    section.fileOffset = this->fileOffset;
    this->fileOffset += bytes.size();
  }
  if (section.bytes != 0) {
    this->cipherBase = this->header.key0 >> 5;
    this->cipherIndex = 0;
    this->cipherKey = this->header.key1 >> 3;
    for (std::uint32_t index = 0; index < section.bytes / 4; ++index) {
      std::uint32_t word;
      std::memcpy(&word, bytes.data() + index * 4, sizeof(word));
      word ^= this->cipherKey * (++this->cipherIndex + this->cipherBase);
      std::memcpy(bytes.data() + index * 4, &word, sizeof(word));
    }
  }
  std::fseek(this->file, static_cast<long>(section.fileOffset), SEEK_SET);
  return std::fwrite(bytes.data(), section.bytes, 1, this->file) != 0;
}

// OFFSET: 0x00B857E0
// FUNCTION: fn_19327
int Replay::WriteHeader(int event) {
  this->header.records = this->records;
  this->header.fileOffset = this->fileOffset;
  this->header.closingSignature = 0x52414705;
  this->header.signature = 0x52414705;
  switch (event) {
  case 11: {
    this->sections[25].id = 111;
    bool written;
    {
      std::vector<unsigned char> metadata = this->BuildMetadata();
      written = this->WriteSection(2, metadata, 14);
    }
    if (!written) {
      return 0;
    }
    char text[100] = {};
    std::sprintf(text, g_replayHeaderText, 5);
    std::sprintf(text + 95, g_replayHeaderPrefix);
    std::fseek(this->file, 0, SEEK_SET);
    std::fwrite(text, sizeof(text), 1, this->file);
    std::fwrite(&this->header, 12, 1, this->file);
    return 1;
  }
  case 12: {
    if (++g_replayFlushCounter != 1200) {
      return 1;
    }
    this->header.closingSignature = 0x51404605;
    this->header.signature = 0x51404605;
    bool written;
    {
      std::vector<unsigned char> metadata = this->BuildMetadata();
      written = this->WriteSection(2, metadata, 15);
    }
    if (!written) {
      return 0;
    }
    std::fseek(this->file, 112, SEEK_SET);
    std::fwrite(this->sections, sizeof(this->sections), 1, this->file);
    g_replayFlushCounter = 0;
    return 1;
  }
  case 13: {
    bool written;
    {
      std::vector<unsigned char> metadata = this->BuildMetadata();
      written = this->WriteSection(2, metadata, 15);
    }
    if (!written) {
      return 0;
    }
    std::fseek(this->file, 112, SEEK_SET);
    std::fwrite(this->sections, sizeof(this->sections), 1, this->file);
    return 1;
  }
  default:
    return 0;
  }
}

// OFFSET: 0x00B85A20
// FUNCTION: fn_19328
std::vector<unsigned char> Replay::BuildMetadata() {
  std::vector<unsigned char> metadata;
  {
    const auto record = BuildReplayTaggedRecord(950, nullptr, 0);
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(961, &this->header.service, sizeof(this->header.service));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(962, &this->header.serviceType, sizeof(this->header.serviceType));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(963, &this->header.unknown_0014, sizeof(this->header.unknown_0014));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(964, this->header.characterName, sizeof(this->header.characterName));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(965, this->header.mapName, sizeof(this->header.mapName));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(966, &this->header.accountId, sizeof(this->header.accountId));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(967, &this->header.cellX, sizeof(this->header.cellX));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(968, &this->header.cellY, sizeof(this->header.cellY));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(969, &this->header.unknown_0078, sizeof(this->header.unknown_0078));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(970, &this->header.duration, sizeof(this->header.duration));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(971, &this->header.records, sizeof(this->header.records));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(972, &this->header.fileOffset, sizeof(this->header.fileOffset));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(973, &this->header.options, sizeof(this->header.options));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(960, &this->header.closingSignature, sizeof(this->header.closingSignature));
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(951, nullptr, 0);
    metadata.insert(metadata.end(), record.begin(), record.end());
  }
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
  if (mode == 2) {
    const int type = this->header.serviceType;
    return type == 0 || type == 13 || type == 1 || type == 2;
  }

  return g_serviceMode == this->header.serviceType;
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
  this->cipherIndex = 0;
  this->cipherKey = this->header.key1 >> 3;
  const int count = bytes / 4;

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
ReplayTime Replay::Duration() const {
  const unsigned seconds = static_cast<int>(this->header.duration) / 1000;
  const unsigned minutes = seconds / 60;
  const unsigned hours = minutes / 60;
  ReplayTime time;
  time.hours = static_cast<std::uint16_t>(hours);
  time.minutes = static_cast<char>(minutes % 60);
  time.seconds = static_cast<char>(seconds % 60);
  return time;
}

// OFFSET: 0x00B843A0
// FUNCTION: fn_19315
int Replay::FindSection(int id) const {
  if (id < 1 || id > 25) {
    return -1;
  }

  if (this->sections[id].id == id) {
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
  if (this->speed == 0.25f) {
    return 1;
  }
  if (this->speed == 0.5f) {
    return 2;
  }
  if (this->speed == 1.0f) {
    return 3;
  }
  if (this->speed == 2.0f) {
    return 4;
  }
  if (this->speed == 4.0f) {
    return 5;
  }
  if (this->speed == 8.0f) {
    return 6;
  }
  if (this->speed == 16.0f) {
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

  if (id >= 1 && id <= 25) {
    const int index = this->FindSection(id);

    if (index != -1 && this->sections[index].fileOffset != 0 && this->sections[index].bytes != 0) {
      const ReplaySection& section = this->sections[index];
      data.resize(section.bytes);
      std::fseek(this->file, static_cast<long>(section.fileOffset), SEEK_SET);
      std::fread(data.data(), section.bytes, 1, this->file);
      this->Cipher(section.bytes, data.data());
    }
  }

  return data;
}

// OFFSET: 0x00B84E40
// FUNCTION: fn_19322
std::uint32_t Replay::ParseMetadata(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return 0xFFFFFFFF;
  }

  const unsigned char* buffer = data.data();
  const std::uint32_t size = static_cast<std::uint32_t>(data.size());
  std::uint32_t offset = 6;

  std::uint16_t signature = 0;
  std::memcpy(&signature, buffer, sizeof(signature));

  if (signature != 0x3B6) {
    return 0xFFFFFFFF;
  }

  if (size > 6) {
    do {
      std::uint16_t id = 0;
      std::uint32_t bytes = 0;
      std::memcpy(&id, buffer + offset, sizeof(id));
      std::memcpy(&bytes, buffer + offset + 2, sizeof(bytes));
      offset += 6;

      switch (id) {
      case 0x3B7:
        return offset;
      case 0x3C0:
        std::memcpy(&this->header.closingSignature, buffer + offset, bytes);
        offset += bytes;

        if (this->header.signature != this->header.closingSignature) {
          std::uint32_t fileOffset = this->fileOffset;
          ReplayRecordHeader record{};
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
      case 0x3C1:
        std::memcpy(&this->header.service, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3C2:
        std::memcpy(&this->header.serviceType, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3C3:
        std::memcpy(&this->header.unknown_0014, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3C4:
        std::memcpy(this->header.characterName, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3C5:
        std::memcpy(this->header.mapName, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3C6:
        std::memcpy(&this->header.accountId, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3C7:
        std::memcpy(&this->header.cellX, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3C8:
        std::memcpy(&this->header.cellY, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3C9:
        std::memcpy(&this->header.unknown_0078, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3CA:
        std::memcpy(&this->header.duration, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3CB:
        std::memcpy(&this->header.records, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3CC:
        std::memcpy(&this->header.fileOffset, buffer + offset, bytes);
        offset += bytes;
        break;
      case 0x3CD:
        std::memcpy(&this->header.options, buffer + offset, bytes);
        offset += bytes;
        break;
      default:
        offset += bytes;
        break;
      }
    } while (offset < size);
  }

  return offset;
}

// OFFSET: 0x00B85350
// FUNCTION: fn_19323
bool Replay::ReadRecord(unsigned char* buffer, std::uint16_t& id, bool force) {
  if (!force && this->state != 7) {
    return false;
  }

  ReplayRecordHeader record{};
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
void ReplayFileList::Clear() {
  this->selected = -1;
  this->files.erase(this->files.begin(), this->files.end());
}

// OFFSET: 0x00B84B60
// FUNCTION: fn_19321
void ReplayFileList::Refresh() {
  this->Clear();

  char directory[260] = {};
  GetCurrentDirectoryA(sizeof(directory), directory);

  std::string pattern(directory, std::strlen(directory));
  pattern.append("\\Replay\\*.rrf", 13);

  WIN32_FIND_DATAA found;
  HANDLE search = FindFirstFileA(pattern.c_str(), &found);

  if (search != INVALID_HANDLE_VALUE) {
    BOOL more = TRUE;

    while (more) {
      if ((found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        const std::size_t length = std::strlen(found.cFileName);
        const char* extension = found.cFileName + length - 4;

        if (extension[0] == '.' && extension[1] == 'r' && extension[2] == 'r' && extension[3] == 'f') {
          ReplayFileEntry entry = {};
          std::memcpy(entry.name, found.cFileName, length);
          entry.name[length] = '\0';
          entry.unknown_0000 = 0;

          if (length == 23) {
            char number[6] = {};
            std::memcpy(number, entry.name + 15, 4);
            entry.unknown_0000 = std::atoi(number);
          }

          this->files.push_back(entry);
        }
      }

      more = FindNextFileA(search, &found);
    }
  }

  FindClose(search);
}

// OFFSET: 0x00B84300
// FUNCTION: fn_19313
const char* ReplayFileList::FileName(int index) const {
  if (static_cast<unsigned>(index) >= this->files.size()) {
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

  if (index >= count) {
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

    this->replay->fileMode = 0x20;
    this->replay->file = std::fopen(this->path, "rb");

    if (this->replay->file) {
      std::fseek(this->replay->file, 100, SEEK_SET);
      std::memset(&this->replay->header, 0, 0xC);
      std::fread(&this->replay->header, 0xC, 1, this->replay->file);

      if (this->replay->header.signature == 0x52414705u || this->replay->header.signature == 0x51404605u) {
        for (int index = 0; index <= 0x1A; ++index) {
          ReplaySection section = {};
          std::fread(&section, sizeof(section), 1, this->replay->file);

          if (section.id > 0x1A) {
            break;
          }

          this->replay->sections[section.id] = section;
        }

        if (this->replay->ParseMetadata(this->replay->ReadSection(2)) != 0xFFFFFFFFu) {
          return 1;
        }

        std::memset(&this->replay->header, 0, sizeof(this->replay->header));
      }

      if (this->replay->file) {
        std::fclose(this->replay->file);
        this->replay->file = nullptr;
      }

      std::memset(&this->replay->header, 0, sizeof(this->replay->header));
      std::memset(this->replay->sections, 0, sizeof(this->replay->sections));
    }

    return 0;
  }

  this->Refresh();

  if (!this->pathEnabled) {
    int index = 1;

    for (std::size_t i = 0; i < this->files.size(); ++i) {
      if (this->files[i].unknown_0000 == index) {
        ++index;
      }
    }

    std::memset(this->path, 0, sizeof(this->path));
    std::sprintf(this->path, g_replayNewFileFormat, index);
  } else {
    if (this->unknown_0119) {
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
  std::uint32_t key0, std::uint32_t key1, const char* characterName, const char* mapName, int accountId, int cellX, int cellY, int unknown0078, int service,
  int serviceType, int unknown0014
) {
  if (!characterName || !mapName) {
    return false;
  }
  std::memcpy(this->header.mapName, mapName, sizeof(this->header.mapName));
  std::memcpy(this->header.characterName, characterName, sizeof(this->header.characterName));
  this->header.service = service;
  this->header.serviceType = serviceType;
  this->header.unknown_0014 = unknown0014;
  this->header.key0 = key0;
  this->header.key1 = key1;
  this->header.accountId = accountId;
  this->header.cellX = cellX;
  this->header.cellY = cellY;
  this->header.unknown_0078 = unknown0078;
  this->header.signature = 0x52414705;
  this->header.closingSignature = 0x52414705;
  return true;
}

// OFFSET: 0x00B871C0
// FUNCTION: fn_19340
void Replay::IncreaseSpeed() {
  if (this->speed == 0.25f) {
    this->speed = 0.5f;
  } else if (this->speed == 0.5f) {
    this->speed = 1.0f;
  } else if (this->speed == 1.0f) {
    this->speed = 2.0f;
  } else if (this->speed == 2.0f || this->speed == 4.0f) {
    this->speed = 4.0f;
  }
}

// OFFSET: 0x00B87240
// FUNCTION: fn_19341
void Replay::DecreaseSpeed() {
  if (this->speed == 0.25f || this->speed == 0.5f) {
    this->speed = 0.25f;
  } else if (this->speed == 1.0f) {
    this->speed = 0.5f;
  } else if (this->speed == 2.0f) {
    this->speed = 1.0f;
  } else if (this->speed == 4.0f) {
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
  *reinterpret_cast<std::uint32_t*>(data + 17) = this->unknown_5C1C;
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
    *reinterpret_cast<std::uint32_t*>(record + 2) = skill.unknown_000C;
    *reinterpret_cast<std::uint16_t*>(record + 6) = static_cast<std::uint16_t>(skill.level);
    *reinterpret_cast<std::uint16_t*>(record + 8) = static_cast<std::uint16_t>(skill.unknown_0014);
    record[36] = skill.unknown_0018 != 0;
    *reinterpret_cast<std::uint16_t*>(record + 10) = static_cast<std::uint16_t>(skill.unknown_001C);
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
  {
    const auto record = BuildReplayTaggedRecord(3500, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const int version = 51;
    const auto record = BuildReplayTaggedRecord(3502, &version, sizeof(version));
    records.insert(records.end(), record.begin(), record.end());
  }
  const int size = g_renewQuestMgr->GetOngoingQuestsSize();
  char* buffer = new char[size];
  g_renewQuestMgr->WriteOngoingQuests(buffer);
  {
    const auto record = BuildReplayTaggedRecord(3510, buffer, size);
    records.insert(records.end(), record.begin(), record.end());
  }
  delete buffer;
  {
    const auto record = BuildReplayTaggedRecord(3501, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x00E449C0
// FUNCTION: fn_24206
std::vector<unsigned char> CSession::BuildShortcutReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(7000, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(7100, this->m_shortcutSlots, sizeof(this->m_shortcutSlots));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(7101, &this->unknown_06EB, sizeof(this->unknown_06EB) + sizeof(this->unknown_06F2));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(7001, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x00E44DF0
// FUNCTION: fn_24207
std::vector<unsigned char> CSession::BuildSkillListReplayPacket(short id) {
  switch (id) {
  case 0x10F:
    return BuildSkillInfoListPacket(0x10F, this->m_skillBundle.skills);
  case 0x235:
    return BuildSkillInfoListPacket(0x235, this->m_homunSkills);
  case 0x29D:
    return BuildSkillInfoListPacket(0x29D, this->m_mercSkills);
  default:
    return {};
  }
}

// OFFSET: 0x00B86440
// FUNCTION: fn_19329
void Replay::CachePacket(std::uint16_t id, std::uint16_t size, const unsigned char* data) {
  if (size == 0 || id > 3142 || data == nullptr) {
    return;
  }
  ReplayCachedPacket& packet = this->cachedPackets[static_cast<short>(id)];
  if (!packet.enabled) {
    return;
  }
  packet.bytes.resize(size, 0);
  packet.bytes.assign(data, data + size);
}

// OFFSET: 0x00E42EB0
