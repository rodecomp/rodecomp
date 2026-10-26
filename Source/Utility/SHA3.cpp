#include "Utility/SHA3.h"

#include <cstring>
#include <utility>

// GLOBAL: gl_24554
const std::uint64_t sha3RoundConstants[24] = {
  0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808AULL, 0x8000000080008000ULL,
  0x000000000000808BULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
  0x000000000000008AULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000AULL,
  0x000000008000808BULL, 0x800000000000008BULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
  0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800AULL, 0x800000008000000AULL,
  0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

// STRING: st_16807
const char sha3HexDigits[] = "0123456789abcdef";

namespace {
  // INLINED
  std::uint64_t RotateLeft(std::uint64_t value, unsigned int bits) {
    return (value << bits) | (value >> (64 - bits));
  }
}

// FUNCTION: fn_04423
void SHA3::Add(const unsigned char* data, unsigned int length) {
  if (this->bufferedBytes != 0 && length != 0) {
    while (this->bufferedBytes < this->blockSize && length != 0) {
      this->buffer[this->bufferedBytes++] = *data++;
      --length;
    }
  }
  if (this->bufferedBytes == this->blockSize) {
    this->ProcessBlock(this->buffer);
    this->processedBytes += this->blockSize;
    this->bufferedBytes = 0;
  }

  if (length != 0) {
    while (length >= this->blockSize) {
      this->ProcessBlock(data);
      data += this->blockSize;
      this->processedBytes += this->blockSize;
      length -= this->blockSize;
    }
    while (length != 0) {
      this->buffer[this->bufferedBytes++] = *data++;
      --length;
    }
  }
}

// FUNCTION: fn_04424
std::string SHA3::GetHash() {
  std::uint64_t savedState[25];
  std::memcpy(savedState, this->state, sizeof(savedState));
  unsigned int end = this->bufferedBytes;
  this->buffer[end++] = 6;
  while (end < this->blockSize) {
    this->buffer[end++] = 0;
  }
  this->buffer[end - 1] |= 0x80;
  this->ProcessBlock(this->buffer);

  std::string result;
  result.reserve(this->digestBits / 4);
  const unsigned int wholeLanes = this->digestBits / 64;
  for (unsigned int lane = 0; lane < wholeLanes; ++lane) {
    for (unsigned int byte = 0; byte < 8; ++byte) {
      const unsigned char value = static_cast<unsigned char>(this->state[lane] >> (8 * byte));
      result.push_back(sha3HexDigits[value >> 4]);
      result.push_back(sha3HexDigits[value & 15]);
    }
  }
  const unsigned int remainingBits = this->digestBits - (wholeLanes << 6);
  for (unsigned int shift = 0; shift < remainingBits; shift += 8) {
    const unsigned char value = static_cast<unsigned char>(this->state[wholeLanes] >> shift);
    result.push_back(sha3HexDigits[value >> 4]);
    result.push_back(sha3HexDigits[value & 15]);
  }

  std::memcpy(this->state, savedState, sizeof(savedState));
  return result;
}

// FUNCTION: fn_04425
void SHA3::ProcessBlock(const unsigned char* data) {
  for (unsigned int lane = 0; lane < this->blockSize / 8; ++lane) {
    std::uint64_t value;
    std::memcpy(&value, data + lane * 8, sizeof(value));
    this->state[lane] ^= value;
  }

  for (unsigned int round = 0; round < 24; ++round) {
    std::uint64_t columns[5];
    for (unsigned int column = 0; column < 5; ++column) {
      columns[column] = this->state[column] ^ this->state[column + 5] ^ this->state[column + 10] ^
        this->state[column + 15] ^ this->state[column + 20];
    }
    for (unsigned int column = 0; column < 5; ++column) {
      const std::uint64_t delta = columns[(column + 4) % 5] ^ RotateLeft(columns[(column + 1) % 5], 1);
      for (unsigned int row = 0; row < 25; row += 5) {
        this->state[row + column] ^= delta;
      }
    }

    std::uint64_t current = this->state[1];
    std::swap(current, this->state[10]);
    this->state[10] = RotateLeft(this->state[10], 1);
    std::swap(current, this->state[7]);
    this->state[7] = RotateLeft(this->state[7], 3);
    std::swap(current, this->state[11]);
    this->state[11] = RotateLeft(this->state[11], 6);
    std::swap(current, this->state[17]);
    this->state[17] = RotateLeft(this->state[17], 10);
    std::swap(current, this->state[18]);
    this->state[18] = RotateLeft(this->state[18], 15);
    std::swap(current, this->state[3]);
    this->state[3] = RotateLeft(this->state[3], 21);
    std::swap(current, this->state[5]);
    this->state[5] = RotateLeft(this->state[5], 28);
    std::swap(current, this->state[16]);
    this->state[16] = RotateLeft(this->state[16], 36);
    std::swap(current, this->state[8]);
    this->state[8] = RotateLeft(this->state[8], 45);
    std::swap(current, this->state[21]);
    this->state[21] = RotateLeft(this->state[21], 55);
    std::swap(current, this->state[24]);
    this->state[24] = RotateLeft(this->state[24], 2);
    std::swap(current, this->state[4]);
    this->state[4] = RotateLeft(this->state[4], 14);
    std::swap(current, this->state[15]);
    this->state[15] = RotateLeft(this->state[15], 27);
    std::swap(current, this->state[23]);
    this->state[23] = RotateLeft(this->state[23], 41);
    std::swap(current, this->state[19]);
    this->state[19] = RotateLeft(this->state[19], 56);
    std::swap(current, this->state[13]);
    this->state[13] = RotateLeft(this->state[13], 8);
    std::swap(current, this->state[12]);
    this->state[12] = RotateLeft(this->state[12], 25);
    std::swap(current, this->state[2]);
    this->state[2] = RotateLeft(this->state[2], 43);
    std::swap(current, this->state[20]);
    this->state[20] = RotateLeft(this->state[20], 62);
    std::swap(current, this->state[14]);
    this->state[14] = RotateLeft(this->state[14], 18);
    std::swap(current, this->state[22]);
    this->state[22] = RotateLeft(this->state[22], 39);
    std::swap(current, this->state[9]);
    this->state[9] = RotateLeft(this->state[9], 61);
    std::swap(current, this->state[6]);
    this->state[6] = RotateLeft(this->state[6], 20);
    this->state[1] = RotateLeft(current, 44);

    for (unsigned int row = 0; row < 25; row += 5) {
      const std::uint64_t first = this->state[row];
      const std::uint64_t second = this->state[row + 1];
      const std::uint64_t third = this->state[row + 2];
      const std::uint64_t fourth = this->state[row + 3];
      const std::uint64_t fifth = this->state[row + 4];
      this->state[row] = first ^ (~second & third);
      this->state[row + 1] = second ^ (~third & fourth);
      this->state[row + 2] = third ^ (~fourth & fifth);
      this->state[row + 3] = fourth ^ (~fifth & first);
      this->state[row + 4] = fifth ^ (~first & second);
    }
    this->state[0] ^= sha3RoundConstants[round];
  }
}
