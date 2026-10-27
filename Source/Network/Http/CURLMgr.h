#ifndef RAGNAROK_SOURCE_NETWORK_HTTP_CURLMGR_H
#define RAGNAROK_SOURCE_NETWORK_HTTP_CURLMGR_H

#include <cstddef>
#include <windows.h>

struct curl_slist;
struct curl_httppost;

class CCURLMgr {
public:
  CCURLMgr();
  virtual ~CCURLMgr();
  static bool Initialize();
  static void Shutdown();

  /* 0004 */ int(__cdecl* globalInit)(long);
  /* 0008 */ void(__cdecl* globalCleanup)();
  /* 000C */ curl_slist*(__cdecl* slistAppend)(curl_slist*, const char*);
  /* 0010 */ int(__cdecl* formAdd)(curl_httppost**, curl_httppost**, ...);
  /* 0014 */ void*(__cdecl* easyInit)();
  /* 0018 */ int(__cdecl* easySetopt)(void*, int, ...);
  /* 001C */ int(__cdecl* easyPerform)(void*);
  /* 0020 */ void(__cdecl* easyCleanup)(void*);
  /* 0024 */ void(__cdecl* formFree)(curl_httppost*);
  /* 0028 */ void(__cdecl* slistFreeAll)(curl_slist*);
  /* 002C */ int(__cdecl* easyGetinfo)(void*, int, ...);
  /* 0030 */ bool m_enabled;
  /* 0034 */ HMODULE m_module;
};

extern CCURLMgr* g_curlMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCURLMgr) == 56);
static_assert(offsetof(CCURLMgr, globalInit) == 0x0004);
static_assert(offsetof(CCURLMgr, globalCleanup) == 0x0008);
static_assert(offsetof(CCURLMgr, slistAppend) == 0x000C);
static_assert(offsetof(CCURLMgr, formAdd) == 0x0010);
static_assert(offsetof(CCURLMgr, easyInit) == 0x0014);
static_assert(offsetof(CCURLMgr, easySetopt) == 0x0018);
static_assert(offsetof(CCURLMgr, easyPerform) == 0x001C);
static_assert(offsetof(CCURLMgr, easyCleanup) == 0x0020);
static_assert(offsetof(CCURLMgr, formFree) == 0x0024);
static_assert(offsetof(CCURLMgr, slistFreeAll) == 0x0028);
static_assert(offsetof(CCURLMgr, easyGetinfo) == 0x002C);
static_assert(offsetof(CCURLMgr, m_enabled) == 0x0030);
static_assert(offsetof(CCURLMgr, m_module) == 0x0034);
#endif

#endif
