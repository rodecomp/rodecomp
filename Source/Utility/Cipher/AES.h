#ifndef RAGNAROK_SOURCE_UTILITY_CIPHER_AES_H
#define RAGNAROK_SOURCE_UTILITY_CIPHER_AES_H

#include <cstddef>
#include <exception>

class CAES {
public:
  enum { DEFAULT_BLOCK_SIZE = 16 };

  enum { MAX_BLOCK_SIZE = 32, MAX_ROUNDS = 14, MAX_KC = 8, MAX_BC = 8 };

  CAES(int keyLength, int blockSize);
  virtual ~CAES();

  void EncryptBlock(const char* in, char* result);
  void DecryptBlock(const char* in, char* result);
  void DefEncryptBlock(const char* in, char* result);
  void DefDecryptBlock(const char* in, char* result);
  void Xor(unsigned char* destination, const unsigned char* source);

  // INLINED
  void Decrypt(const char* in, char* result, std::size_t n) {
    if (!this->m_bKeyInit) {
      throw std::exception(sm_szErrorMsg1);
    }

    if (n == 0 || n % this->m_blockSize != 0) {
      throw std::exception(sm_szErrorMsg2);
    }

    for (std::size_t i = 0; i < n / this->m_blockSize; ++i) {
      this->DecryptBlock(in, result);
      in += this->m_blockSize;
      result += this->m_blockSize;
    }
  }

private:
  static const char* sm_szErrorMsg1;
  static const char* sm_szErrorMsg2;
  static const unsigned char sm_key[16];
  static const unsigned char sm_chain0[16];
  static const signed char sm_S[256];
  static const signed char sm_Si[256];
  static const int sm_T1[256];
  static const int sm_T2[256];
  static const int sm_T3[256];
  static const int sm_T4[256];
  static const int sm_T5[256];
  static const int sm_T6[256];
  static const int sm_T7[256];
  static const int sm_T8[256];
  static const int sm_U1[256];
  static const int sm_U2[256];
  static const int sm_U3[256];
  static const int sm_U4[256];
  static const signed char sm_rcon[30];
  static const int sm_shifts[3][4][2];

  /* 0004 */ bool m_bKeyInit;
  /* 0008 */ int m_Ke[MAX_ROUNDS + 1][MAX_BC];
  /* 01E8 */ int m_Kd[MAX_ROUNDS + 1][MAX_BC];
  /* 03C8 */ int m_keylength;
  /* 03CC */ int m_blockSize;
  /* 03D0 */ int m_iROUNDS;
  /* 03D4 */ char m_chain0[MAX_BLOCK_SIZE];
  /* 03F4 */ char m_chain[MAX_BLOCK_SIZE];
  /* 0414 */ int tk[MAX_KC];
  /* 0434 */ int a[MAX_BC];
  /* 0454 */ int t[MAX_BC];
};

void DecryptAES(const char* input, char* output, unsigned int length);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAES) == 0x474);
#endif

#endif
