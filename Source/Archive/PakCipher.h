#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKCIPHER_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKCIPHER_H

#include <cstdint>

namespace myGPack {

  // Reconstructed semantic class name; original symbol unavailable.
  // The original stream routine processes complete eight-byte blocks, including
  // one block for length <= 0. Callers must provide corresponding buffer space.
  class PakCipher {
  public:
    PakCipher();
    explicit PakCipher(const std::uint8_t* key);
    ~PakCipher();
    int DecodeBlock(int rounds, const std::uint8_t* input, std::uint8_t* output) const;
    int Decode(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength, int cycle) const;
    int DecodePrefix(const std::uint8_t* input, int length, std::uint8_t* output, int* outputLength) const;

  private:
    std::uint8_t m_roundKeys[16][8];
  };

  static_assert(sizeof(PakCipher) == 128);
  std::uint8_t* __stdcall DerivePakKey(const char* name, std::uint8_t* key);
  int __stdcall InitialPermutation(std::uint8_t* block);
  int __stdcall FinalPermutation(std::uint8_t* block);
  int __stdcall EncodeBlock(
    int rounds,
    const std::uint8_t (*roundKeys)[8],
    std::uint32_t inputLow,
    std::uint32_t inputHigh,
    std::uint8_t* output
  );

}

#endif
