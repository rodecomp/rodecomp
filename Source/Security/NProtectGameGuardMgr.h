#ifndef RAGNAROK_SOURCE_SECURITY_NPROTECTGAMEGUARDMGR_H
#define RAGNAROK_SOURCE_SECURITY_NPROTECTGAMEGUARDMGR_H

#include <cstddef>
#include <string>
#include <vector>
#include <windows.h>

// VTABLE: vt_00506
class CNProtectGameGuardMgr {
public:
  // VTABLE: vt_00505
  class lcCSAuthToken {
  public:
    // INLINED
    lcCSAuthToken(int id, const std::vector<unsigned char>& bytes) : id(id), bytes(bytes) {
    }

    virtual ~lcCSAuthToken() = default;

    /* 0004 */ int id;
    /* 0008 */ std::vector<unsigned char> bytes;
  };

  static int Create();
  static void Destroy();
  static const char* GameName();

  virtual ~CNProtectGameGuardMgr();

  /* 0004 */ bool m_initialized;
  /* 0008 */ unsigned int padding_0008;
  /* 000C */ void* m_gameLib;
  /* 0010 */ bool m_hasError;
  /* 0014 */ std::string m_errorMessage;
  /* 002C */ CRITICAL_SECTION m_lock;
  /* 0044 */ std::vector<lcCSAuthToken> m_tokens;

  void ProcessAuth(int first, int second);
  void Stop();
  void Tick();
  int HasError();
  std::string ErrorMessage();
  void SetHwnd(HWND window);
  bool Initialize();
  void IncrementCounter();
  void SetError(const char* file, int line, std::string message);
  void Send(const char* text);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CNProtectGameGuardMgr, m_errorMessage) == 0x14);
static_assert(offsetof(CNProtectGameGuardMgr, m_lock) == 0x2C);
static_assert(offsetof(CNProtectGameGuardMgr, m_tokens) == 0x44);
static_assert(sizeof(CNProtectGameGuardMgr::lcCSAuthToken) == 0x14);
static_assert(sizeof(CNProtectGameGuardMgr) == 0x50);
#endif

extern CNProtectGameGuardMgr* g_nProtectGameGuardMgr;

int __stdcall GameGuardCallback(int code, const int* packet);

#endif
