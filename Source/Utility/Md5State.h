#ifndef RAGNAROK_SOURCE_UTILITY_MD5STATE_H
#define RAGNAROK_SOURCE_UTILITY_MD5STATE_H

#include <cstddef>
#include <cstdint>

class MD5State {
public:
  void Init();
  void Append(const unsigned char* data, int nbytes);
  void Finish(unsigned char* digest);

  void Process(const unsigned char* data);

  /* 0000 */ std::uint32_t count[2];
  /* 0008 */ std::uint32_t abcd[4];
  /* 0018 */ unsigned char buf[64];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(MD5State, abcd) == 0x08);
static_assert(offsetof(MD5State, buf) == 0x18);
static_assert(sizeof(MD5State) == 0x58);
#endif

#endif
