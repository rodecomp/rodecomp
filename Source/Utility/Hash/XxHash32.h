#ifndef RAGNAROK_SOURCE_UTILITY_HASH_XXHASH32_H
#define RAGNAROK_SOURCE_UTILITY_HASH_XXHASH32_H

#include <cstdint>

std::uint32_t XxHash32(const void* data, std::uint32_t length, std::uint32_t seed);
std::uint32_t FinalizeXxHash32(std::uint32_t hash, const void* data, std::uint32_t remaining, int endian);

#endif
