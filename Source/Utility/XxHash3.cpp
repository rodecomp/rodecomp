#include "Utility/XxHash3.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <emmintrin.h>
#include <intrin.h>

namespace {
  constexpr std::uint32_t XXH_PRIME32_1 = 0x9E3779B1U;
  constexpr std::uint32_t XXH_PRIME32_2 = 0x85EBCA77U;
  constexpr std::uint32_t XXH_PRIME32_3 = 0xC2B2AE3DU;

  constexpr std::uint64_t XXH_PRIME64_1 = 0x9E3779B185EBCA87ULL;
  constexpr std::uint64_t XXH_PRIME64_2 = 0xC2B2AE3D27D4EB4FULL;
  constexpr std::uint64_t XXH_PRIME64_3 = 0x165667B19E3779F9ULL;
  constexpr std::uint64_t XXH_PRIME64_4 = 0x85EBCA77C2B2AE63ULL;
  constexpr std::uint64_t XXH_PRIME64_5 = 0x27D4EB2F165667C5ULL;

  constexpr std::uint64_t XXH_PRIME_MX1 = 0x165667919E3779F9ULL;
  constexpr std::uint64_t XXH_PRIME_MX2 = 0x9FB21C651E98DF25ULL;

  constexpr std::size_t XXH_STRIPE_LEN = 64;
  constexpr std::size_t XXH_SECRET_CONSUME_RATE = 8;
  enum : std::size_t { XXH_ACC_NB = XXH_STRIPE_LEN / sizeof(std::uint64_t) };
  constexpr std::size_t XXH_SECRET_DEFAULT_SIZE = 192;
  constexpr std::size_t XXH_SECRET_SIZE_MIN = 136;
  constexpr std::size_t XXH3_MIDSIZE_MAX = 240;
  constexpr std::size_t XXH3_MIDSIZE_STARTOFFSET = 3;
  constexpr std::size_t XXH3_MIDSIZE_LASTOFFSET = 17;
  constexpr std::size_t XXH_SECRET_LASTACC_START = 7;
  constexpr std::size_t XXH_SECRET_MERGEACCS_START = 11;
  constexpr int XXH_PREFETCH_DIST = 384;

  constexpr unsigned XXH_VERSION_MAJOR = 0;
  constexpr unsigned XXH_VERSION_MINOR = 8;
  constexpr unsigned XXH_VERSION_RELEASE = 1;
  constexpr unsigned XXH_VERSION_NUMBER =
      XXH_VERSION_MAJOR * 100 * 100 + XXH_VERSION_MINOR * 100 + XXH_VERSION_RELEASE;

  using XXH3_f_accumulate_512 = void (*)(void*, const void*, const void*);
  using XXH3_f_scrambleAcc = void (*)(void*, const void*);
  using XXH3_f_initCustomSecret = void (*)(void*, std::uint64_t);
  using XXH3_hashLong64_f = std::uint64_t (*)(const void*, std::size_t, std::uint64_t, const void*, std::size_t);
  using XXH3_hashLong128_f = XXH128_hash_t (*)(const void*, std::size_t, std::uint64_t, const void*, std::size_t);

  // INLINED
  __forceinline std::uint32_t ReadLE32(const void* data) {
    std::uint32_t value;
    std::memcpy(&value, data, sizeof(value));
    return value;
  }

  // INLINED
  __forceinline std::uint64_t ReadLE64(const void* data) {
    std::uint64_t value;
    std::memcpy(&value, data, sizeof(value));
    return value;
  }

  // INLINED
  __forceinline std::uint32_t Swap32(std::uint32_t value) { return _byteswap_ulong(value); }

  // INLINED
  __forceinline std::uint64_t Swap64(std::uint64_t value) { return _byteswap_uint64(value); }

  // INLINED
  __forceinline std::uint32_t Rotl32(std::uint32_t value, int bits) { return (value << bits) | (value >> (32 - bits)); }

  // INLINED
  __forceinline std::uint64_t Rotl64(std::uint64_t value, int bits) { return (value << bits) | (value >> (64 - bits)); }

  // INLINED
  __forceinline std::uint64_t Xorshift64(std::uint64_t value, int bits) { return value ^ (value >> bits); }

  // INLINED
  __forceinline std::uint64_t Mult32To64(std::uint64_t lhs, std::uint64_t rhs) {
    return __emulu(static_cast<unsigned>(lhs), static_cast<unsigned>(rhs));
  }

  // INLINED
  __forceinline XXH128_hash_t Mult64To128(std::uint64_t lhs, std::uint64_t rhs) {
    std::uint64_t const loLo = Mult32To64(lhs & 0xFFFFFFFFULL, rhs & 0xFFFFFFFFULL);
    std::uint64_t const hiLo = Mult32To64(lhs >> 32, rhs & 0xFFFFFFFFULL);
    std::uint64_t const loHi = Mult32To64(lhs & 0xFFFFFFFFULL, rhs >> 32);
    std::uint64_t const hiHi = Mult32To64(lhs >> 32, rhs >> 32);

    std::uint64_t const cross = (loLo >> 32) + (hiLo & 0xFFFFFFFFULL) + loHi;
    std::uint64_t const upper = (hiLo >> 32) + (cross >> 32) + hiHi;
    std::uint64_t const lower = (cross << 32) | (loLo & 0xFFFFFFFFULL);
    return XXH128_hash_t{lower, upper};
  }

  // INLINED
  __forceinline std::uint64_t Mul128Fold64(std::uint64_t lhs, std::uint64_t rhs) {
    XXH128_hash_t const product = Mult64To128(lhs, rhs);
    return product.low64 ^ product.high64;
  }

  // INLINED
  __forceinline std::uint64_t Avalanche64b3(std::uint64_t hash) {
    hash = Xorshift64(hash, 37);
    hash *= XXH_PRIME_MX1;
    hash = Xorshift64(hash, 32);
    return hash;
  }

  // INLINED
  __forceinline std::uint64_t Mix16B(const std::uint8_t* input, const std::uint8_t* secret, std::uint64_t seed) {
    std::uint64_t const inputLo = ReadLE64(input);
    std::uint64_t const inputHi = ReadLE64(input + 8);
    return Mul128Fold64(inputLo ^ (ReadLE64(secret) + seed), inputHi ^ (ReadLE64(secret + 8) - seed));
  }

  // INLINED
  __forceinline std::uint64_t Mix2Accs(const std::uint64_t* acc, const std::uint8_t* secret) {
    return Mul128Fold64(acc[0] ^ ReadLE64(secret), acc[1] ^ ReadLE64(secret + 8));
  }

  // INLINED
  __forceinline void Accumulate512(void* acc, const void* input, const void* secret) {
    __m128i* const xacc = static_cast<__m128i*>(acc);
    const __m128i* const xinput = static_cast<const __m128i*>(input);
    const __m128i* const xsecret = static_cast<const __m128i*>(secret);

    for (std::size_t i = 0; i < XXH_STRIPE_LEN / sizeof(__m128i); ++i) {
      __m128i const dataVec = _mm_loadu_si128(xinput + i);
      __m128i const keyVec = _mm_loadu_si128(xsecret + i);
      __m128i const dataKey = _mm_xor_si128(dataVec, keyVec);
      __m128i const dataKeyLo = _mm_shuffle_epi32(dataKey, _MM_SHUFFLE(0, 3, 0, 1));
      __m128i const product = _mm_mul_epu32(dataKey, dataKeyLo);
      __m128i const dataSwap = _mm_shuffle_epi32(dataVec, _MM_SHUFFLE(1, 0, 3, 2));
      __m128i const sum = _mm_add_epi64(xacc[i], dataSwap);
      xacc[i] = _mm_add_epi64(product, sum);
    }
  }

  // INLINED
  __forceinline void ScrambleAcc(void* acc, const void* secret) {
    __m128i* const xacc = static_cast<__m128i*>(acc);
    const __m128i* const xsecret = static_cast<const __m128i*>(secret);
    __m128i const prime32 = _mm_set1_epi32(static_cast<int>(XXH_PRIME32_1));

    for (std::size_t i = 0; i < XXH_STRIPE_LEN / sizeof(__m128i); ++i) {
      __m128i const accVec = xacc[i];
      __m128i const shifted = _mm_srli_epi64(accVec, 47);
      __m128i const dataVec = _mm_xor_si128(accVec, shifted);
      __m128i const keyVec = _mm_loadu_si128(xsecret + i);
      __m128i const dataKey = _mm_xor_si128(dataVec, keyVec);
      __m128i const dataKeyHi = _mm_shuffle_epi32(dataKey, _MM_SHUFFLE(0, 3, 0, 1));
      __m128i const prodLo = _mm_mul_epu32(dataKey, prime32);
      __m128i const prodHi = _mm_mul_epu32(dataKeyHi, prime32);
      xacc[i] = _mm_add_epi64(prodLo, _mm_slli_epi64(prodHi, 32));
    }
  }

  // INLINED
  __forceinline void InitCustomSecret(void* customSecret, std::uint64_t seed) {
    __m128i const seedVector = _mm_set_epi64x(static_cast<long long>(0U - seed), static_cast<long long>(seed));
    const __m128i* const source = reinterpret_cast<const __m128i*>(kSecret);
    __m128i* const destination = static_cast<__m128i*>(customSecret);

    for (std::size_t i = 0; i < XXH_SECRET_DEFAULT_SIZE / sizeof(__m128i); ++i) {
      destination[i] = _mm_add_epi64(_mm_load_si128(source + i), seedVector);
    }
  }

  // INLINED
  __forceinline void Accumulate(std::uint64_t* acc, const std::uint8_t* input, const std::uint8_t* secret,
                                std::size_t nbStripes, XXH3_f_accumulate_512 f_acc512) {
    for (std::size_t n = 0; n < nbStripes; ++n) {
      const std::uint8_t* const in = input + n * XXH_STRIPE_LEN;
      _mm_prefetch(reinterpret_cast<const char*>(in) + XXH_PREFETCH_DIST, _MM_HINT_T0);
      f_acc512(acc, in, secret + n * XXH_SECRET_CONSUME_RATE);
    }
  }

  // INLINED
  __forceinline void HashLongInternalLoop(std::uint64_t* acc, const std::uint8_t* input, std::size_t length,
                                          const std::uint8_t* secret, std::size_t secretSize,
                                          XXH3_f_accumulate_512 f_acc512, XXH3_f_scrambleAcc f_scramble) {
    std::size_t const nbStripesPerBlock = (secretSize - XXH_STRIPE_LEN) / XXH_SECRET_CONSUME_RATE;
    std::size_t const blockLength = XXH_STRIPE_LEN * nbStripesPerBlock;
    std::size_t const nbBlocks = (length - 1) / blockLength;

    for (std::size_t n = 0; n < nbBlocks; ++n) {
      Accumulate(acc, input + n * blockLength, secret, nbStripesPerBlock, f_acc512);
      f_scramble(acc, secret + secretSize - XXH_STRIPE_LEN);
    }

    std::size_t const nbStripes = ((length - 1) - (blockLength * nbBlocks)) / XXH_STRIPE_LEN;
    Accumulate(acc, input + nbBlocks * blockLength, secret, nbStripes, f_acc512);

    const std::uint8_t* const lastStripe = input + length - XXH_STRIPE_LEN;
    f_acc512(acc, lastStripe, secret + secretSize - XXH_STRIPE_LEN - XXH_SECRET_LASTACC_START);
  }

  // INLINED
  __forceinline std::uint64_t Len1to3_64b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                          std::uint64_t seed) {
    std::uint8_t const c1 = input[0];
    std::uint8_t const c2 = input[length >> 1];
    std::uint8_t const c3 = input[length - 1];
    std::uint32_t const combined = (static_cast<std::uint32_t>(c1) << 16) | (static_cast<std::uint32_t>(c2) << 24) |
                                   (static_cast<std::uint32_t>(c3) << 0) |
                                   (static_cast<std::uint32_t>(length) << 8);
    std::uint64_t const bitflip = (ReadLE32(secret) ^ ReadLE32(secret + 4)) + seed;
    std::uint64_t const keyed = static_cast<std::uint64_t>(combined) ^ bitflip;
    return XXH64_avalanche(keyed);
  }

  // INLINED
  __forceinline std::uint64_t Len4to8_64b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                          std::uint64_t seed) {
    seed ^= static_cast<std::uint64_t>(Swap32(static_cast<std::uint32_t>(seed))) << 32;
    std::uint32_t const input1 = ReadLE32(input);
    std::uint32_t const input2 = ReadLE32(input + length - 4);
    std::uint64_t const bitflip = (ReadLE64(secret + 8) ^ ReadLE64(secret + 16)) - seed;
    std::uint64_t const input64 = input2 + (static_cast<std::uint64_t>(input1) << 32);
    std::uint64_t const keyed = input64 ^ bitflip;
    return XXH3_rrmxmx(keyed, length);
  }

  // INLINED
  __forceinline std::uint64_t Len9to16_64b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                           std::uint64_t seed) {
    std::uint64_t const bitflip1 = (ReadLE64(secret + 24) ^ ReadLE64(secret + 32)) + seed;
    std::uint64_t const bitflip2 = (ReadLE64(secret + 40) ^ ReadLE64(secret + 48)) - seed;
    std::uint64_t const inputLo = ReadLE64(input) ^ bitflip1;
    std::uint64_t const inputHi = ReadLE64(input + length - 8) ^ bitflip2;
    std::uint64_t const acc =
        length + Swap64(inputLo) + inputHi + Mul128Fold64(inputLo, inputHi);
    return Avalanche64b3(acc);
  }

  // INLINED
  __forceinline std::uint64_t Len0to16_64b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                           std::uint64_t seed) {
    if (length > 8) {
      return Len9to16_64b(input, length, secret, seed);
    }
    if (length >= 4) {
      return Len4to8_64b(input, length, secret, seed);
    }
    if (length != 0) {
      return Len1to3_64b(input, length, secret, seed);
    }
    return XXH64_avalanche(seed ^ (ReadLE64(secret + 56) ^ ReadLE64(secret + 64)));
  }

  // INLINED
  __forceinline std::uint64_t Len17to128_64b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                             std::uint64_t seed) {
    std::uint64_t acc = length * XXH_PRIME64_1;
    if (length > 32) {
      if (length > 64) {
        if (length > 96) {
          acc += Mix16B(input + 48, secret + 96, seed);
          acc += Mix16B(input + length - 64, secret + 112, seed);
        }
        acc += Mix16B(input + 32, secret + 64, seed);
        acc += Mix16B(input + length - 48, secret + 80, seed);
      }
      acc += Mix16B(input + 16, secret + 32, seed);
      acc += Mix16B(input + length - 32, secret + 48, seed);
    }
    acc += Mix16B(input + 0, secret + 0, seed);
    acc += Mix16B(input + length - 16, secret + 16, seed);
    return Avalanche64b3(acc);
  }

  // INLINED
  __forceinline XXH128_hash_t Len1to3_128b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                           std::uint64_t seed) {
    std::uint8_t const c1 = input[0];
    std::uint8_t const c2 = input[length >> 1];
    std::uint8_t const c3 = input[length - 1];
    std::uint32_t const combinedl = (static_cast<std::uint32_t>(c1) << 16) |
                                    (static_cast<std::uint32_t>(c2) << 24) | (static_cast<std::uint32_t>(c3) << 0) |
                                    (static_cast<std::uint32_t>(length) << 8);
    std::uint32_t const combinedh = Rotl32(Swap32(combinedl), 13);
    std::uint64_t const bitflipl = (ReadLE32(secret) ^ ReadLE32(secret + 4)) + seed;
    std::uint64_t const bitfliph = (ReadLE32(secret + 8) ^ ReadLE32(secret + 12)) - seed;
    std::uint64_t const keyedLo = static_cast<std::uint64_t>(combinedl) ^ bitflipl;
    std::uint64_t const keyedHi = static_cast<std::uint64_t>(combinedh) ^ bitfliph;
    return XXH128_hash_t{XXH64_avalanche(keyedLo), XXH64_avalanche(keyedHi)};
  }

  // INLINED
  __forceinline XXH128_hash_t Len4to8_128b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                           std::uint64_t seed) {
    seed ^= static_cast<std::uint64_t>(Swap32(static_cast<std::uint32_t>(seed))) << 32;
    std::uint32_t const inputLo = ReadLE32(input);
    std::uint32_t const inputHi = ReadLE32(input + length - 4);
    std::uint64_t const input64 = inputLo + (static_cast<std::uint64_t>(inputHi) << 32);
    std::uint64_t const bitflip = (ReadLE64(secret + 16) ^ ReadLE64(secret + 24)) + seed;
    std::uint64_t const keyed = input64 ^ bitflip;

    XXH128_hash_t m128 = Mult64To128(keyed, XXH_PRIME64_1 + (static_cast<std::uint64_t>(length) << 2));
    m128.high64 += (m128.low64 << 1);
    m128.low64 ^= (m128.high64 >> 3);
    m128.low64 = Xorshift64(m128.low64, 35);
    m128.low64 *= XXH_PRIME_MX2;
    m128.low64 = Xorshift64(m128.low64, 28);
    m128.high64 = Avalanche64b3(m128.high64);
    return m128;
  }

  // INLINED
  __forceinline XXH128_hash_t Len9to16_128b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                            std::uint64_t seed) {
    std::uint64_t const bitflipl = (ReadLE64(secret + 32) ^ ReadLE64(secret + 40)) - seed;
    std::uint64_t const bitfliph = (ReadLE64(secret + 48) ^ ReadLE64(secret + 56)) + seed;
    std::uint64_t const inputLo = ReadLE64(input);
    std::uint64_t inputHi = ReadLE64(input + length - 8);
    XXH128_hash_t m128 = Mult64To128(inputLo ^ inputHi ^ bitflipl, XXH_PRIME64_1);

    m128.low64 += static_cast<std::uint64_t>(length - 1) << 54;
    inputHi ^= bitfliph;
    m128.high64 += (inputHi & 0xFFFFFFFF00000000ULL) + Mult32To64(static_cast<std::uint32_t>(inputHi), XXH_PRIME32_2);
    m128.low64 ^= Swap64(m128.high64);

    XXH128_hash_t h128 = Mult64To128(m128.low64, XXH_PRIME64_2);
    h128.high64 += m128.high64 * XXH_PRIME64_2;
    h128.low64 = Avalanche64b3(h128.low64);
    h128.high64 = Avalanche64b3(h128.high64);
    return h128;
  }

  // INLINED
  __forceinline XXH128_hash_t Len0to16_128b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                            std::uint64_t seed) {
    if (length > 8) {
      return Len9to16_128b(input, length, secret, seed);
    }
    if (length >= 4) {
      return Len4to8_128b(input, length, secret, seed);
    }
    if (length != 0) {
      return Len1to3_128b(input, length, secret, seed);
    }
    std::uint64_t const bitflipl = ReadLE64(secret + 64) ^ ReadLE64(secret + 72);
    std::uint64_t const bitfliph = ReadLE64(secret + 80) ^ ReadLE64(secret + 88);
    return XXH128_hash_t{XXH64_avalanche(seed ^ bitflipl), XXH64_avalanche(seed ^ bitfliph)};
  }

  // INLINED
  __forceinline XXH128_hash_t Mix32B(XXH128_hash_t acc, const std::uint8_t* input1, const std::uint8_t* input2,
                                     const std::uint8_t* secret, std::uint64_t seed) {
    acc.low64 += Mix16B(input1, secret + 0, seed);
    acc.low64 ^= ReadLE64(input2) + ReadLE64(input2 + 8);
    acc.high64 += Mix16B(input2, secret + 16, seed);
    acc.high64 ^= ReadLE64(input1) + ReadLE64(input1 + 8);
    return acc;
  }

  // INLINED
  __forceinline XXH128_hash_t Len17to128_128b(const std::uint8_t* input, std::size_t length, const std::uint8_t* secret,
                                              std::uint64_t seed) {
    XXH128_hash_t acc{length * XXH_PRIME64_1, 0};
    if (length > 32) {
      if (length > 64) {
        if (length > 96) {
          acc = Mix32B(acc, input + 48, input + length - 64, secret + 96, seed);
        }
        acc = Mix32B(acc, input + 32, input + length - 48, secret + 64, seed);
      }
      acc = Mix32B(acc, input + 16, input + length - 32, secret + 32, seed);
    }
    acc = Mix32B(acc, input, input + length - 16, secret, seed);

    XXH128_hash_t h128;
    h128.low64 = acc.low64 + acc.high64;
    h128.high64 = (acc.low64 * XXH_PRIME64_1) + (acc.high64 * XXH_PRIME64_4) + ((length - seed) * XXH_PRIME64_2);
    h128.low64 = Avalanche64b3(h128.low64);
    h128.high64 = static_cast<std::uint64_t>(0) - Avalanche64b3(h128.high64);
    return h128;
  }

  // INLINED
  __forceinline std::uint64_t HashLong64bInternal(const void* input, std::size_t length, const void* secret,
                                                  std::size_t secretSize, XXH3_f_accumulate_512 f_acc512,
                                                  XXH3_f_scrambleAcc f_scramble) {
    std::uint64_t acc[XXH_ACC_NB] = {XXH_PRIME32_3, XXH_PRIME64_1, XXH_PRIME64_2, XXH_PRIME64_3,
                                     XXH_PRIME64_4, XXH_PRIME32_2, XXH_PRIME64_5, XXH_PRIME32_1};

    HashLongInternalLoop(acc, static_cast<const std::uint8_t*>(input), length,
                         static_cast<const std::uint8_t*>(secret), secretSize, f_acc512, f_scramble);
    return XXH3_mergeAccs(acc, static_cast<const std::uint8_t*>(secret) + XXH_SECRET_MERGEACCS_START,
                          static_cast<std::uint64_t>(length) * XXH_PRIME64_1);
  }

  // INLINED
  __forceinline XXH128_hash_t HashLong128bInternal(const void* input, std::size_t length, const void* secret,
                                                   std::size_t secretSize, XXH3_f_accumulate_512 f_acc512,
                                                   XXH3_f_scrambleAcc f_scramble) {
    std::uint64_t acc[XXH_ACC_NB] = {XXH_PRIME32_3, XXH_PRIME64_1, XXH_PRIME64_2, XXH_PRIME64_3,
                                     XXH_PRIME64_4, XXH_PRIME32_2, XXH_PRIME64_5, XXH_PRIME32_1};

    HashLongInternalLoop(acc, static_cast<const std::uint8_t*>(input), length,
                         static_cast<const std::uint8_t*>(secret), secretSize, f_acc512, f_scramble);

    XXH128_hash_t h128;
    h128.low64 = XXH3_mergeAccs(
        acc, static_cast<const std::uint8_t*>(secret) + XXH_SECRET_MERGEACCS_START,
        static_cast<std::uint64_t>(length) * XXH_PRIME64_1);
    h128.high64 = XXH3_mergeAccs(
        acc, static_cast<const std::uint8_t*>(secret) + secretSize - sizeof(acc) - XXH_SECRET_MERGEACCS_START,
        ~(static_cast<std::uint64_t>(length) * XXH_PRIME64_2));
    return h128;
  }

  // INLINED
  __forceinline std::uint64_t HashLong64bWithSecret(const void* input, std::size_t length, std::uint64_t seed,
                                                    const void* secret, std::size_t secretSize) {
    (void)seed;
    return HashLong64bInternal(input, length, secret, secretSize, Accumulate512, ScrambleAcc);
  }

  // INLINED
  __forceinline XXH128_hash_t HashLong128bWithSecret(const void* input, std::size_t length, std::uint64_t seed,
                                                     const void* secret, std::size_t secretSize) {
    (void)seed;
    return HashLong128bInternal(input, length, secret, secretSize, Accumulate512, ScrambleAcc);
  }

  // INLINED
  __forceinline std::uint64_t HashLong64bWithSeedInternal(const void* input, std::size_t length, std::uint64_t seed,
                                                          XXH3_f_accumulate_512 f_acc512,
                                                          XXH3_f_scrambleAcc f_scramble,
                                                          XXH3_f_initCustomSecret f_initSecret) {
    if (seed == 0) {
      return HashLong64bInternal(input, length, kSecret, XXH_SECRET_DEFAULT_SIZE, f_acc512, f_scramble);
    }
    __declspec(align(16)) unsigned char secret[XXH_SECRET_DEFAULT_SIZE];
    f_initSecret(secret, seed);
    return HashLong64bInternal(input, length, secret, sizeof(secret), f_acc512, f_scramble);
  }

  // INLINED
  __forceinline XXH128_hash_t HashLong128bWithSeedInternal(const void* input, std::size_t length, std::uint64_t seed,
                                                           XXH3_f_accumulate_512 f_acc512,
                                                           XXH3_f_scrambleAcc f_scramble,
                                                           XXH3_f_initCustomSecret f_initSecret) {
    if (seed == 0) {
      return HashLong128bInternal(input, length, kSecret, XXH_SECRET_DEFAULT_SIZE, f_acc512, f_scramble);
    }
    __declspec(align(16)) unsigned char secret[XXH_SECRET_DEFAULT_SIZE];
    f_initSecret(secret, seed);
    return HashLong128bInternal(input, length, secret, sizeof(secret), f_acc512, f_scramble);
  }

  // INLINED
  __forceinline std::uint64_t Hash64bitsInternal(const void* input, std::size_t length, std::uint64_t seed,
                                                 const void* secret, std::size_t secretSize,
                                                 XXH3_hashLong64_f f_hashLong) {
    if (length <= 16) {
      return Len0to16_64b(static_cast<const std::uint8_t*>(input), length, static_cast<const std::uint8_t*>(secret),
                          seed);
    }
    if (length <= 128) {
      return Len17to128_64b(static_cast<const std::uint8_t*>(input), length,
                            static_cast<const std::uint8_t*>(secret), seed);
    }
    if (length <= XXH3_MIDSIZE_MAX) {
      return XXH3_len_129to240_64b(input, length, secret, secretSize, seed);
    }
    return f_hashLong(input, length, seed, secret, secretSize);
  }

  // INLINED
  __forceinline XXH128_hash_t Hash128bitsInternal(const void* input, std::size_t length, std::uint64_t seed,
                                                  const void* secret, std::size_t secretSize,
                                                  XXH3_hashLong128_f f_hashLong) {
    if (length <= 16) {
      return Len0to16_128b(static_cast<const std::uint8_t*>(input), length, static_cast<const std::uint8_t*>(secret),
                           seed);
    }
    if (length <= 128) {
      return Len17to128_128b(static_cast<const std::uint8_t*>(input), length,
                             static_cast<const std::uint8_t*>(secret), seed);
    }
    if (length <= XXH3_MIDSIZE_MAX) {
      return XXH3_len_129to240_128b(input, length, secret, secretSize, seed);
    }
    return f_hashLong(input, length, seed, secret, secretSize);
  }
}

// GLOBAL: gl_24486
const unsigned char kSecret[192] = {
    0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe, 0x7c, 0x01, 0x81, 0x2c, 0xf7, 0x21, 0xad, 0x1c,
    0xde, 0xd4, 0x6d, 0xe9, 0x83, 0x90, 0x97, 0xdb, 0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f,
    0xcb, 0x79, 0xe6, 0x4e, 0xcc, 0xc0, 0xe5, 0x78, 0x82, 0x5a, 0xd0, 0x7d, 0xcc, 0xff, 0x72, 0x21,
    0xb8, 0x08, 0x46, 0x74, 0xf7, 0x43, 0x24, 0x8e, 0xe0, 0x35, 0x90, 0xe6, 0x81, 0x3a, 0x26, 0x4c,
    0x3c, 0x28, 0x52, 0xbb, 0x91, 0xc3, 0x00, 0xcb, 0x88, 0xd0, 0x65, 0x8b, 0x1b, 0x53, 0x2e, 0xa3,
    0x71, 0x64, 0x48, 0x97, 0xa2, 0x0d, 0xf9, 0x4e, 0x38, 0x19, 0xef, 0x46, 0xa9, 0xde, 0xac, 0xd8,
    0xa8, 0xfa, 0x76, 0x3f, 0xe3, 0x9c, 0x34, 0x3f, 0xf9, 0xdc, 0xbb, 0xc7, 0xc7, 0x0b, 0x4f, 0x1d,
    0x8a, 0x51, 0xe0, 0x4b, 0xcd, 0xb4, 0x59, 0x31, 0xc8, 0x9f, 0x7e, 0xc9, 0xd9, 0x78, 0x73, 0x64,
    0xea, 0xc5, 0xac, 0x83, 0x34, 0xd3, 0xeb, 0xc3, 0xc5, 0x81, 0xa0, 0xff, 0xfa, 0x13, 0x63, 0xeb,
    0x17, 0x0d, 0xdd, 0x51, 0xb7, 0xf0, 0xda, 0x49, 0xd3, 0x16, 0x55, 0x26, 0x29, 0xd4, 0x68, 0x9e,
    0x2b, 0x16, 0xbe, 0x58, 0x7d, 0x47, 0xa1, 0xfc, 0x8f, 0xf8, 0xb8, 0xd1, 0x7a, 0xd0, 0x31, 0xce,
    0x45, 0xcb, 0x3a, 0x8f, 0x95, 0x16, 0x04, 0x28, 0xaf, 0xd7, 0xfb, 0xca, 0xbb, 0x4b, 0x40, 0x7e,
};

// OFFSET: 0x004DF8E0
// FUNCTION: fn_00182
std::uint32_t XXH_versionNumber() { return XXH_VERSION_NUMBER; }

// OFFSET: 0x004DF120

// OFFSET: 0x004DF670

// OFFSET: 0x004DF400

// OFFSET: 0x004DEC70

// OFFSET: 0x004DEAB0

// OFFSET: 0x004DE830

// OFFSET: 0x004DE270

// OFFSET: 0x004DEBA0

// OFFSET: 0x004DD1F0

// OFFSET: 0x004DDA00

// OFFSET: 0x004DC090

// OFFSET: 0x004DC930

// OFFSET: 0x004D84B0

// OFFSET: 0x004D9F30

// OFFSET: 0x004DB600

// OFFSET: 0x004D3760

// OFFSET: 0x004D56F0

// OFFSET: 0x004D7930
