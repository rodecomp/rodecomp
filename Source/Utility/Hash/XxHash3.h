#ifndef RAGNAROK_SOURCE_UTILITY_HASH_XXHASH3_H
#define RAGNAROK_SOURCE_UTILITY_HASH_XXHASH3_H

#include <cstddef>
#include <cstdint>

struct XXH128_hash_t {
  /* 0000 */ std::uint64_t low64;
  /* 0008 */ std::uint64_t high64;
};

extern "C" std::uint64_t __cdecl XXH3_64bits(const void* input, std::size_t length);
extern "C" XXH128_hash_t __cdecl XXH3_128bits(const void* input, std::size_t length);

#endif
