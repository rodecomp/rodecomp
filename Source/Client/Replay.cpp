#include "Client/Replay.h"
#include "Contents/Configuration/ServiceConfiguration.h"

#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

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
