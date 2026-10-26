#ifndef RAGNAROK_SOURCE_UTILITY_MD5_H
#define RAGNAROK_SOURCE_UTILITY_MD5_H

#include <cstdint>

class MD5 {
public:
  MD5();
  ~MD5();

  int Initialize();
  void Update(const unsigned char* input, std::uint32_t length);
  unsigned char* Finalize();
  unsigned char* DigestBuffer(const unsigned char* input, std::uint32_t length, unsigned char* digest);
  std::uint32_t HashBuffer(const unsigned char* input, std::uint32_t length);
  std::uint32_t HashString(const char* text);
  std::uint32_t HashString(const wchar_t* text);
  std::uint32_t HashFile(const wchar_t* filename);
  std::uint32_t HashStringLower(const char* text);
  std::uint32_t HashStringLower(const wchar_t* text);
  std::uint32_t HashFile(const char* filename);

private:
  const unsigned char* Decode(std::uint32_t* output, const unsigned char* input, std::uint32_t count);
  std::uint32_t* Transform(std::uint32_t* state, const std::uint32_t* input);

  union {
    /* 0000 */ unsigned char m_buffer[64];
    /* 0000 */ std::uint32_t m_words[16];
  };
  /* 0040 */ std::uint32_t m_state[4];
  /* 0050 */ std::uint32_t m_byteCount[2];
};

static_assert(sizeof(MD5) == 0x58);

#endif
