#ifndef RAGNAROK_SOURCE_UTILITY_REGISTRYKEY_H
#define RAGNAROK_SOURCE_UTILITY_REGISTRYKEY_H

#include <windows.h>

class RegistryKey {
public:
  /* 0000 */ HKEY handle;

  bool Create(HKEY parent, const char* subKey);
  bool Open(HKEY parent, const char* subKey);
  bool Query(const char* name, DWORD expectedType, BYTE* data, DWORD size);
  bool Set(const char* name, DWORD type, const BYTE* data, DWORD size);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RegistryKey) == 4);
#endif

#endif
