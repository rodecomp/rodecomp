#include "Utility/Hash/XxHash32.h"

#include <bit>
#include <cstring>
#include <cstdlib>

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
    hash = std::rotl(hash + ReadWord(word) * 0xC2B2AE3Du, 17) * 0x27D4EB2Fu;
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
  const unsigned char* input = static_cast<const unsigned char*>(data);
  const unsigned char* end = input + length;
  std::uint32_t hash;

  if (length >= 16u) {
    const unsigned char* const limit = end - 15;
    std::uint32_t v1 = seed + 0x9E3779B1u + 0x85EBCA77u;
    std::uint32_t v2 = seed + 0x85EBCA77u;
    std::uint32_t v3 = seed + 0;
    std::uint32_t v4 = seed - 0x9E3779B1u;

    do {
      v1 = XxHash32Round(v1, ReadWord(input));
      input += 4;
      v2 = XxHash32Round(v2, ReadWord(input));
      input += 4;
      v3 = XxHash32Round(v3, ReadWord(input));
      input += 4;
      v4 = XxHash32Round(v4, ReadWord(input));
      input += 4;
    } while (input < limit);

    hash = _rotl(v1, 1) + _rotl(v2, 7) + _rotl(v3, 12) + _rotl(v4, 18);
  } else {
    hash = seed + 0x165667B1u;
  }

  hash += length;
  return FinalizeXxHash32(hash, input, length & 15u, 1);
}
