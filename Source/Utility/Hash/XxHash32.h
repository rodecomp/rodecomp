#ifndef RAGNAROK_SOURCE_UTILITY_HASH_XXHASH32_H
#define RAGNAROK_SOURCE_UTILITY_HASH_XXHASH32_H

#include <cstddef>
#include <cstdint>

extern "C" unsigned int __cdecl XXH32(const void* input, std::size_t length, unsigned int seed);

// INLINED
inline std::uint32_t XxHash32(const void* data, std::size_t length, std::uint32_t seed)
{
  return XXH32(data, length, seed);
}

#endif
