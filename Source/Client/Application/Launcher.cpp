#include "Client/Application/Launcher.h"

#include <cstdint>
#include <cstring>
#include <windows.h>

// STATIC: 0x0173ad64
// GLOBAL: gl_24472
std::uint32_t g_clientVersion;

// OFFSET: 0x00E58820
// FUNCTION: fn_24363
bool RestartFromCommandLine(char* commandLine) {
  char* marker = std::strstr(commandLine, "/launcherRESTART:");
  if (!marker) {
    return false;
  }

  STARTUPINFOA startup = {};
  startup.cb = sizeof(startup);
  startup.dwFlags = STARTF_USESHOWWINDOW;
  startup.wShowWindow = SW_SHOWNORMAL;
  PROCESS_INFORMATION process;

  CreateProcessA(nullptr, marker + std::strlen("/launcherRESTART:"), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &process);
  CloseHandle(process.hProcess);
  CloseHandle(process.hThread);

  return true;
}

// INLINED
void ExitForProtectionCheck(const char* commandLine) {
  if (std::strstr(commandLine, "/check_themida")) {
    ExitProcess(0);
  }
}

// INLINED
bool AcquireInstanceMutex(HANDLE* mutex) {
  *mutex = CreateMutexA(nullptr, TRUE, "MutexInit");
  return GetLastError() != ERROR_ALREADY_EXISTS;
}
