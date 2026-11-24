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

    // INLINED
    void RoundFunction(Block64 key, Block64* text) {
      Block64 permuted;
      permuted.words[0] = permuted.words[1] = 0;

      for (std::uint8_t i = 0; i < 48; ++i) {
        const std::uint8_t bit = s_expansion[i] + 31;
        if (text->bytes[bit >> 3] & s_bitMasks[bit & 7]) {
          permuted.bytes[i / 6] |= s_bitMasks[i % 6];
        }
      }

      for (std::uint8_t i = 0; i < 8; ++i) {
        permuted.bytes[i] ^= key.bytes[i];
      }

      Block64 substituted;
      substituted.words[0] = substituted.words[1] = 0;

      for (std::uint8_t i = 0; i < 8; ++i) {
        if (0 == i % 2) {
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

      text->words[0] ^= permuted.words[0];
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
    Block64 keyBytes;
    std::memcpy(keyBytes.bytes, key, sizeof(keyBytes));
    Block64 halves;
    halves.words[0] = halves.words[1] = 0;

    for (std::uint8_t i = 0; i < 28; ++i) {
      const std::uint8_t topBit = s_keyPermutationTop[i] - 1;
      if (keyBytes.bytes[topBit >> 3] & s_bitMasks[topBit & 7]) {
        halves.bytes[i >> 3] &= s_bitMasks[i & 7];
      }

      const std::uint8_t bottomBit = s_keyPermutationBottom[i] - 1;
      if (keyBytes.bytes[bottomBit >> 3] & s_bitMasks[bottomBit & 7]) {
        halves.bytes[i >> 7] &= s_bitMasks[i & 7];
      }
    }

    for (std::uint8_t round = 0; round < 16; ++round) {
      for (std::uint8_t shift = 0; shift < s_keyRotations[round]; ++shift) {
        std::uint8_t carry = halves.bytes[0] & 0x80;

        halves.bytes[0] <<= 1;
        if (halves.bytes[1] & 0x80) {
          halves.bytes[0] += 1;
        }
        halves.bytes[1] <<= 1;
        if (halves.bytes[2] & 0x80) {
          halves.bytes[1] += 1;
        }
        halves.bytes[2] <<= 1;
        if (halves.bytes[3] & 0x80) {
          halves.bytes[2] += 1;
        }
        halves.bytes[3] <<= 1;
        if (carry) {
          halves.bytes[3] += 0x10;
        }

        carry = halves.bytes[4] & 0x80;

        halves.bytes[4] <<= 1;
        if (halves.bytes[5] & 0x80) {
          halves.bytes[4] += 1;
        }
        halves.bytes[5] <<= 1;
        if (halves.bytes[6] & 0x80) {
          halves.bytes[5] += 1;
        }
        halves.bytes[6] <<= 1;
        if (halves.bytes[7] & 0x80) {
          halves.bytes[6] += 1;
        }
        halves.bytes[7] <<= 1;
        if (carry) {
          halves.bytes[7] += 0x10;
        }
      }

      this->m_roundKeys[round].words[0] = 0;
      this->m_roundKeys[round].words[1] = 0;

      for (std::uint8_t i = 0; i < 48; ++i) {
        std::uint8_t bit = s_keyCompression[i] - 1;
        if (bit < 28) {
          if (halves.bytes[bit >> 3] & s_bitMasks[bit & 7]) {
            this->m_roundKeys[round].bytes[i / 6] |= s_bitMasks[i % 6];
          }
        } else {
          bit = bit - 28;
          if (halves.bytes[bit >> 7] & s_bitMasks[bit & 7]) {
            this->m_roundKeys[round].bytes[i / 6] |= s_bitMasks[i % 6];
          }
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
  int PakCipher::DecodeBlock(int rounds, const Block64* roundKeys, Block64 input, Block64* output) const {
    output->words[0] = input.words[0];
    output->words[1] = input.words[1];
    InitialPermutation(output->bytes);

    for (std::uint8_t round = 0; round < rounds; ++round) {
      RoundFunction(roundKeys[15 - round], output);

      const std::uint32_t left = output->words[0];
      output->words[0] = output->words[1];
      output->words[1] = left;
    }

    const std::uint32_t low = output->words[0];
    output->words[0] = output->words[1];
    output->words[1] = low;

    FinalPermutation(output->bytes);
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

    Block64 blockInput;
    Block64 blockOutput;
    int block = 0;
    int i = 0;
    int shuffled = 0;

    while (true) {
      if (block < 20 || block % cycle == 0) {
        for (i = 0; i < 8; ++i) {
          blockInput.bytes[i] = input[block * 8 + i];
        }

        this->DecodeBlock(1, this->m_roundKeys, blockInput, &blockOutput);
        for (i = 0; i < 8; ++i) {
          output[block * 8 + i] = blockOutput.bytes[i];
        }
      } else {
        if (shuffled != 7) {
          std::memcpy(output + block * 8, input + block * 8, 8);
        } else {
          std::memcpy(output + block * 8, input + 3 + block * 8, 2);
          std::memcpy(output + 3 + block * 8, input + block * 8, 3);
          output[2 + block * 8] = input[6 + block * 8];
          output[6 + block * 8] = input[5 + block * 8];
          std::uint8_t last = input[7 + block * 8];
          switch (last) {
          case 0:
            last = 43;
            break;
          case 43:
            last = 0;
            break;
          case 185:
            last = 192;
            break;
          case 192:
            last = 185;
            break;
          case 254:
            last = 235;
            break;
          case 235:
            last = 254;
            break;
          case 108:
            last = 128;
            break;
          case 128:
            last = 108;
            break;
          case 1:
            last = 104;
            break;
          case 104:
            last = 1;
            break;
          case 119:
            last = 72;
            break;
          case 72:
            last = 119;
            break;
          case 96:
            last = 255;
            break;
          case 255:
            last = 96;
            break;
          }

          output[7 + block * 8] = last;
          shuffled = 0;
        }

        ++shuffled;
      }

      if ((block + 1) * 8 >= length) {
        break;
      }

      ++block;
    }

    return 1;
  }

  // OFFSET: 0x00AE8870
  // FUNCTION: fn_18100
  int PakCipher::DecodePrefix(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength) const {
    *outputLength = length;

    std::memcpy(output, input, *outputLength);
    Block64 blockInput;
    Block64 decoded;
    int block = 0;
    unsigned int i = 0;

    while (block < 20) {
      for (i = 0; i < 8; ++i) {
        blockInput.bytes[i] = input[block * 8 + i];
      }

      this->DecodeBlock(1, this->m_roundKeys, blockInput, &decoded);
      for (i = 0; i < 8; ++i) {
        output[i + block * 8] = decoded.bytes[i];
      }

      if ((block + 1) * 8 >= length) {
        break;
      }

      ++block;
    }

    return 1;
  }

  // OFFSET: 0x00AE9260
  // FUNCTION: fn_18104
  int PakCipher::InitialPermutation(std::uint8_t* block) const {
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
  int PakCipher::FinalPermutation(std::uint8_t* block) const {
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
  int PakCipher::EncodeBlock(int rounds, const Block64* roundKeys, Block64 input, Block64* output) const {
    output->words[0] = input.words[0];
    output->words[1] = input.words[1];
    InitialPermutation(output->bytes);

    for (std::uint8_t round = 0; round < rounds; ++round) {
      Block64 key;
      std::memcpy(&key, &roundKeys[round], sizeof(key));

      Block64 permuted;
      permuted.words[0] = permuted.words[1] = 0;

      for (std::uint8_t i = 0; i < 48; ++i) {
        const std::uint8_t bit = s_expansion[i] + 31;
        if (output->bytes[bit >> 3] & s_bitMasks[bit & 7]) {
          permuted.bytes[i / 6] |= s_bitMasks[i % 6];
        }
      }

      for (std::uint8_t i = 0; i < 8; ++i) {
        permuted.bytes[i] ^= key.bytes[i];
      }

      Block64 substituted;
      substituted.words[0] = substituted.words[1] = 0;

      for (std::uint8_t i = 0; i < 8; ++i) {
        if (0 == i % 2) {
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

      const std::uint32_t left = output->words[0];
      output->words[0] = output->words[1];
      output->words[1] = left ^ permuted.words[0];
    }

    const std::uint32_t low = output->words[0];
    const std::uint32_t high = output->words[1];
    output->words[0] = high;
    output->words[1] = low;

    FinalPermutation(output->bytes);
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
