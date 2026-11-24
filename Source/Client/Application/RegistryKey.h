#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_REGISTRYKEY_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_REGISTRYKEY_H

#include <windows.h>

class RegistryKey {
public:
  RegistryKey() : handle(nullptr) {}

  // OFFSET: 0x00B92F70
  // FUNCTION: fn_46594
  ~RegistryKey() {
    if (this->handle != nullptr) {
      RegCloseKey(this->handle);
    }
    this->handle = nullptr;
  }

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
