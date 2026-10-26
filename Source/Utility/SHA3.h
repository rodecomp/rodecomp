#ifndef RAGNAROK_SOURCE_UTILITY_SHA3_H
#define RAGNAROK_SOURCE_UTILITY_SHA3_H

#include <cstddef>
#include <cstdint>
#include <string>

struct SHA3 {
  /* 0000 */ std::uint64_t state[25];
  /* 00C8 */ std::uint64_t processedBytes;
  /* 00D0 */ unsigned int blockSize;
  /* 00D4 */ unsigned int bufferedBytes;
  /* 00D8 */ unsigned char buffer[144];
  /* 0168 */ int digestBits;

  void Add(const unsigned char* data, unsigned int length);
  std::string GetHash();
  void ProcessBlock(const unsigned char* data);
};

static_assert(offsetof(SHA3, processedBytes) == 0xC8);
static_assert(offsetof(SHA3, blockSize) == 0xD0);
static_assert(offsetof(SHA3, bufferedBytes) == 0xD4);
static_assert(offsetof(SHA3, buffer) == 0xD8);
static_assert(offsetof(SHA3, digestBits) == 0x168);

#endif
