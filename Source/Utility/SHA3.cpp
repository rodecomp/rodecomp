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

  // INLINED
  unsigned int Mod5(unsigned int value) {
    if (value < 5) {
      return value;
    }

    return value - 5;
  }
}

// OFFSET: 0x0061B740
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

// OFFSET: 0x0061B820
// FUNCTION: fn_04424
std::string SHA3::GetHash() {
  std::uint64_t oldHash[25];
  for (unsigned int i = 0; i < 25; ++i) {
    oldHash[i] = this->state[i];
  }

  unsigned int offset = this->bufferedBytes;
  this->buffer[offset++] = 6;
  while (offset < this->blockSize) {
    this->buffer[offset++] = 0;
  }
  this->buffer[offset - 1] |= 0x80;
  this->ProcessBlock(this->buffer);

  std::string result;
  result.reserve(this->digestBits / 4);

  const unsigned int hashLength = this->digestBits / 64;
  for (unsigned int i = 0; i < hashLength; ++i) {
    for (unsigned int j = 0; j < 8; ++j) {
      const unsigned char oneByte = static_cast<unsigned char>(this->state[i] >> (8 * j));
      result += sha3HexDigits[oneByte >> 4];
      result += sha3HexDigits[oneByte & 15];
    }
  }

  const unsigned int remainder = this->digestBits - hashLength * 64;
  unsigned int processed = 0;
  while (processed < remainder) {
    const unsigned char oneByte = static_cast<unsigned char>(this->state[hashLength] >> processed);
    result += sha3HexDigits[oneByte >> 4];
    result += sha3HexDigits[oneByte & 15];
    processed += 8;
  }

  for (unsigned int i = 0; i < 25; ++i) {
    this->state[i] = oldHash[i];
  }

  return result;
}

// OFFSET: 0x0061C2C0
// FUNCTION: fn_04425
void SHA3::ProcessBlock(const unsigned char* data) {
  const std::uint64_t* data64 = reinterpret_cast<const std::uint64_t*>(data);
  for (unsigned int i = 0; i < this->blockSize / 8; ++i) {
    this->state[i] ^= data64[i];
  }

  for (unsigned int round = 0; round < 24; ++round) {
    std::uint64_t coefficients[5];
    for (unsigned int i = 0; i < 5; ++i) {
      coefficients[i] = this->state[i] ^ this->state[i + 5] ^ this->state[i + 10] ^ this->state[i + 15] ^ this->state[i + 20];
    }

    for (unsigned int i = 0; i < 5; ++i) {
      const std::uint64_t one = coefficients[Mod5(i + 4)] ^ RotateLeft(coefficients[Mod5(i + 1)], 1);
      this->state[i] ^= one;
      this->state[i + 5] ^= one;
      this->state[i + 10] ^= one;
      this->state[i + 15] ^= one;
      this->state[i + 20] ^= one;
    }

    std::uint64_t one;
    std::uint64_t last = this->state[1];
    one = this->state[10];
    this->state[10] = RotateLeft(last, 1);
    last = one;
    one = this->state[7];
    this->state[7] = RotateLeft(last, 3);
    last = one;
    one = this->state[11];
    this->state[11] = RotateLeft(last, 6);
    last = one;
    one = this->state[17];
    this->state[17] = RotateLeft(last, 10);
    last = one;
    one = this->state[18];
    this->state[18] = RotateLeft(last, 15);
    last = one;
    one = this->state[3];
    this->state[3] = RotateLeft(last, 21);
    last = one;
    one = this->state[5];
    this->state[5] = RotateLeft(last, 28);
    last = one;
    one = this->state[16];
    this->state[16] = RotateLeft(last, 36);
    last = one;
    one = this->state[8];
    this->state[8] = RotateLeft(last, 45);
    last = one;
    one = this->state[21];
    this->state[21] = RotateLeft(last, 55);
    last = one;
    one = this->state[24];
    this->state[24] = RotateLeft(last, 2);
    last = one;
    one = this->state[4];
    this->state[4] = RotateLeft(last, 14);
    last = one;
    one = this->state[15];
    this->state[15] = RotateLeft(last, 27);
    last = one;
    one = this->state[23];
    this->state[23] = RotateLeft(last, 41);
    last = one;
    one = this->state[19];
    this->state[19] = RotateLeft(last, 56);
    last = one;
    one = this->state[13];
    this->state[13] = RotateLeft(last, 8);
    last = one;
    one = this->state[12];
    this->state[12] = RotateLeft(last, 25);
    last = one;
    one = this->state[2];
    this->state[2] = RotateLeft(last, 43);
    last = one;
    one = this->state[20];
    this->state[20] = RotateLeft(last, 62);
    last = one;
    one = this->state[14];
    this->state[14] = RotateLeft(last, 18);
    last = one;
    one = this->state[22];
    this->state[22] = RotateLeft(last, 39);
    last = one;
    one = this->state[9];
    this->state[9] = RotateLeft(last, 61);
    last = one;
    one = this->state[6];
    this->state[6] = RotateLeft(last, 20);
    last = one;
    this->state[1] = RotateLeft(last, 44);

    for (unsigned int j = 0; j < 25; j += 5) {
      const std::uint64_t first = this->state[j];
      const std::uint64_t second = this->state[j + 1];
      this->state[j] ^= this->state[j + 2] & ~second;
      this->state[j + 1] ^= this->state[j + 3] & ~this->state[j + 2];
      this->state[j + 2] ^= this->state[j + 4] & ~this->state[j + 3];
      this->state[j + 3] ^= first & ~this->state[j + 4];
      this->state[j + 4] ^= second & ~first;
    }

    this->state[0] ^= sha3RoundConstants[round];
  }
}
