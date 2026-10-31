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
// FUNCTION: fn_24203
std::vector<unsigned char> CSession::BuildReservedReplayRecords() {
  std::vector<unsigned char> records;
  return records;
}

// OFFSET: 0x00E41290
// FUNCTION: fn_24200
std::vector<unsigned char> CSession::BuildItemReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(4500, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4701, &this->unknown_5210, sizeof(this->unknown_5210));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4702, &this->unknown_5214, sizeof(this->unknown_5214));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->m_inventoryItems);
    const auto record = MakeFramedMessage(4510, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1718);
    const auto record = MakeFramedMessage(4511, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1720);
    const auto record = MakeFramedMessage(4512, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1728);
    const auto record = MakeFramedMessage(4513, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1730);
    const auto record = MakeFramedMessage(4514, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1738);
    const auto record = MakeFramedMessage(4515, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1740);
    const auto record = MakeFramedMessage(4516, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1748);
    const auto record = MakeFramedMessage(4517, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1750);
    const auto record = MakeFramedMessage(4518, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1758);
    const auto record = MakeFramedMessage(4519, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1760);
    const auto record = MakeFramedMessage(4520, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1768);
    const auto record = MakeFramedMessage(4521, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeItems(this->unknown_1770);
    const auto record = MakeFramedMessage(4522, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const std::list<ITEM_INFO> items(std::begin(this->unknown_17F0), std::end(this->unknown_17F0));
    const auto bytes = SerializeItems(items);
    const auto record = MakeFramedMessage(4601, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const std::list<ITEM_INFO> items(std::begin(this->unknown_21A0), std::end(this->unknown_21A0));
    const auto bytes = SerializeItems(items);
    const auto record = MakeFramedMessage(4602, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const std::list<ITEM_INFO> items(std::begin(this->unknown_2B50), std::end(this->unknown_2B50));
    const auto bytes = SerializeItems(items);
    const auto record = MakeFramedMessage(4603, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const std::list<ITEM_INFO> items(std::begin(this->unknown_3500), std::end(this->unknown_3500));
    const auto bytes = SerializeItems(items);
    const auto record = MakeFramedMessage(4604, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const std::list<ITEM_INFO> items(std::begin(this->m_exchangeItems), std::end(this->m_exchangeItems));
    const auto bytes = SerializeItems(items);
    const auto record = MakeFramedMessage(4605, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const std::list<ITEM_INFO> items(std::begin(this->m_itemSlots), std::end(this->m_itemSlots));
    const auto bytes = SerializeItems(items);
    const auto record = MakeFramedMessage(4606, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4501, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x00E42F20
// FUNCTION: fn_24204
std::vector<unsigned char> CSession::BuildCompanionReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(5000, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5101, &this->unknown_5570[0x08], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5102, &this->unknown_5570[0x0C], 32);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5103, &this->unknown_5570[0x2C], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5104, &this->unknown_5570[0x30], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5105, &this->unknown_5570[0x34], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5106, &this->unknown_5570[0x38], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5107, &this->unknown_5570[0x3C], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5108, &this->unknown_5570[0x40], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5109, &this->unknown_5570[0x44], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5110, &this->unknown_5570[0x48], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5111, &this->m_homunJob, sizeof(this->m_homunJob));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5112, this->unknown_55C0, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5113, &this->m_homunHp, sizeof(this->m_homunHp));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5114, this->unknown_55C8, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5115, &this->m_homunSp, sizeof(this->m_homunSp));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5116, &this->unknown_55D0, sizeof(this->unknown_55D0));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5117, &this->unknown_55D4, sizeof(this->unknown_55D4));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5118, &this->unknown_55D8, sizeof(this->unknown_55D8));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5119, &this->m_homunExp, sizeof(this->m_homunExp));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5120, &this->m_homunMaxExp, sizeof(this->m_homunMaxExp));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5121, &this->unknown_55F0, sizeof(this->unknown_55F0));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5122, &this->unknown_55F4, sizeof(this->unknown_55F4));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5123, &this->unknown_55F8, sizeof(this->unknown_55F8));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5124, &this->unknown_55FC, sizeof(this->unknown_55FC));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5125, &this->unknown_5600, sizeof(this->unknown_5600));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5126, &this->unknown_5604, sizeof(this->unknown_5604));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5127, &this->unknown_5608, sizeof(this->unknown_5608));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5128, &this->unknown_560C, sizeof(this->unknown_560C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5129, &this->unknown_5610, sizeof(this->unknown_5610));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5130, &this->unknown_5614, sizeof(this->unknown_5614));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5131, &this->m_traceAi, sizeof(this->m_traceAi));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5132, &this->m_homunActionState, sizeof(this->m_homunActionState));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5133, &this->unknown_5620[0x0], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5134, &this->unknown_5620[0x4], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5201, &this->unknown_5620[0x8], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5202, this->m_mercenaryName, sizeof(this->m_mercenaryName));
    records.insert(records.end(), record.begin(), record.end());
  }
  AppendBytes(records, BuildReplayTaggedRecord(5203, &this->m_mercenaryStats[0], sizeof(this->m_mercenaryStats[0])));
  AppendBytes(records, BuildReplayTaggedRecord(5204, &this->m_mercenaryStats[1], sizeof(this->m_mercenaryStats[1])));
  AppendBytes(records, BuildReplayTaggedRecord(5205, &this->m_mercenaryStats[2], sizeof(this->m_mercenaryStats[2])));
  AppendBytes(records, BuildReplayTaggedRecord(5206, &this->m_mercenaryStats[3], sizeof(this->m_mercenaryStats[3])));
  AppendBytes(records, BuildReplayTaggedRecord(5207, &this->m_mercenaryStats[4], sizeof(this->m_mercenaryStats[4])));
  AppendBytes(records, BuildReplayTaggedRecord(5208, &this->m_mercenaryStats[5], sizeof(this->m_mercenaryStats[5])));
  AppendBytes(records, BuildReplayTaggedRecord(5209, &this->m_mercenaryStats[6], sizeof(this->m_mercenaryStats[6])));
  AppendBytes(records, BuildReplayTaggedRecord(5210, &this->m_mercenaryStats[7], sizeof(this->m_mercenaryStats[7])));
  AppendBytes(records, BuildReplayTaggedRecord(5211, &this->m_mercenaryHp, sizeof(this->m_mercenaryHp)));
  AppendBytes(records, BuildReplayTaggedRecord(5212, &this->m_mercenaryMaxHp, sizeof(this->m_mercenaryMaxHp)));
  AppendBytes(records, BuildReplayTaggedRecord(5213, &this->m_mercenaryClass, sizeof(this->m_mercenaryClass)));
  AppendBytes(records, BuildReplayTaggedRecord(5214, &this->m_mercenaryLevel, sizeof(this->m_mercenaryLevel)));
  AppendBytes(records, BuildReplayTaggedRecord(5215, &this->m_mercenarySp, sizeof(this->m_mercenarySp)));
  AppendBytes(records, BuildReplayTaggedRecord(5216, &this->m_mercenaryMaxSp, sizeof(this->m_mercenaryMaxSp)));
  AppendBytes(records, BuildReplayTaggedRecord(5217, &this->m_mercenaryLoyalty, sizeof(this->m_mercenaryLoyalty)));
  AppendBytes(records, BuildReplayTaggedRecord(5218, &this->m_mercenarySummons, sizeof(this->m_mercenarySummons)));
  AppendBytes(records, BuildReplayTaggedRecord(5219, &this->m_mercenaryKills, sizeof(this->m_mercenaryKills)));
  AppendBytes(records, BuildReplayTaggedRecord(5220, this->unknown_5690, 4));
  AppendBytes(records, BuildReplayTaggedRecord(5221, &this->m_mercenaryExpiration, sizeof(this->m_mercenaryExpiration)));
  AppendBytes(records, BuildReplayTaggedRecord(5222, &this->unknown_5698, sizeof(this->unknown_5698)));
  AppendBytes(records, BuildReplayTaggedRecord(5223, &this->unknown_569C, sizeof(this->unknown_569C)));
  AppendBytes(records, BuildReplayTaggedRecord(5224, &this->unknown_56A0, sizeof(this->unknown_56A0)));
  AppendBytes(records, BuildReplayTaggedRecord(5225, &this->unknown_56A4, sizeof(this->unknown_56A4)));
  AppendBytes(records, BuildReplayTaggedRecord(5226, &this->unknown_56A8, sizeof(this->unknown_56A8)));
  AppendBytes(records, BuildReplayTaggedRecord(5227, &this->unknown_56AC, sizeof(this->unknown_56AC)));
  AppendBytes(records, BuildReplayTaggedRecord(5228, &this->unknown_56B0, sizeof(this->unknown_56B0)));
  AppendBytes(records, BuildReplayTaggedRecord(5229, &this->m_mercenaryActionState, sizeof(this->m_mercenaryActionState)));
  AppendBytes(records, BuildReplayTaggedRecord(5301, &this->unknown_1010, sizeof(this->unknown_1010)));
  AppendBytes(records, BuildReplayTaggedRecord(5302, this->unknown_1014, 4));
  AppendBytes(records, BuildReplayTaggedRecord(5303, this->unknown_1018, 32));
  AppendBytes(records, BuildReplayTaggedRecord(5304, &this->unknown_1038, sizeof(this->unknown_1038)));
  AppendBytes(records, BuildReplayTaggedRecord(5305, &this->unknown_103C, sizeof(this->unknown_103C)));
  AppendBytes(records, BuildReplayTaggedRecord(5306, &this->unknown_1040, sizeof(this->unknown_1040)));
  AppendBytes(records, BuildReplayTaggedRecord(5307, &this->unknown_1044, sizeof(this->unknown_1044)));
  AppendBytes(records, BuildReplayTaggedRecord(5308, &this->m_petFriendly, sizeof(this->m_petFriendly)));
  AppendBytes(records, BuildReplayTaggedRecord(5309, &this->unknown_1050, sizeof(this->unknown_1050)));
  AppendBytes(records, BuildReplayTaggedRecord(5310, &this->unknown_1054, sizeof(this->unknown_1054)));
  AppendBytes(records, BuildReplayTaggedRecord(5401, &this->unknown_5BC0, sizeof(this->unknown_5BC0)));
  AppendBytes(records, BuildReplayTaggedRecord(5402, &this->unknown_5BC4, sizeof(this->unknown_5BC4)));
  AppendBytes(records, BuildReplayTaggedRecord(5403, &this->unknown_5BC8, sizeof(this->unknown_5BC8)));
  AppendBytes(records, BuildReplayTaggedRecord(5404, &this->unknown_5BCC, sizeof(this->unknown_5BCC)));
  AppendBytes(records, BuildReplayTaggedRecord(5405, &this->unknown_5BD0, sizeof(this->unknown_5BD0)));
  AppendBytes(records, BuildReplayTaggedRecord(5001, nullptr, 0));
  return records;
}

namespace {
#pragma pack(push, 1)
struct ReplayShopNameRecord {
  /* 0000 */ std::int16_t unknown;
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
  {
    const auto record = BuildReplayTaggedRecord(251, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(253, &this->valid, sizeof(this->valid));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(254, &this->accountId, sizeof(this->accountId));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(255, &this->characterId, sizeof(this->characterId));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(258, &this->leader, sizeof(this->leader));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(259, &this->offline, sizeof(this->offline));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(260, &this->color, sizeof(this->color));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(256, this->name.c_str(), this->name.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(257, this->map.c_str(), this->map.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(252, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x00E3E550
// FUNCTION: fn_24197
std::vector<unsigned char> CSession::BuildCharacterReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(1000, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1010, &this->m_aid, sizeof(this->m_aid));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1011, &this->m_gid, sizeof(this->m_gid));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1012, &this->m_navigationPlayerMap[0x14], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1013, &this->m_navigationPlayerMap[0x18], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1014, &this->m_job, sizeof(this->m_job));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1015, &this->unknown_162C[0x4], 8);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1016, &this->m_level, sizeof(this->m_level));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1017, this->unknown_1654, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1018, &this->unknown_162C[0xC], 8);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1019, &this->m_jobLevel, sizeof(this->m_jobLevel));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1020, this->unknown_165C, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1021, &this->m_guildSkillPoint, sizeof(this->m_guildSkillPoint));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1022, this->unknown_1664, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1023, &this->unknown_1664[0x4], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1024, &this->m_standardStats[0], sizeof(this->m_standardStats[0]));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1025, &this->m_standardStats[1], sizeof(this->m_standardStats[1]));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1026, &this->m_standardStats[2], sizeof(this->m_standardStats[2]));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1027, &this->m_standardStats[3], sizeof(this->m_standardStats[3]));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1028, &this->m_standardStats[4], sizeof(this->m_standardStats[4]));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1029, &this->m_standardStats[5], sizeof(this->m_standardStats[5]));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1030, this->unknown_169C, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1031, &this->unknown_169C[0x4], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1032, &this->unknown_169C[0x8], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1033, &this->unknown_169C[0xC], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1034, &this->unknown_169C[0x10], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1035, &this->unknown_169C[0x14], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1036, &this->unknown_169C[0x18], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1037, &this->unknown_169C[0x1C], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1038, &this->unknown_169C[0x20], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1039, &this->unknown_169C[0x24], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1040, &this->unknown_169C[0x28], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1041, &this->unknown_169C[0x2C], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1042, &this->unknown_169C[0x30], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1043, &this->unknown_169C[0x34], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(1044, &this->unknown_169C[0x38], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  AppendBytes(records, BuildReplayTaggedRecord(1045, &this->unknown_169C[0x3C], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1046, &this->unknown_169C[0x40], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1047, &this->unknown_169C[0x44], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1048, &this->unknown_169C[0x48], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1049, &this->unknown_169C[0x4C], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1050, &this->unknown_16EC, sizeof(this->unknown_16EC)));
  AppendBytes(records, BuildReplayTaggedRecord(1051, &this->m_zeny, sizeof(this->m_zeny)));
  AppendBytes(records, BuildReplayTaggedRecord(1052, this->unknown_16F4, 4));
  AppendBytes(records, BuildReplayTaggedRecord(1053, &this->unknown_16F4[0x4], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1054, &this->unknown_162C[0x14], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1055, &this->unknown_162C[0x1C], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1056, this->unknown_1704, 4));
  AppendBytes(records, BuildReplayTaggedRecord(1057, &this->unknown_1708, sizeof(this->unknown_1708)));
  AppendBytes(records, BuildReplayTaggedRecord(1058, this->unknown_170C, 4));
  AppendBytes(records, BuildReplayTaggedRecord(1059, &this->unknown_0E9C[0x34], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1060, &this->m_head, sizeof(this->m_head)));
  AppendBytes(records, BuildReplayTaggedRecord(1061, &this->unknown_0EDC[0x4], 8));
  AppendBytes(records, BuildReplayTaggedRecord(1062, &this->unknown_0EDC[0xC], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1063, &this->m_bodyStyle, sizeof(this->m_bodyStyle)));
  AppendBytes(records, BuildReplayTaggedRecord(1064, &this->m_headPalette, sizeof(this->m_headPalette)));
  AppendBytes(records, BuildReplayTaggedRecord(1065, this->unknown_0EF4, 4));
  AppendBytes(records, BuildReplayTaggedRecord(1066, &this->unknown_0EF4[0x4], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1067, &this->unknown_0EF4[0x8], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1068, this->unknown_0F0C, 4));
  AppendBytes(records, BuildReplayTaggedRecord(1069, &this->unknown_0F0C[0x4], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1070, &this->m_jobFlags, sizeof(this->m_jobFlags)));
  AppendBytes(records, BuildReplayTaggedRecord(1071, &this->m_robe, sizeof(this->m_robe)));
  AppendBytes(records, BuildReplayTaggedRecord(1072, &this->unknown_5974[0x4], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1073, &this->unknown_5974[0x8], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1074, &this->unknown_5974[0xC], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1075, &this->unknown_5974[0x10], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1076, &this->unknown_5974[0x14], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1077, &this->unknown_5974[0x18], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1078, &this->unknown_5974[0x1C], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1079, &this->unknown_5974[0x20], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1080, &this->unknown_5974[0x24], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1081, &this->unknown_5974[0x2C], 0x18));
  AppendBytes(records, BuildReplayTaggedRecord(1082, this->unknown_0F24, 1));
  AppendBytes(records, BuildReplayTaggedRecord(1083, &this->m_score1, sizeof(this->m_score1)));
  AppendBytes(records, BuildReplayTaggedRecord(1084, &this->m_score2, sizeof(this->m_score2)));
  AppendBytes(records, BuildReplayTaggedRecord(1085, this->unknown_0F30, 1));
  AppendBytes(records, BuildReplayTaggedRecord(1086, &this->cartCount, sizeof(this->cartCount)));
  AppendBytes(records, BuildReplayTaggedRecord(1087, &this->cartMaxCount, sizeof(this->cartMaxCount)));
  AppendBytes(records, BuildReplayTaggedRecord(1088, &this->cartWeight, sizeof(this->cartWeight)));
  AppendBytes(records, BuildReplayTaggedRecord(1089, &this->cartMaxWeight, sizeof(this->cartMaxWeight)));
  AppendBytes(records, BuildReplayTaggedRecord(1090, this->unknown_0F44, 4));
  AppendBytes(records, BuildReplayTaggedRecord(1091, &this->unknown_0F44[0x4], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1092, &this->unknown_5490[0x8], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1093, &this->unknown_5490[0xC], 4));
  AppendBytes(records, BuildReplayTaggedRecord(1094, &this->unknown_54A0, sizeof(this->unknown_54A0)));
  AppendBytes(records, BuildReplayTaggedRecord(1095, &this->unknown_54A4, sizeof(this->unknown_54A4)));
  AppendBytes(records, BuildReplayTaggedRecord(1096, &this->unknown_54A8, sizeof(this->unknown_54A8)));
  AppendBytes(records, BuildReplayTaggedRecord(1097, this->unknown_550C.c_str(), this->unknown_550C.size()));
  const unsigned char option = g_serviceOptions.Get(0x40);
  AppendBytes(records, BuildReplayTaggedRecord(1098, &option, sizeof(option)));
  AppendBytes(records, BuildReplayTaggedRecord(1099, &this->unknown_5468, sizeof(this->unknown_5468)));
  AppendBytes(records, BuildReplayTaggedRecord(1100, &this->m_bodyCostume, sizeof(this->m_bodyCostume)));
  AppendBytes(records, BuildReplayTaggedRecord(1101, &this->unknown_7FBC, sizeof(this->unknown_7FBC)));
  AppendBytes(records, BuildReplayTaggedRecord(1001, nullptr, 0));
  return records;
}

// OFFSET: 0x00E41E60
// FUNCTION: fn_24201
std::vector<unsigned char> CSession::BuildShopReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(7500, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  for (auto it = this->m_nameToId.begin(); it != this->m_nameToId.end(); ++it) {
    ReplayShopNameRecord value;
    value.unknown = 0;
    value.shopId = it->second;
    std::memset(value.name, 0, sizeof(value.name));
    std::memcpy(value.name, it->first.c_str(), (std::min)(it->first.size(), sizeof(value.name)));
    const auto record = BuildReplayTaggedRecord(7502, &value, sizeof(value));
    records.insert(records.end(), record.begin(), record.end());
  }
  for (auto shop = this->m_shops.begin(); shop != this->m_shops.end(); ++shop) {
    for (unsigned int index = 0; index < shop->second.unknown_0004.size(); ++index) {
      const std::map<int, std::pair<int, int>>& items = shop->second.unknown_0004[index];
      for (auto item = items.begin(); item != items.end(); ++item) {
        ReplayShopItemRecord value;
        value.shopId = shop->first;
        value.index = index;
        value.itemId = item->first;
        value.first = item->second.first;
        value.second = item->second.second;
        const auto record = BuildReplayTaggedRecord(7504, &value, sizeof(value));
        records.insert(records.end(), record.begin(), record.end());
      }
    }
    ReplayShopFlagRecord value;
    value.shopId = shop->first;
    value.flag = shop->second.unknown_0000;
    const auto record = BuildReplayTaggedRecord(7505, &value, sizeof(value));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(7501, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x00E42600
// FUNCTION: fn_24202
std::vector<unsigned char> CSession::BuildExtraReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(5500, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5510, &this->unknown_593C, sizeof(this->unknown_593C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5511, &this->unknown_5940, sizeof(this->unknown_5940));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5512, &this->unknown_5944, sizeof(this->unknown_5944));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5513, &this->unknown_5948, sizeof(this->unknown_5948));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5514, &this->unknown_594C, sizeof(this->unknown_594C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5515, this->unknown_5950.c_str(), this->unknown_5950.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5516, &this->unknown_5968, sizeof(this->unknown_5968));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5517, &this->unknown_596C, sizeof(this->unknown_596C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5518, &this->unknown_5970, sizeof(this->unknown_5970));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(5501, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x00E3FF30
// FUNCTION: fn_24198
std::vector<unsigned char> CSession::BuildStatsReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(2000, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2010, &this->unknown_166C, sizeof(this->unknown_166C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2011, &this->unknown_1670, sizeof(this->unknown_1670));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2012, &this->unknown_1674, sizeof(this->unknown_1674));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2013, &this->unknown_1678, sizeof(this->unknown_1678));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2014, &this->unknown_167C, sizeof(this->unknown_167C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2015, &this->unknown_1680, sizeof(this->unknown_1680));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2016, &this->unknown_0F4C, sizeof(this->unknown_0F4C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2017, &this->m_weight, sizeof(this->m_weight));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2018, &this->m_maxWeight, sizeof(this->m_maxWeight));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2019, &this->unknown_532C, sizeof(this->unknown_532C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2020, &this->unknown_5330, sizeof(this->unknown_5330));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2021, &this->unknown_5334, sizeof(this->unknown_5334));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2022, &this->unknown_5338, sizeof(this->unknown_5338));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2023, &this->unknown_533C, sizeof(this->unknown_533C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2024, this->unknown_5340, sizeof(this->unknown_5340));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2025, this->unknown_5370, sizeof(this->unknown_5370));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2026, this->unknown_53A0, sizeof(this->unknown_53A0));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2027, this->unknown_53D0, sizeof(this->unknown_53D0));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2028, this->unknown_5400, 0x30);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2029, &this->unknown_5568, sizeof(this->unknown_5568));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2030, &this->unknown_556C, sizeof(this->unknown_556C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2031, &this->unknown_5570[0], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2032, &this->unknown_5570[4], 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2033, &this->m_prohibitionTarget, sizeof(this->m_prohibitionTarget));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2034, &this->unknown_5480, sizeof(this->unknown_5480));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2035, &this->unknown_5484, sizeof(this->unknown_5484));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2036, &this->unknown_5488, sizeof(this->unknown_5488));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2037, &this->unknown_548C, sizeof(this->unknown_548C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2038, this->m_accountName.c_str(), this->m_accountName.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2052, &this->unknown_8074, sizeof(this->unknown_8074));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(2001, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x00E40C60
// FUNCTION: fn_24199
std::vector<unsigned char> CSession::BuildMessengerReplayRecords() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(4000, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4010, &this->unknown_5938, 1);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4011, &this->unknown_5460, sizeof(this->unknown_5460));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4012, &this->unknown_5464, sizeof(this->unknown_5464));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4013, this->unknown_54AC.c_str(), this->unknown_54AC.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4014, this->unknown_54C4.c_str(), this->unknown_54C4.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeMessengerMembers(this->m_partyMembers);
    const auto record = MakeFramedMessage(4015, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto bytes = SerializeMessengerMembers(this->m_friends);
    const auto record = MakeFramedMessage(4016, reinterpret_cast<const std::vector<char>&>(bytes));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(4001, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}
