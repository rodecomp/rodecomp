#ifndef RAGNAROK_SOURCE_UTILITY_XXHASH3_H
#define RAGNAROK_SOURCE_UTILITY_XXHASH3_H

#include <cstddef>
#include <cstdint>

enum XXH_alignment {
  XXH_aligned,
  XXH_unaligned
};

struct XXH128_hash_t {
  /* 0000 */ std::uint64_t low64;
  /* 0008 */ std::uint64_t high64;
};

struct alignas(64) XXH3_state_t {
  /* 0000 */ std::uint64_t acc[8];
  /* 0040 */ unsigned char customSecret[192];
  /* 0100 */ unsigned char buffer[256];
  /* 0200 */ std::uint32_t bufferedSize;
  /* 0204 */ std::uint32_t useSeed;
  /* 0208 */ std::size_t nbStripesSoFar;
  /* 0210 */ std::uint64_t totalLen;
  /* 0218 */ std::size_t nbStripesPerBlock;
  /* 021C */ std::size_t secretLimit;
  /* 0220 */ std::uint64_t seed;
  /* 0228 */ std::uint64_t reserved64;
  /* 0230 */ const unsigned char* extSecret;
};
static_assert(alignof(XXH3_state_t) == 64);
static_assert(sizeof(XXH3_state_t) == 0x240);

static_assert(offsetof(XXH3_state_t, customSecret) == 0x0040);
static_assert(offsetof(XXH3_state_t, buffer) == 0x0100);
static_assert(offsetof(XXH3_state_t, bufferedSize) == 0x0200);
static_assert(offsetof(XXH3_state_t, useSeed) == 0x0204);
static_assert(offsetof(XXH3_state_t, nbStripesSoFar) == 0x0208);
static_assert(offsetof(XXH3_state_t, totalLen) == 0x0210);
static_assert(offsetof(XXH3_state_t, nbStripesPerBlock) == 0x0218);
static_assert(offsetof(XXH3_state_t, secretLimit) == 0x021C);
static_assert(offsetof(XXH3_state_t, seed) == 0x0220);
static_assert(offsetof(XXH3_state_t, reserved64) == 0x0228);
static_assert(offsetof(XXH3_state_t, extSecret) == 0x0230);

extern const unsigned char kSecret[192];

std::uint32_t XXH_versionNumber();

std::uint64_t XXH3_64bits(const void* data, std::size_t length);
std::uint64_t XXH3_64bits_withSecret(const void* data, std::size_t length, const void* secret, std::size_t secretSize);
std::uint64_t XXH3_64bits_withSeed(const void* data, std::size_t length, std::uint64_t seed);
std::uint64_t XXH3_hashLong_64b_default(const void* data, std::size_t length, std::uint64_t seed, const void* secret, std::size_t secretSize);
std::uint64_t XXH3_hashLong_64b_withSeed(const void* data, std::size_t length, std::uint64_t seed, const void* secret, std::size_t secretSize);
std::uint64_t XXH3_len_129to240_64b(const void* data, std::size_t length, const void* secret, std::size_t secretSize, std::uint64_t seed);
std::uint64_t XXH3_mergeAccs(const std::uint64_t* acc, const void* secret, std::uint64_t start);

XXH128_hash_t XXH3_128bits(const void* data, std::size_t length);
XXH128_hash_t XXH3_128bits_withSecret(const void* data, std::size_t length, const void* secret, std::size_t secretSize);
XXH128_hash_t XXH3_128bits_withSeed(const void* data, std::size_t length, std::uint64_t seed);
XXH128_hash_t XXH3_hashLong_128b_default(const void* data, std::size_t length, std::uint64_t seed, const void* secret, std::size_t secretSize);
XXH128_hash_t XXH3_hashLong_128b_withSeed(const void* data, std::size_t length, std::uint64_t seed, const void* secret, std::size_t secretSize);
XXH128_hash_t XXH3_len_129to240_128b(const void* data, std::size_t length, const void* secret, std::size_t secretSize, std::uint64_t seed);

void XXH3_reset_internal(XXH3_state_t* state, std::uint64_t seed, const void* secret, std::size_t secretSize);
std::uint64_t XXH3_rrmxmx(std::uint64_t hash, std::uint64_t length);
std::uint64_t XXH64_round(std::uint64_t acc, std::uint64_t input);
std::uint64_t XXH64_avalanche(std::uint64_t hash);
std::uint64_t XXH64_finalize(std::uint64_t hash, const void* data, std::size_t length, XXH_alignment alignment);

#endif
