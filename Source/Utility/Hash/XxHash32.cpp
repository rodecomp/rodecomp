#include "Utility/Hash/XxHash32.h"

#include <bit>
#include <cstring>

namespace {
  // INLINED
  std::uint32_t ReadWord(const unsigned char* data) {
    std::uint32_t value;
    std::memcpy(&value, data, sizeof(value));
    return value;
  }
}

// OFFSET: 0x004D3490
// FUNCTION: fn_00163
std::uint32_t FinalizeXxHash32(std::uint32_t hash, const void* data, std::uint32_t remaining, int) {
  const auto* current = static_cast<const unsigned char*>(data);
  remaining &= 15u;

  while (remaining >= 4u) {
    hash += ReadWord(current) * 0xC2B2AE3Du;
    hash = std::rotl(hash, 17) * 0x27D4EB2Fu;
    current += 4;
    remaining -= 4;
  }

  while (remaining != 0) {
    hash += *current++ * 0x165667B1u;
    hash = std::rotl(hash, 11) * 0x9E3779B1u;
    --remaining;
  }

  hash ^= hash >> 15;
  hash *= 0x85EBCA77u;
  hash ^= hash >> 13;
  hash *= 0xC2B2AE3Du;
  hash ^= hash >> 16;
  return hash;
}

// OFFSET: 0x004D3320
// FUNCTION: fn_00162
std::uint32_t XxHash32(const void* data, std::uint32_t length, std::uint32_t seed) {
  const auto* current = static_cast<const unsigned char*>(data);
  std::uint32_t hash;

  if (length >= 16u) {
    const unsigned char* limit = current + length - 15u;
    std::uint32_t first = seed + 0x24234428u;
    std::uint32_t second = seed + 0x85EBCA77u;
    std::uint32_t third = seed;
    std::uint32_t fourth = seed - 0x9E3779B1u;

    do {
      first = std::rotl(first + ReadWord(current) * 0x85EBCA77u, 13) * 0x9E3779B1u;
      second = std::rotl(second + ReadWord(current + 4) * 0x85EBCA77u, 13) * 0x9E3779B1u;
      third = std::rotl(third + ReadWord(current + 8) * 0x85EBCA77u, 13) * 0x9E3779B1u;
      fourth = std::rotl(fourth + ReadWord(current + 12) * 0x85EBCA77u, 13) * 0x9E3779B1u;
      current += 16;
    } while (current < limit);

    hash = std::rotl(first, 1) + std::rotl(second, 7) + std::rotl(third, 12) + std::rotl(fourth, 18);
  } else {
    hash = seed + 0x165667B1u;
  }

  return FinalizeXxHash32(hash + length, current, length & 15u, 1);
}
