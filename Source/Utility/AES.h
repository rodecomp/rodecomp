#ifndef RAGNAROK_SOURCE_UTILITY_AES_H
#define RAGNAROK_SOURCE_UTILITY_AES_H

#include <windows.h>
#include <bcrypt.h>
#include <vector>

class AES {
public:
  enum Mode : int { ECB = 0, CBC = 1, CFB = 2 };

private:
  /* 0000 */ BCRYPT_ALG_HANDLE m_algorithm;
  /* 0004 */ BCRYPT_KEY_HANDLE m_key;
  /* 0008 */ std::vector<UCHAR> m_keyObject;
  /* 0014 */ std::vector<UCHAR> m_iv;
  /* 0020 */ int m_mode;

public:
  // INLINED
  AES() : m_algorithm(nullptr), m_key(nullptr), m_mode(CBC) {
  }

  ~AES();
  ULONG Decrypt(PUCHAR input, ULONG inputSize, PUCHAR output, ULONG outputSize, UCHAR flags);
  bool Initialize(PUCHAR secret, ULONG secretSize, const UCHAR* iv, ULONG ivSize, int mode);
  bool Initialize(PUCHAR secret, ULONG secretSize, int mode);
  bool SetIV(const UCHAR* iv, ULONG ivSize);
  bool SetMode(int mode);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(std::vector<UCHAR>) == 12);
static_assert(sizeof(AES) == 36);
#endif

#endif
