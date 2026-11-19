#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKCIPHER_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKCIPHER_H

#include <cstdint>

namespace myGPack {

  union Block64 {
    /* 0000 */ std::uint8_t bytes[8];
    /* 0000 */ std::uint32_t words[2];
  };

  static_assert(sizeof(Block64) == 8);

  class PakCipher {
  public:
    PakCipher();
    explicit PakCipher(const std::uint8_t* key);
    ~PakCipher();
    int DecodeBlock(int rounds, const Block64* roundKeys, Block64 input, Block64* output) const;
    int EncodeBlock(int rounds, const Block64* roundKeys, Block64 input, Block64* output) const;
    int Decode(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength, int cycle) const;
    int DecodePrefix(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength) const;

  private:
    /* 0000 */ Block64 m_roundKeys[16];

    void BuildRoundKeys(const std::uint8_t* key);
    int InitialPermutation(std::uint8_t* block) const;
    int FinalPermutation(std::uint8_t* block) const;
  };

  static_assert(sizeof(PakCipher) == 128);

}

#endif
