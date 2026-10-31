#ifndef RAGNAROK_SOURCE_ARCHIVE_GPAK_H
#define RAGNAROK_SOURCE_ARCHIVE_GPAK_H

#include "Archive/PakFile.h"
#include "Archive/PakName.h"

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

namespace myGPack {

  struct PakEntry {
    /* 0000 */ std::uint32_t alignedSize;
    /* 0004 */ std::uint32_t compressedSize;
    /* 0008 */ std::uint32_t originalSize;
    /* 000C */ std::uint32_t unknown_000C;
    /* 0010 */ std::uint64_t dataOffset;
    /* 0018 */ std::uint8_t flags;
    /* 0019 */ std::uint8_t unknown_0019[3];
    /* 001C */ PakName name;
  };

  static_assert(sizeof(PakEntry) == 288);
  static_assert(offsetof(PakEntry, dataOffset) == 16);
  static_assert(offsetof(PakEntry, flags) == 24);
  static_assert(offsetof(PakEntry, name) == 28);

}

struct PakAuxiliary {
  PakAuxiliary();
  ~PakAuxiliary();

  void Apply(const void* data, std::uint32_t size, const void* auxData, std::uint32_t auxSize);
  bool Extract(std::string_view name, const std::uint8_t* data, std::uint32_t size, std::span<const std::uint8_t> scratch, void* output, std::uint32_t outputSize, unsigned flags);

  /* 0000 */ std::uint8_t unknown_0000[244];
};

static_assert(sizeof(PakAuxiliary) == 244);

class CGPak {
public:
  CGPak();
  virtual ~CGPak();

  bool Open(CPakFile* file);
  void Reset();
  bool Find(const myGPack::PakName& name, myGPack::PakEntry* entry) const;
  bool Extract(const myGPack::PakEntry* entry, void* buffer);
  void ReadAuxiliary(std::vector<std::uint8_t>& output, std::uint64_t* identifier = nullptr);
  void ApplyAuxiliary(const void* data, std::uint32_t size, const void* auxData, std::uint32_t auxSize);

protected:
  bool ReadTableV1();
  bool ReadTableV2();
  bool ReadTableV3();

  /* 0008 */ alignas(8) CPakFile* m_file;
  /* 000C */ std::uint32_t m_version;
  /* 0010 */ std::uint32_t m_entryCount;
  /* 0014 */ std::uint32_t unknown_0014;
  /* 0018 */ std::uint64_t m_tableOffset;
  /* 0020 */ std::uint32_t m_tableSize;
  /* 0024 */ std::vector<myGPack::PakEntry> m_entries;
  /* 0030 */ std::vector<std::uint8_t> m_scratch;
  /* 003C */ CRITICAL_SECTION m_criticalSection;
  /* 0054 */ PakAuxiliary m_auxiliary;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGPak) == 328);
#endif

#endif
