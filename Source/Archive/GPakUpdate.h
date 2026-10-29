#ifndef RAGNAROK_SOURCE_ARCHIVE_GPAKUPDATE_H
#define RAGNAROK_SOURCE_ARCHIVE_GPAKUPDATE_H

#include "Archive/GPak.h"
#include "Archive/PakFile.h"
#include "Archive/PakName.h"

#include <compare>
#include <cstddef>
#include <cstdint>
#include <list>
#include <set>
#include <string>
#include <vector>

namespace myGPack {

  class CNotifier {
  public:
    // OFFSET: 0x00E55AC0
    virtual ~CNotifier() {
    }

    virtual void Notify(std::uint64_t current, std::uint64_t total) {
    }
  };

  class CGPakRead : public CGPak {
  public:
    bool ReadEntry(PakEntry* entry, void* buffer);
    void CollectNames(std::set<std::string>& names) const;
  };

  struct PakFreeBlock {
    /* 0000 */ std::uint32_t size;
    /* 0008 */ std::uint64_t offset;

    auto operator<=>(const PakFreeBlock& other) const = default;
  };

  static_assert(sizeof(PakFreeBlock) == 16);
  static_assert(offsetof(PakFreeBlock, offset) == 8);

  class CGPakUpdate : public CGPakRead {
  public:
    CGPakUpdate();
    ~CGPakUpdate() override;

    bool Open(const char* filename);
    bool Commit(CNotifier* notifier);
    void Close();

  protected:
    bool RemoveObsoleteEntries();
    void BuildFreeBlocks();
    std::uint64_t Allocate(std::uint32_t size);
    bool CopyEntry(const PakName& name);

    /* 0148 */ std::list<CGPakRead*> m_sources;
    /* 0150 */ std::set<std::string> m_names;
    /* 0158 */ std::vector<PakEntry> m_added;
    /* 0164 */ std::set<PakFreeBlock> m_freeBlocks;
    /* 0170 */ CWritePakFile m_output;
    /* 0190 */ CNotifier m_notifier;
    /* 0194 */ std::vector<std::uint8_t> m_auxiliary;
  };

}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(myGPack::CGPakUpdate) == 416);
#endif

#endif
