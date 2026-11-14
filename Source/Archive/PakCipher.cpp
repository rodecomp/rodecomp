#include "Archive/PakCipher.h"

#include "Archive/GPakUpdate.h"

#include <algorithm>
#include <cstring>

namespace myGPack {
  namespace {
    // GLOBAL: gl_24709
    constexpr std::uint8_t s_bitMasks[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

    // STATIC: 0x01159EC8
    // GLOBAL: gl_13428
    constexpr std::uint8_t s_initialPermutation[64] = {
      58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
      57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7,
    };

    // STATIC: 0x01159F08
    // GLOBAL: gl_13429
    constexpr std::uint8_t s_finalPermutation[64] = {
      40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
      36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9,  49, 17, 57, 25,
    };

    // GLOBAL: gl_24710
    constexpr std::uint8_t s_keyRotations[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    // GLOBAL: gl_24711
    constexpr std::uint8_t s_keyPermutationTop[28] = {
      57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 28,
    };

    // GLOBAL: gl_24712
    constexpr std::uint8_t s_keyPermutationBottom[28] = {
      63, 55, 47, 39, 31, 23, 15, 7,  62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4,
    };

    // GLOBAL: gl_24713
    constexpr std::uint8_t s_keyCompression[48] = {
      14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10, 23, 19, 12, 4,  26, 8,  16, 7,  27, 20, 13, 2,
      41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32,
    };

    // STATIC: 0x01159FC0
    // GLOBAL: gl_13430
    constexpr std::uint8_t s_expansion[48] = {
      32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,  8,  9,  10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
      16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1,
    };

    // STATIC: 0x01159FF0
    // GLOBAL: gl_13431
    constexpr std::uint8_t s_roundPermutation[32] = {
      16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25,
    };

    // STATIC: 0x0115A010
    // GLOBAL: gl_13432
    constexpr std::uint8_t s_substitution[512] = {
      14, 0,  4,  15, 13, 7,  1,  4,  2,  14, 15, 2,  11, 13, 8,  1,  3,  10, 10, 6,  6,  12, 12, 11, 5,  9,  9,  5,  0,  3,  7,  8,  4,  15, 1,  12, 14,
      8,  8,  2,  13, 4,  6,  9,  2,  1,  11, 7,  15, 5,  12, 11, 9,  3,  7,  14, 3,  10, 10, 0,  5,  6,  0,  13, 15, 3,  1,  13, 8,  4,  14, 7,  6,  15,
      11, 2,  3,  8,  4,  14, 9,  12, 7,  0,  2,  1,  13, 10, 12, 6,  0,  9,  5,  11, 10, 5,  0,  13, 14, 8,  7,  10, 11, 1,  10, 3,  4,  15, 13, 4,  1,
      2,  5,  11, 8,  6,  12, 7,  6,  12, 9,  0,  3,  5,  2,  14, 15, 9,  10, 13, 0,  7,  9,  0,  14, 9,  6,  3,  3,  4,  15, 6,  5,  10, 1,  2,  13, 8,
      12, 5,  7,  14, 11, 12, 4,  11, 2,  15, 8,  1,  13, 1,  6,  10, 4,  13, 9,  0,  8,  6,  15, 9,  3,  8,  0,  7,  11, 4,  1,  15, 2,  14, 12, 3,  5,
      11, 10, 5,  14, 2,  7,  12, 7,  13, 13, 8,  14, 11, 3,  5,  0,  6,  6,  15, 9,  0,  10, 3,  1,  4,  2,  7,  8,  2,  5,  12, 11, 1,  12, 10, 4,  14,
      15, 9,  10, 3,  6,  15, 9,  0,  0,  6,  12, 10, 11, 1,  7,  13, 13, 8,  15, 9,  1,  4,  3,  5,  14, 11, 5,  12, 2,  7,  8,  2,  4,  14, 2,  14, 12,
      11, 4,  2,  1,  12, 7,  4,  10, 7,  11, 13, 6,  1,  8,  5,  5,  0,  3,  15, 15, 10, 13, 3,  0,  9,  14, 8,  9,  6,  4,  11, 2,  8,  1,  12, 11, 7,
      10, 1,  13, 14, 7,  2,  8,  13, 15, 6,  9,  15, 12, 0,  5,  9,  6,  10, 3,  4,  0,  5,  14, 3,  12, 10, 1,  15, 10, 4,  15, 2,  9,  7,  2,  12, 6,
      9,  8,  5,  0,  6,  13, 1,  3,  13, 4,  14, 14, 0,  7,  11, 5,  3,  11, 8,  9,  4,  14, 3,  15, 2,  5,  12, 2,  9,  8,  5,  12, 15, 3,  10, 7,  11,
      0,  14, 4,  1,  10, 7,  1,  6,  13, 0,  11, 8,  6,  13, 4,  13, 11, 0,  2,  11, 14, 7,  15, 4,  0,  9,  8,  1,  13, 10, 3,  14, 12, 3,  9,  5,  7,
      12, 5,  2,  10, 15, 6,  8,  1,  6,  1,  6,  4,  11, 11, 13, 13, 8,  12, 1,  3,  4,  7,  10, 14, 7,  10, 9,  15, 5,  6,  0,  8,  15, 0,  14, 5,  2,
      9,  3,  2,  12, 13, 1,  2,  15, 8,  13, 4,  8,  6,  10, 15, 3,  11, 7,  1,  4,  10, 12, 9,  5,  3,  6,  14, 11, 5,  0,  0,  14, 12, 9,  7,  2,  7,
      2,  11, 1,  4,  14, 1,  7,  9,  4,  12, 10, 14, 8,  2,  13, 0,  15, 6,  12, 10, 9,  13, 0,  15, 3,  3,  5,  5,  6,  8,  11,
    };

    union Block64 {
      std::uint8_t bytes[8];
      std::uint32_t words[2];
    };

    // INLINED
    std::uint8_t SubstituteLastByte(std::uint8_t value) {
      switch (value) {
      case 0x00:
        return 0x2B;
      case 0x2B:
        return 0x00;
      case 0xB9:
        return 0xC0;
      case 0xC0:
        return 0xB9;
      case 0xEB:
        return 0xFE;
      case 0x6C:
        return 0x80;
      case 0x80:
        return 0x6C;
      case 0x01:
        return 0x68;
      case 0x68:
        return 0x01;
      case 0x77:
        return 0x48;
      case 0x48:
        return 0x77;
      case 0x60:
        return 0xFF;
      case 0xFE:
        return 0xEB;
      case 0xFF:
        return 0x60;
      default:
        return value;
      }
    }
  }

  // OFFSET: 0x00AE8480
  // FUNCTION: fn_18097
  PakCipher::PakCipher() {
  }

  // OFFSET: 0x00AE8460
  // FUNCTION: fn_18096
  PakCipher::PakCipher(const std::uint8_t* key) {
    this->BuildRoundKeys(key);
  }

  // OFFSET: 0x00AE92F0
  // FUNCTION: fn_18105
  void PakCipher::BuildRoundKeys(const std::uint8_t* key) {
    std::uint8_t keyBytes[8];
    std::memcpy(keyBytes, key, sizeof(keyBytes));
    std::uint8_t halves[8] = {};

    for (unsigned i = 0; i < 28; ++i) {
      const std::uint8_t topBit = s_keyPermutationTop[i] - 1;
      if (keyBytes[topBit >> 3] & s_bitMasks[topBit & 7]) {
        halves[i >> 3] &= s_bitMasks[i & 7];
      }

      const std::uint8_t bottomBit = s_keyPermutationBottom[i] - 1;
      if (keyBytes[bottomBit >> 3] & s_bitMasks[bottomBit & 7]) {
        halves[i >> 7] &= s_bitMasks[i & 7];
      }
    }

    for (unsigned round = 0; round < 16; ++round) {
      for (std::uint8_t shift = 0; shift < s_keyRotations[round]; ++shift) {
        const std::uint8_t topCarry = halves[0] & 0x80;

        for (unsigned byte = 0; byte < 3; ++byte) {
          halves[byte] <<= 1;
          if (halves[byte + 1] & 0x80) {
            ++halves[byte];
          }
        }

        halves[3] <<= 1;
        if (topCarry) {
          halves[3] += 0x10;
        }

        const std::uint8_t bottomCarry = halves[4] & 0x80;

        for (unsigned byte = 4; byte < 7; ++byte) {
          halves[byte] <<= 1;
          if (halves[byte + 1] & 0x80) {
            ++halves[byte];
          }
        }

        halves[7] <<= 1;
        if (bottomCarry) {
          halves[7] += 0x10;
        }
      }

      std::memset(this->m_roundKeys[round], 0, sizeof(this->m_roundKeys[round]));

      unsigned bitsPerGroup = 6;
      for (unsigned i = 0; i < 48; ++i) {
        std::uint8_t bit = s_keyCompression[i] - 1;
        unsigned byte;
        if (bit < 28) {
          byte = bit >> 3;
        } else {
          bit -= 28;
          byte = bit >> 7;
        }

        if (halves[byte] & s_bitMasks[bit & 7]) {
          this->m_roundKeys[round][i / bitsPerGroup] |= s_bitMasks[i % bitsPerGroup];
        }
      }
    }
  }

  // OFFSET: 0x00AE8490
  // FUNCTION: fn_18098
  PakCipher::~PakCipher() {
  }

  // OFFSET: 0x00AE8E70
  // FUNCTION: fn_18101
  int PakCipher::DecodeBlock(int rounds, const std::uint8_t (*roundKeys)[8], std::uint32_t inputLow, std::uint32_t inputHigh, std::uint8_t* output) const {
    std::memcpy(output, &inputLow, sizeof(inputLow));
    std::memcpy(output + 4, &inputHigh, sizeof(inputHigh));
    InitialPermutation(output);

    for (std::uint8_t round = 0; round < rounds; ++round) {
      Block64 key;
      std::memcpy(&key, roundKeys[15 - round], sizeof(key));

      Block64 permuted;
      permuted.words[0] = permuted.words[1] = 0;

      for (std::uint8_t i = 0; i < 48; ++i) {
        const std::uint8_t bit = s_expansion[i] + 31;
        if (output[bit >> 3] & s_bitMasks[bit & 7]) {
          permuted.bytes[i / 6] |= s_bitMasks[i % 6];
        }
      }

      for (std::uint8_t i = 0; i < 8; ++i) {
        permuted.bytes[i] ^= key.bytes[i];
      }

      Block64 substituted;
      substituted.words[0] = substituted.words[1] = 0;

      for (std::uint8_t i = 0; i < 8; ++i) {
        if (i % 2 == 0) {
          substituted.bytes[i / 2] = static_cast<std::uint8_t>(s_substitution[64 * i + (permuted.bytes[i] >> 2)] << 4);
        } else {
          substituted.bytes[i / 2] += s_substitution[64 * i + (permuted.bytes[i] >> 2)];
        }
      }

      permuted.words[0] = permuted.words[1] = 0;

      for (std::uint8_t i = 0; i < 32; ++i) {
        const std::uint8_t bit = s_roundPermutation[i] - 1;
        if (substituted.bytes[bit >> 3] & s_bitMasks[bit & 7]) {
          permuted.bytes[i >> 3] |= s_bitMasks[i & 7];
        }
      }

      std::uint32_t left;
      std::uint32_t right;
      std::memcpy(&left, output, sizeof(left));
      std::memcpy(&right, output + 4, sizeof(right));
      std::memcpy(output, &right, sizeof(right));
      left ^= permuted.words[0];
      std::memcpy(output + 4, &left, sizeof(left));
    }

    std::uint32_t low;
    std::uint32_t high;
    std::memcpy(&low, output, sizeof(low));
    std::memcpy(&high, output + 4, sizeof(high));
    std::memcpy(output + 4, &low, sizeof(low));
    std::memcpy(output, &high, sizeof(high));

    FinalPermutation(output);
    return 1;
  }

  // OFFSET: 0x00AE84A0
  // FUNCTION: fn_18099
  int PakCipher::Decode(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength, int cycle) const {
    if (!input || !output || !outputLength) {
      return 0;
    }

    *outputLength = length;
    if (cycle < 3) {
      cycle = 1;
    } else if (cycle < 5) {
      cycle += 1;
    } else if (cycle < 7) {
      cycle += 9;
    } else {
      cycle += 15;
    }

    int shuffled = 0;
    int block = 0;
    int offset = 0;

    do {
      if (block < 20 || block % cycle == 0) {
        std::uint8_t blockInput[8];
        std::uint8_t blockOutput[8];
        for (int i = 0; i < 8; ++i) {
          blockInput[i] = input[offset + i];
        }

        std::uint32_t low;
        std::uint32_t high;
        std::memcpy(&low, blockInput, sizeof(low));
        std::memcpy(&high, blockInput + 4, sizeof(high));
        this->DecodeBlock(1, this->m_roundKeys, low, high, blockOutput);
        for (int i = 0; i < 8; ++i) {
          output[offset + i] = blockOutput[i];
        }
      } else {
        if (shuffled != 7) {
          std::uint32_t low;
          std::uint32_t high;
          std::memcpy(&low, input + offset, sizeof(low));
          std::memcpy(&high, input + offset + 4, sizeof(high));
          std::memcpy(output + offset, &low, sizeof(low));
          std::memcpy(output + offset + 4, &high, sizeof(high));
        } else {
          std::uint16_t pair;
          std::memcpy(&pair, input + offset + 3, 2);
          std::memcpy(output + offset, &pair, 2);
          std::memcpy(&pair, input + offset, 2);
          std::memcpy(output + offset + 3, &pair, 2);
          output[offset + 5] = input[offset + 2];
          output[offset + 2] = input[offset + 6];
          output[offset + 6] = input[offset + 5];
          output[offset + 7] = SubstituteLastByte(input[offset + 7]);
          shuffled = 0;
        }

        ++shuffled;
      }

      offset += 8;
      ++block;
    } while (offset < length);

    return 1;
  }

  // OFFSET: 0x00AE8870
  // FUNCTION: fn_18100
  int PakCipher::DecodePrefix(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength) const {
    *outputLength = length;
    std::memcpy(output, input, static_cast<std::uint32_t>(length));

    for (int block = 0; block < 20; ++block) {
      std::uint8_t blockInput[8];
      std::uint8_t decoded[8];
      for (int i = 0; i < 8; ++i) {
        blockInput[i] = input[8 * block + i];
      }
      std::uint32_t low;
      std::uint32_t high;
      std::memcpy(&low, blockInput, sizeof(low));
      std::memcpy(&high, blockInput + 4, sizeof(high));
      this->DecodeBlock(1, this->m_roundKeys, low, high, decoded);
      for (int i = 0; i < 8; ++i) {
        output[8 * block + i] = decoded[i];
      }
      if (8 * (block + 1) >= length) {
        break;
      }
    }

    return 1;
  }

  // OFFSET: 0x00AE9260
  // FUNCTION: fn_18104
  int __stdcall InitialPermutation(std::uint8_t* block) {
    Block64 result;
    result.words[0] = result.words[1] = 0;

    for (std::uint8_t i = 0; i < 64; ++i) {
      const std::uint8_t bit = s_initialPermutation[i] - 1;
      if (block[bit >> 3] & s_bitMasks[bit & 7]) {
        result.bytes[i >> 3] |= s_bitMasks[i & 7];
      }
    }

    std::memcpy(block, &result.words[0], sizeof(result.words[0]));
    std::memcpy(block + sizeof(result.words[0]), &result.words[1], sizeof(result.words[1]));
    return static_cast<int>(result.words[1]);
  }

  // OFFSET: 0x00AE91D0
  // FUNCTION: fn_18103
  int __stdcall FinalPermutation(std::uint8_t* block) {
    Block64 result;
    result.words[0] = result.words[1] = 0;

    for (std::uint8_t i = 0; i < 64; ++i) {
      const std::uint8_t bit = s_finalPermutation[i] - 1;
      if (block[bit >> 3] & s_bitMasks[bit & 7]) {
        result.bytes[i >> 3] |= s_bitMasks[i & 7];
      }
    }

    std::memcpy(block, &result.words[0], sizeof(result.words[0]));
    std::memcpy(block + sizeof(result.words[0]), &result.words[1], sizeof(result.words[1]));
    return static_cast<int>(result.words[1]);
  }

  // OFFSET: 0x00AE9020
  // FUNCTION: fn_18102
  int PakCipher::EncodeBlock(int rounds, const std::uint8_t (*roundKeys)[8], std::uint32_t inputLow, std::uint32_t inputHigh, std::uint8_t* output) const {
    std::memcpy(output, &inputLow, sizeof(inputLow));
    std::memcpy(output + 4, &inputHigh, sizeof(inputHigh));
    InitialPermutation(output);

    for (std::uint8_t round = 0; round < rounds; ++round) {
      Block64 key;
      std::memcpy(&key, roundKeys[round], sizeof(key));

      Block64 permuted;
      permuted.words[0] = permuted.words[1] = 0;

      for (std::uint8_t i = 0; i < 48; ++i) {
        const std::uint8_t bit = s_expansion[i] + 31;
        if (output[bit >> 3] & s_bitMasks[bit & 7]) {
          permuted.bytes[i / 6] |= s_bitMasks[i % 6];
        }
      }

      for (std::uint8_t i = 0; i < 8; ++i) {
        permuted.bytes[i] ^= key.bytes[i];
      }

      Block64 substituted;
      substituted.words[0] = substituted.words[1] = 0;

      for (std::uint8_t i = 0; i < 8; ++i) {
        if (i % 2 == 0) {
          substituted.bytes[i / 2] = static_cast<std::uint8_t>(s_substitution[64 * i + (permuted.bytes[i] >> 2)] << 4);
        } else {
          substituted.bytes[i / 2] += s_substitution[64 * i + (permuted.bytes[i] >> 2)];
        }
      }

      permuted.words[0] = permuted.words[1] = 0;

      for (std::uint8_t i = 0; i < 32; ++i) {
        const std::uint8_t bit = s_roundPermutation[i] - 1;
        if (substituted.bytes[bit >> 3] & s_bitMasks[bit & 7]) {
          permuted.bytes[i >> 3] |= s_bitMasks[i & 7];
        }
      }

      std::uint32_t left;
      std::uint32_t right;
      std::memcpy(&left, output, sizeof(left));
      std::memcpy(&right, output + 4, sizeof(right));
      std::memcpy(output, &right, sizeof(right));
      left ^= permuted.words[0];
      std::memcpy(output + 4, &left, sizeof(left));
    }

    std::uint32_t low;
    std::uint32_t high;
    std::memcpy(&low, output, sizeof(low));
    std::memcpy(&high, output + 4, sizeof(high));
    std::memcpy(output, &high, sizeof(high));
    std::memcpy(output + 4, &low, sizeof(low));

    FinalPermutation(output);
    return 1;
  }

  // OFFSET: 0x00AEBC90
  // FUNCTION: fn_18131
  std::uint8_t* CGPakRead::DerivePakKey(const char* name, std::uint8_t* key) {
    const char* base = name;
    const std::size_t length = std::strlen(name);
    const char* const end = name + length;

    for (const char* cursor = name; cursor < end; ++cursor) {
      if (*cursor == '\\' || *cursor == '/') {
        base = cursor + 1;
      }
    }

    const int baseLength = static_cast<int>(end - base);

    key[0] = 'n';
    key[7] = 'k';

    if (baseLength < 6) {
      std::memcpy(&key[1], base, baseLength);
      std::memcpy(&key[1 + baseLength], base, 6 - baseLength);
    } else {
      std::memcpy(&key[1], base, 6);
    }

    return key;
  }
}
