#ifndef RAGNAROK_SOURCE_NETWORK_HTTP_CURLMGR_H
#define RAGNAROK_SOURCE_NETWORK_HTTP_CURLMGR_H

#include <windows.h>

struct curl_slist;
struct curl_httppost;

class CCURLMgr {
public:
  CCURLMgr();
  virtual ~CCURLMgr();
  static bool Initialize();
  static void Shutdown();

  int(__cdecl* globalInit)(long);
  void(__cdecl* globalCleanup)();
  curl_slist*(__cdecl* slistAppend)(curl_slist*, const char*);
  int(__cdecl* formAdd)(curl_httppost**, curl_httppost**, ...);
  void*(__cdecl* easyInit)();
  int(__cdecl* easySetopt)(void*, int, ...);
  int(__cdecl* easyPerform)(void*);
  void(__cdecl* easyCleanup)(void*);
  void(__cdecl* formFree)(curl_httppost*);
  void(__cdecl* slistFreeAll)(curl_slist*);
  int(__cdecl* easyGetinfo)(void*, int, ...);
  bool m_enabled;
  HMODULE m_module;
};

extern CCURLMgr* g_curlMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCURLMgr) == 56);
#endif

#endif
