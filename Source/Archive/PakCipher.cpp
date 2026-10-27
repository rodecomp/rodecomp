#include "Archive/PakCipher.h"

#include <algorithm>
#include <cstring>

namespace myGPack {
  namespace {
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

    // INLINED
    void Permute(std::uint8_t* block, const std::uint8_t* table, unsigned count) {
      std::uint8_t result[8] = {};

      for (unsigned i = 0; i < count; ++i) {
        unsigned bit = table[i] - 1;
        if (block[bit >> 3] & (0x80u >> (bit & 7))) {
          result[i >> 3] |= static_cast<std::uint8_t>(0x80u >> (i & 7));
        }
      }

      std::memcpy(block, result, sizeof(result));
    }

    // INLINED
    std::uint8_t SubstituteLastByte(std::uint8_t value) {
      switch (value) {
      case 0x00:
        return 0x2B;
      case 0x01:
        return 0x68;
      case 0x2B:
        return 0x00;
      case 0x48:
        return 0x77;
      case 0x60:
        return 0xFF;
      case 0x68:
        return 0x01;
      case 0x6C:
        return 0x80;
      case 0x77:
        return 0x48;
      case 0x80:
        return 0x6C;
      case 0xB9:
        return 0xC0;
      case 0xC0:
        return 0xB9;
      case 0xEB:
        return 0xFE;
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
  // OFFSET: 0x00AE92F0
  // FUNCTION: fn_18096
  PakCipher::PakCipher(const std::uint8_t* key) {
    (void)key;
    std::memset(this->m_roundKeys, 0, sizeof(this->m_roundKeys));
  }

  // OFFSET: 0x00AE8490
  // FUNCTION: fn_18098
  PakCipher::~PakCipher() {
  }

  // OFFSET: 0x00AE8E70
  // FUNCTION: fn_18101
  int PakCipher::DecodeBlock(int rounds, const std::uint8_t* input, std::uint8_t* output) const {
    std::uint32_t inputLow;
    std::uint32_t inputHigh;
    std::memcpy(&inputLow, input, sizeof(inputLow));
    std::memcpy(&inputHigh, input + 4, sizeof(inputHigh));
    std::memcpy(output, &inputLow, sizeof(inputLow));
    std::memcpy(output + 4, &inputHigh, sizeof(inputHigh));
    Permute(output, s_initialPermutation, 64);

    for (int round = 0; round < rounds; ++round) {
      std::uint8_t expanded[8] = {};

      for (unsigned i = 0; i < 48; ++i) {
        unsigned bit = s_expansion[i] + 31;
        if (output[bit >> 3] & (0x80u >> (bit & 7))) {
          expanded[i / 6] |= static_cast<std::uint8_t>(0x80u >> (i % 6));
        }
      }

      for (unsigned i = 0; i < 8; ++i) {
        expanded[i] ^= this->m_roundKeys[15 - round][i];
      }

      std::uint8_t substituted[4] = {};

      for (unsigned i = 0; i < 8; ++i) {
        std::uint8_t value = s_substitution[64 * i + (expanded[i] >> 2)];
        if (i & 1) {
          substituted[i >> 1] += value;
        } else {
          substituted[i >> 1] = static_cast<std::uint8_t>(value << 4);
        }
      }

      std::uint8_t transformed[4] = {};

      for (unsigned i = 0; i < 32; ++i) {
        unsigned bit = s_roundPermutation[i] - 1;
        if (substituted[bit >> 3] & (0x80u >> (bit & 7))) {
          transformed[i >> 3] |= static_cast<std::uint8_t>(0x80u >> (i & 7));
        }
      }

      std::uint8_t previous[4];
      std::memcpy(previous, output, 4);
      std::memcpy(output, output + 4, 4);

      for (unsigned i = 0; i < 4; ++i) {
        output[i + 4] = transformed[i] ^ previous[i];
      }
    }

    for (unsigned i = 0; i < 4; ++i) {
      std::swap(output[i], output[i + 4]);
    }

    Permute(output, s_finalPermutation, 64);
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
      if (block >= 20 && block % cycle != 0) {
        if (shuffled == 7) {
          std::uint16_t pair;
          std::memcpy(&pair, input + offset + 3, 2);
          std::memcpy(output + offset, &pair, 2);
          std::memcpy(&pair, input + offset, 2);
          std::memcpy(output + offset + 3, &pair, 2);
          output[offset + 5] = input[offset + 2];
          output[offset + 2] = input[offset + 6];
          output[offset + 6] = input[offset + 5];
          output[offset + 7] = SubstituteLastByte(input[offset + 7]);
          shuffled = 1;
        } else {
          std::uint32_t low;
          std::uint32_t high;
          std::memcpy(&low, input + offset, sizeof(low));
          std::memcpy(&high, input + offset + 4, sizeof(high));
          std::memcpy(output + offset, &low, sizeof(low));
          std::memcpy(output + offset + 4, &high, sizeof(high));
          ++shuffled;
        }
      } else {
        std::uint8_t blockInput[8];
        std::uint8_t blockOutput[8];
        std::memcpy(blockInput, input + offset, 8);
        this->DecodeBlock(1, blockInput, blockOutput);
        std::memcpy(output + offset, blockOutput, 8);
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
    int offset = 0;
    int block = 0;

    do {
      std::uint8_t blockInput[8];
      std::uint8_t decoded[8];
      std::memcpy(blockInput, input + offset, 8);
      this->DecodeBlock(1, blockInput, decoded);
      std::memcpy(output + offset, decoded, 8);
      offset += 8;
      ++block;
    } while (offset < length && block < 20);

    return 1;
  }

  // OFFSET: 0x00AE9260
  // FUNCTION: fn_18104
  int InitialPermutation(std::uint8_t* block) {
    std::uint8_t result[8] = {};

    for (unsigned i = 0; i < 64; ++i) {
      unsigned bit = s_initialPermutation[i] - 1;
      if (block[bit >> 3] & (0x80u >> (bit & 7))) {
        result[i >> 3] |= static_cast<std::uint8_t>(0x80u >> (i & 7));
      }
    }

    std::memcpy(block, result, 8);
    int high;
    std::memcpy(&high, result + 4, sizeof(high));

    return high;
  }

  // OFFSET: 0x00AE91D0
  // FUNCTION: fn_18103
  int FinalPermutation(std::uint8_t* block) {
    std::uint8_t result[8] = {};

    for (unsigned i = 0; i < 64; ++i) {
      unsigned bit = s_finalPermutation[i] - 1;
      if (block[bit >> 3] & (0x80u >> (bit & 7))) {
        result[i >> 3] |= static_cast<std::uint8_t>(0x80u >> (i & 7));
      }
    }

    std::memcpy(block, result, 8);
    int high;
    std::memcpy(&high, result + 4, sizeof(high));

    return high;
  }

  // OFFSET: 0x00AE9020
  // FUNCTION: fn_18102
  int EncodeBlock(int rounds, const std::uint8_t (*roundKeys)[8], std::uint32_t inputLow, std::uint32_t inputHigh, std::uint8_t* output) {
    std::memcpy(output, &inputLow, 4);
    std::memcpy(output + 4, &inputHigh, 4);
    InitialPermutation(output);

    if (rounds > 0) {
      std::uint32_t left;
      std::uint32_t right;
      std::memcpy(&left, output, sizeof(left));
      std::memcpy(&right, output + 4, sizeof(right));

      for (int round = 0; round < rounds; ++round) {
        std::uint8_t expanded[8] = {};

        for (unsigned i = 0; i < 48; ++i) {
          unsigned bit = s_expansion[i] + 31;
          if (output[bit >> 3] & (0x80u >> (bit & 7))) {
            expanded[i / 6] |= static_cast<std::uint8_t>(0x80u >> (i % 6));
          }
        }

        for (unsigned i = 0; i < 8; ++i) {
          expanded[i] ^= roundKeys[round][i];
        }

        std::uint8_t substituted[4] = {};

        for (unsigned i = 0; i < 8; ++i) {
          std::uint8_t value = s_substitution[64 * i + (expanded[i] >> 2)];
          if (i & 1) {
            substituted[i >> 1] += value;
          } else {
            substituted[i >> 1] = static_cast<std::uint8_t>(value << 4);
          }
        }

        std::uint8_t transformed[4] = {};

        for (unsigned i = 0; i < 32; ++i) {
          unsigned bit = s_roundPermutation[i] - 1;
          if (substituted[bit >> 3] & (0x80u >> (bit & 7))) {
            transformed[i >> 3] |= static_cast<std::uint8_t>(0x80u >> (i & 7));
          }
        }

        std::uint32_t transformedWord;
        std::memcpy(&transformedWord, transformed, sizeof(transformedWord));
        std::uint32_t newRight = left ^ transformedWord;
        left = right;
        right = newRight;
        std::memcpy(output, &left, sizeof(left));
        std::memcpy(output + 4, &right, sizeof(right));
      }

      std::memcpy(output, &right, sizeof(right));
      std::memcpy(output + 4, &left, sizeof(left));
    } else {
      std::uint32_t left;
      std::uint32_t right;
      std::memcpy(&left, output, sizeof(left));
      std::memcpy(&right, output + 4, sizeof(right));
      std::memcpy(output, &right, sizeof(right));
      std::memcpy(output + 4, &left, sizeof(left));
    }

    FinalPermutation(output);
    return 1;
  }

  // OFFSET: 0x00AEBC90
  // FUNCTION: fn_18131
  std::uint8_t* DerivePakKey(const char* name, std::uint8_t* key) {
    const char* base = name;
    const std::size_t length = std::strlen(name);
    const char* const end = name + length;

    for (const char* cursor = name; cursor < end; ++cursor) {
      if (*cursor == '\\' || *cursor == '/') {
        base = cursor + 1;
      }
    }

    const std::size_t baseLength = static_cast<std::size_t>(end - base);

    key[0] = 'n';
    key[7] = 'k';

    if (baseLength >= 6) {
      std::memcpy(&key[1], base, 6);
    } else {
      std::memcpy(&key[1], base, baseLength);
      std::memcpy(&key[1 + baseLength], base, 6 - baseLength);
    }

    return key;
  }
}
