#include "Client/Launcher.h"

#include <cstring>
#include <windows.h>

namespace client {

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
    // Original behavior ignores CreateProcessA failure and still returns true.
    CreateProcessA(
      nullptr, marker + std::strlen("/launcherRESTART:"), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup,
      &process
    );
    CloseHandle(process.hProcess);
    CloseHandle(process.hThread);
    return true;
  }

  // OFFSET: 0x00E5D390
  // FUNCTION: fn_24380
  void ExitForProtectionCheck(const char* commandLine) {
    if (std::strstr(commandLine, "/check_themida")) {
      ExitProcess(0);
    }
  }

  // OFFSET: 0x00E5D390
  // FUNCTION: fn_24380
  bool AcquireInstanceMutex(HANDLE* mutex) {
    *mutex = CreateMutexA(nullptr, TRUE, "MutexInit");
    return GetLastError() != ERROR_ALREADY_EXISTS;
  }

}
