#include "Utility/ProcessSecurity.h"

#include <cstdarg>
#include <cstring>
#include <io.h>

// OFFSET: 0x00AEA5B0
// FUNCTION: fn_18120
int __fastcall sub_AEA5B0(const void* self) {
  return *reinterpret_cast<const int*>(static_cast<const char*>(self) + 0x20);
}

// FUNCTION: fn_18122
int RemoveProtectionTools() {
  if (_access("nconfig.ini", 6) == 0) {
    _unlink("nconfig.ini");
  }

  if (_access("ffindhack.exe", 6) == 0) {
    _unlink("ffindhack.exe");
  }

  if (_access("gfindhack.exe", 6) == 0) {
    _unlink("gfindhack.exe");
  }

  if (_access("removenp.exe", 6) == 0) {
    _unlink("removenp.exe");
  }

  const int result = _access("cfindhack.exe", 6);
  return result == 0 ? _unlink("cfindhack.exe") : result;
}

// FUNCTION: fn_18124
int LoadLibraryFunctions(HMODULE* module, const char* name, int count, ...) {
  *module = LoadLibraryA(name);
  if (*module != nullptr) {
    std::va_list arguments;
    va_start(arguments, count);
    int index = 0;
    for (; index < count; ++index) {
      FARPROC* output = va_arg(arguments, FARPROC*);
      const char* symbol = va_arg(arguments, const char*);
      *output = GetProcAddress(*module, symbol);
      if (*output == nullptr) {
        break;
      }
    }

    va_end(arguments);
    if (index >= count) {
      return 1;
    }
  }

  if (*module != nullptr) {
    FreeLibrary(*module);
  }

  return 0;
}

// FUNCTION: fn_18123
BOOL __stdcall SetDebugPrivilege(int enabled) {
  BOOL result = FALSE;
  HMODULE module = nullptr;
  FARPROC openToken;
  FARPROC lookupPrivilege;
  FARPROC adjustPrivileges;
  if (LoadLibraryFunctions(
        &module, "advapi32.dll", 3, &openToken, "OpenProcessToken", &lookupPrivilege, "LookupPrivilegeValueA", &adjustPrivileges, "AdjustTokenPrivileges"
      ) == 1) {
    HANDLE token;
    const auto open = reinterpret_cast<decltype(&OpenProcessToken)>(openToken);
    if (open(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token) != FALSE) {
      TOKEN_PRIVILEGES privileges;
      privileges.PrivilegeCount = 1;
      const auto lookup = reinterpret_cast<decltype(&LookupPrivilegeValueA)>(lookupPrivilege);
      lookup(nullptr, "SeDebugPrivilege", &privileges.Privileges[0].Luid);
      privileges.Privileges[0].Attributes = enabled != 0 ? SE_PRIVILEGE_ENABLED : 0;
      const auto adjust = reinterpret_cast<decltype(&AdjustTokenPrivileges)>(adjustPrivileges);
      adjust(token, FALSE, &privileges, sizeof(privileges), nullptr, nullptr);
      result = GetLastError() == ERROR_SUCCESS;
      CloseHandle(token);
    }

    FreeLibrary(module);
  }

  return result;
}

// FUNCTION: fn_18125
BOOL IsLegacyWindows() {
  OSVERSIONINFOEXA version;
  std::memset(reinterpret_cast<unsigned char*>(&version) + sizeof(version.dwOSVersionInfoSize), 0, sizeof(version) - sizeof(version.dwOSVersionInfoSize));
  version.dwOSVersionInfoSize = sizeof(version);
  if (GetVersionExA(reinterpret_cast<OSVERSIONINFOA*>(&version)) == FALSE) {
    version.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    if (GetVersionExA(reinterpret_cast<OSVERSIONINFOA*>(&version)) == FALSE) {
      return FALSE;
    }
  }

  return version.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS;
}
