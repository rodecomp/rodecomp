#include "Client/Replay.h"

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

// OFFSET: 0x006DA340
// FUNCTION: fn_06623
static std::vector<unsigned char> BuildReplayTaggedRecord(std::uint16_t id, const void* data, std::uint32_t bytes) {
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


// OFFSET: 0x00B83A40

// OFFSET: 0x00B83AE0

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
  this->header.Reset();
  this->records = 0;
  this->fileMode = -1;
  this->file = nullptr;
  this->fileOffset = 0;
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
