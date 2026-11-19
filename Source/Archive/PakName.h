#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKNAME_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKNAME_H

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace myGPack {

  // INLINED
  __forceinline void AssignNameKey(std::uint32_t& result, char* destination, const char* source) {
    std::strcpy(destination, source);
    _strlwr(destination);
    const std::size_t length = std::strlen(destination);
    std::uint32_t hash = 5381;

    for (std::size_t i = 0; i < length; ++i) {
      hash = 33 * hash + static_cast<std::int8_t>(destination[i]);
    }

    result = hash;
  }

  class PakName {
  public:
    PakName();
    explicit PakName(const char* name);
    __declspec(noinline) void Assign(const char* name);
    bool operator==(const PakName& other) const;
    bool operator!=(const PakName& other) const;
    bool operator<(const PakName& other) const;

    /* 0000 */ std::uint32_t m_hash;
    /* 0004 */ char m_name[252];
  };

  static_assert(sizeof(PakName) == 256);
  static_assert(offsetof(PakName, m_name) == 4);

}

#endif
