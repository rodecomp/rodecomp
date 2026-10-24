#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKNAME_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKNAME_H

#include <cstddef>
#include <cstdint>

namespace myGPack {

  // Shared machine algorithm used with different owner-reserved buffer extents.
  void AssignNameKey(std::uint32_t& hash, char* destination, const char* source);

  // Reconstructed name for the key embedded at archive entry offset 0x1C.
  class PakName {
  public:
    PakName();
    PakName& Assign(const char* name);
    bool operator==(const PakName& other) const;
    bool operator!=(const PakName& other) const;
    bool operator<(const PakName& other) const;

    /* 0000 */ std::uint32_t m_hash;
    /* 0004 */ char m_name[256];
  };

  static_assert(sizeof(PakName) == 260);
  static_assert(offsetof(PakName, m_name) == 4);

}

#endif
