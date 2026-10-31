#ifndef RAGNAROK_SOURCE_SECURITY_NPROTECTGAMEGUARDMGR_H
#define RAGNAROK_SOURCE_SECURITY_NPROTECTGAMEGUARDMGR_H

#include <cstddef>
#include <string>
#include <vector>
#include <windows.h>

class CNProtectGameGuardMgr {
public:
  struct CSAuthToken {
    /* 0000 */ void* vtable;
    /* 0004 */ int id;
    /* 0008 */ std::vector<unsigned char> bytes;
  };

  static int Create();
  static void Destroy();
  static const char* GameName();

  /* 0000 */ void* vtable;
  /* 0004 */ bool m_initialized;
  /* 0008 */ unsigned int unknown_0008;
  /* 000C */ void* m_gameLib;
  /* 0010 */ bool m_hasError;
  /* 0014 */ std::string m_errorMessage;
  /* 002C */ CRITICAL_SECTION m_lock;
  /* 0044 */ std::vector<CSAuthToken> m_tokens;

  void ProcessAuth(int first, int second);
  void Stop();
  void Tick();
  int HasError();
  std::string ErrorMessage();
  void SetHwnd(HWND window);
  void sub_76C4C0(unsigned char flags);
  int sub_76C9D0();
  void IncrementCounter();
  void SetError(const char* file, int line, std::string message);
  void Send(const char* text);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CNProtectGameGuardMgr, m_errorMessage) == 0x14);
static_assert(offsetof(CNProtectGameGuardMgr, m_lock) == 0x2C);
static_assert(offsetof(CNProtectGameGuardMgr, m_tokens) == 0x44);
static_assert(sizeof(CNProtectGameGuardMgr::CSAuthToken) == 0x14);
static_assert(sizeof(CNProtectGameGuardMgr) == 0x50);
#endif

extern CNProtectGameGuardMgr* g_nProtectGameGuardMgr;

#endif
