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
  // INLINED
  std::uint32_t XxHash32Round(std::uint32_t accumulator, std::uint32_t input) {
    accumulator += input * 0x85EBCA77u;
    accumulator = std::rotl(accumulator, 13);
    accumulator *= 0x9E3779B1u;
    return accumulator;
  }
}

__declspec(noinline) std::uint32_t FinalizeXxHash32(std::uint32_t hash, const void* data, std::uint32_t remaining, int);

// OFFSET: 0x004D3490
// FUNCTION: fn_00163
std::uint32_t FinalizeXxHash32(std::uint32_t hash, const void* data, std::uint32_t remaining, int) {
  const auto* current = static_cast<const unsigned char*>(data);
  remaining &= 15u;

  while (remaining >= 4u) {
    const unsigned char* const word = current;
    remaining -= 4;
    current += 4;
    hash += ReadWord(word) * 0xC2B2AE3Du;
    hash = std::rotl(hash, 17) * 0x27D4EB2Fu;
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
  const unsigned char* const end = current + length;
  std::uint32_t hash;

  if (length >= 16u) {
    const unsigned char* const limit = end - 15;
    std::uint32_t first = seed + 0x9E3779B1u + 0x85EBCA77u;
    std::uint32_t second = seed + 0x85EBCA77u;
    std::uint32_t third = seed + 0;
    std::uint32_t fourth = seed - 0x9E3779B1u;

    do {
      first = XxHash32Round(first, ReadWord(current));
      current += 4;
      second = XxHash32Round(second, ReadWord(current));
      current += 4;
      third = XxHash32Round(third, ReadWord(current));
      current += 4;
      fourth = XxHash32Round(fourth, ReadWord(current));
      current += 4;
    } while (current < limit);

    hash = std::rotl(first, 1) + std::rotl(second, 7) + std::rotl(third, 12) + std::rotl(fourth, 18);
  } else {
    hash = seed + 0x165667B1u;
  }

  hash += length;
  return FinalizeXxHash32(hash, current, length & 15u, 1);
}
