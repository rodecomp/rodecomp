#ifndef RAGNAROK_SOURCE_SECURITY_THEMIDAMGR_H
#define RAGNAROK_SOURCE_SECURITY_THEMIDAMGR_H

#include <atomic>
#include <cstddef>
#include <unordered_set>
#include <windows.h>

struct ThemidaKeyEncoderA {
  // INLINED
  ThemidaKeyEncoderA() {
    const unsigned int zero = 0;
    this->Generate(reinterpret_cast<const unsigned char*>(&zero));
  }

  unsigned char* Generate(const unsigned char* input);

  // INLINED
  unsigned int Decode() const {
    unsigned int value;
    unsigned char* output = reinterpret_cast<unsigned char*>(&value);
    output[0] = this->m_key[1] ^ this->m_key[5] ^ this->m_key[0];
    output[1] = this->m_key[1] ^ this->m_key[2] ^ this->m_key[6];
    output[2] = this->m_key[2] ^ this->m_key[3] ^ this->m_key[7];
    output[3] = this->m_key[3] ^ this->m_key[4] ^ this->m_key[8];
    return value;
  }

  /* 0000 */ unsigned char m_key[9];
};

struct ThemidaKeyEncoderB {
  // INLINED
  ThemidaKeyEncoderB() {
    const unsigned int zero = 0;
    this->Generate(reinterpret_cast<const unsigned char*>(&zero));
  }

  unsigned char* Generate(const unsigned char* input);

  // INLINED
  unsigned int Decode() const {
    unsigned int value;
    unsigned char* output = reinterpret_cast<unsigned char*>(&value);
    output[0] = this->m_key[1] ^ this->m_key[5] ^ this->m_key[0];
    output[1] = this->m_key[1] ^ this->m_key[2] ^ this->m_key[6];
    output[2] = this->m_key[2] ^ this->m_key[3] ^ this->m_key[7];
    output[3] = this->m_key[3] ^ this->m_key[4] ^ this->m_key[8];
    return value;
  }

  /* 0000 */ unsigned char m_key[9];
};

inline constexpr unsigned int ThemidaKeyMagic = 0x50525443;

extern std::atomic<bool> s_themidaThreadStop;
extern std::atomic<bool> s_themidaThreadStarted;
extern ThemidaKeyEncoderA s_themidaKeyA;
extern ThemidaKeyEncoderB s_themidaThreadHandle;
extern std::unordered_set<DWORD> s_threadIds;

// VTABLE: vt_00507
class CThemidaMgr {
public:
  CThemidaMgr();
  virtual ~CThemidaMgr();

  static bool Create();
  static void Destroy();
  static void Check();
  static void StopThread();

  // INLINED
  static bool IsProtected() {
    if (s_themidaKeyA.Decode() != ThemidaKeyMagic) {
      return false;
    }

    HANDLE thread = reinterpret_cast<HANDLE>(s_themidaThreadHandle.Decode());
    if (WaitForSingleObject(thread, 0) != WAIT_TIMEOUT) {
      return false;
    }

    return ResumeThread(thread) == 0;
  }

  // INLINED
  static bool IsProtectedThread() {
    return s_threadIds.find(GetCurrentThreadId()) != s_threadIds.end();
  }

  /* 0004 */ bool m_valid;
};

extern CThemidaMgr* g_themidaMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CThemidaMgr) == 8);
static_assert(offsetof(CThemidaMgr, m_valid) == 4);
#endif

#endif
