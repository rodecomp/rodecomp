#ifndef RAGNAROK_SOURCE_SECURITY_NPROTECTGAMEGUARDMGR_H
#define RAGNAROK_SOURCE_SECURITY_NPROTECTGAMEGUARDMGR_H

#include <cstddef>
#include <string>
#include <windows.h>

class CNProtectGameGuardMgr {
public:
  static int Create();
  static const char* GameName();

  /* 0000 */ void* vtable;
  /* 0004 */ bool m_initialized;
  /* 0008 */ unsigned int unknown_0008;
  /* 000C */ void* m_gameLib;
  /* 0010 */ bool m_hasError;
  /* 0014 */ std::string m_errorMessage;
  /* 002C */ CRITICAL_SECTION m_lock;
  /* 0044 */ unsigned char unknown_0044[0xC];

  void ProcessAuth(int first, int second);
  void Stop();
  void Tick();
  int HasError();
  std::string ErrorMessage();
  void SetHwnd(HWND window);
  void IncrementCounter();
  void SetError(const char* file, int line, std::string message);
  void Send(const char* text);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CNProtectGameGuardMgr, m_errorMessage) == 0x14);
static_assert(offsetof(CNProtectGameGuardMgr, m_lock) == 0x2C);
static_assert(sizeof(CNProtectGameGuardMgr) == 0x50);
#endif

extern CNProtectGameGuardMgr* g_nProtectGameGuardMgr;

#endif
