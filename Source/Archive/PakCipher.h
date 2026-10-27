#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKCIPHER_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKCIPHER_H

#include <cstdint>

namespace myGPack {

  class PakCipher {
  public:
    PakCipher();
    explicit PakCipher(const std::uint8_t* key);
    ~PakCipher();
    int DecodeBlock(int rounds, const std::uint8_t* input, std::uint8_t* output) const;
    int Decode(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength, int cycle) const;
    int DecodePrefix(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength) const;

  private:
    /* 0000 */ std::uint8_t m_roundKeys[16][8];

    void BuildRoundKeys(const std::uint8_t* key);
  };

  static_assert(sizeof(PakCipher) == 128);
  std::uint8_t* DerivePakKey(const char* name, std::uint8_t* key);
  int InitialPermutation(std::uint8_t* block);
  int FinalPermutation(std::uint8_t* block);
  int EncodeBlock(int rounds, const std::uint8_t (*roundKeys)[8], std::uint32_t inputLow, std::uint32_t inputHigh, std::uint8_t* output);

}

#endif
