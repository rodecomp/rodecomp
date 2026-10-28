#ifndef RAGNAROK_SOURCE_CLIENT_REPLAY_H
#define RAGNAROK_SOURCE_CLIENT_REPLAY_H
#include "Client/ReplayRecordOption.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

struct ReplayCachedPacket {
  /* 0000 */ bool enabled;
  /* 0004 */ std::vector<unsigned char> bytes;

  ReplayCachedPacket() = default;
  ReplayCachedPacket(const ReplayCachedPacket& other);
};

#pragma pack(push, 1)
struct ReplayOptions {
  /* 0000 */ unsigned char unknown_0000[6];
  /* 0006 */ unsigned char recordRestricted;
  /* 0007 */ unsigned char unknown_0007;
  /* 0008 */ unsigned char unknown_0008[22];

  unsigned char Get(ReplayRecordOption option) const {
    return reinterpret_cast<const unsigned char*>(this)[static_cast<std::size_t>(option)];
  }

  void Set(ReplayRecordOption option, unsigned char value) {
    reinterpret_cast<unsigned char*>(this)[static_cast<std::size_t>(option)] = value;
  }
};

struct ReplayHeader {
  /* 0000 */ std::uint32_t signature;
  /* 0004 */ std::uint32_t key0;
  /* 0008 */ std::uint32_t key1;
  /* 000C */ int service;
  /* 0010 */ int serviceType;
  /* 0014 */ int unknown_0014;
  /* 0018 */ char characterName[64];
  /* 0058 */ char mapName[20];
  /* 006C */ int accountId;
  /* 0070 */ int cellX;
  /* 0074 */ int cellY;
  /* 0078 */ int unknown_0078;
  /* 007C */ std::uint32_t duration;
  /* 0080 */ std::uint32_t records;
  /* 0084 */ std::uint32_t fileOffset;
  /* 0088 */ ReplayOptions options;
  /* 00A6 */ std::uint32_t closingSignature;

  // INLINED
  void Reset() {
    const ReplayOptions savedOptions = this->options;
    std::memset(this, 0, sizeof(*this));
    this->options = savedOptions;
    this->signature = 0x52414705;
    this->closingSignature = 0x52414705;
  }
};

struct ReplaySection {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t bytes;
  /* 0006 */ std::uint32_t fileOffset;

  ReplaySection();
  ~ReplaySection();
  void Clear();
};
#pragma pack(pop)

class Replay {
public:
  static Replay& Instance();

  /* 0000 */ std::vector<ReplayCachedPacket> cachedPackets;
  /* 000C */ int state;
  /* 0010 */ std::uint32_t baseFrame;
  /* 0014 */ std::uint32_t started;
  /* 0018 */ std::uint32_t previousTick;
  /* 001C */ std::uint32_t frame;
  /* 0020 */ std::uint32_t elapsed;
  /* 0024 */ float speed;
  /* 0028 */ ReplayHeader header;
  /* 00D2 */ ReplaySection sections[26];
  /* 01D8 */ int fileMode;
  /* 01DC */ std::FILE* file;
  /* 01E0 */ std::uint32_t fileOffset;
  /* 01E4 */ std::uint32_t records;
  /* 01E8 */ std::uint32_t cipherIndex;
  /* 01EC */ std::uint32_t cipherBase;
  /* 01F0 */ std::uint32_t cipherKey;
  /* 01F4 */ int unknown_01F4;
  /* 01F8 */ int unknown_01F8;

  Replay();

  // INLINED
  ~Replay() {
    if (this->file) {
      if (this->fileMode == 31) {
        this->WriteHeader(13);
      }
      std::fclose(this->file);
      this->file = nullptr;
    }
  }

  bool WriteSection(int id, std::vector<unsigned char>& bytes, int event);
  int WriteHeader(int event);
  std::vector<unsigned char> BuildMetadata();
  float AnimationScale() const;
  float DelayScale() const;
  bool SetState(int state);
  bool SetBaseFrame(std::uint32_t baseFrame);
  void UpdateDuration();
  bool WriteFinalSection();
  bool RecordPacket(const std::vector<unsigned char>& packet, int event, std::uint32_t delay);
};

std::vector<unsigned char> BuildReplayTaggedRecord(std::uint16_t id, const void* data, std::uint32_t bytes);

extern int g_replayAnimationScaling;

static_assert(sizeof(ReplayCachedPacket) == 0x10);
static_assert(offsetof(ReplayCachedPacket, bytes) == 0x4);
static_assert(sizeof(ReplayOptions) == 0x1E);
static_assert(sizeof(ReplayHeader) == 0xAA);
static_assert(offsetof(ReplayHeader, options) == 0x88);
static_assert(offsetof(ReplayHeader, closingSignature) == 0xA6);
static_assert(sizeof(ReplaySection) == 0xA);
static_assert(offsetof(ReplaySection, bytes) == 0x2);
static_assert(offsetof(ReplaySection, fileOffset) == 0x6);
static_assert(sizeof(Replay) == 0x1FC);
static_assert(offsetof(Replay, state) == 0xC);
static_assert(offsetof(Replay, header) == 0x28);
static_assert(offsetof(Replay, header) + offsetof(ReplayHeader, options) + offsetof(ReplayOptions, recordRestricted) == 0xB6);
static_assert(offsetof(Replay, header) + offsetof(ReplayHeader, options) + offsetof(ReplayOptions, unknown_0007) == 0xB7);
static_assert(offsetof(Replay, sections) == 0xD2);
static_assert(offsetof(Replay, fileMode) == 0x1D8);
static_assert(offsetof(Replay, file) == 0x1DC);
static_assert(offsetof(Replay, cipherIndex) == 0x1E8);
static_assert(offsetof(Replay, unknown_01F8) == 0x1F8);

#endif
