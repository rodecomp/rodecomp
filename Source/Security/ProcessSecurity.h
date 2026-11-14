#ifndef RAGNAROK_SOURCE_SECURITY_PROCESSSECURITY_H
#define RAGNAROK_SOURCE_SECURITY_PROCESSSECURITY_H

#include <cstddef>
#include <windows.h>

class ProcessModuleApi {
public:
  /* 0000 */ FARPROC enumProcessModules;
  /* 0004 */ FARPROC getModuleFileName;
  /* 0008 */ FARPROC createSnapshot;
  /* 000C */ FARPROC processFirst;
  /* 0010 */ FARPROC processNext;
  /* 0014 */ FARPROC moduleFirst;
  /* 0018 */ FARPROC moduleNext;
  /* 001C */ HMODULE library;
  /* 0020 */ int bad;
  /* 0024 */ int usePsapi;

  int Bad() const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ProcessModuleApi, bad) == 0x20);
static_assert(offsetof(ProcessModuleApi, usePsapi) == 0x24);
#endif

int RemoveProtectionTools();
BOOL __stdcall SetDebugPrivilege(int enabled);
int LoadLibraryFunctions(HMODULE* module, const char* name, int count, ...);
BOOL IsLegacyWindows();

#endif
