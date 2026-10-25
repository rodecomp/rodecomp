#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKNAME_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKNAME_H

#include <cstddef>
#include <cstdint>

namespace myGPack {

  void AssignNameKey(std::uint32_t& hash, char* destination, const char* source);

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
