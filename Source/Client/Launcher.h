#ifndef RAGNAROK_SOURCE_CLIENT_LAUNCHER_H
#define RAGNAROK_SOURCE_CLIENT_LAUNCHER_H

#include <windows.h>

namespace client {

  // Reconstructed semantic name for the original launcher restart branch.
  // A mutable buffer is required because CreateProcessA may modify its tail.
  bool RestartFromCommandLine(char* commandLine);

  // Named source extractions of the established WinMain guard blocks.
  void ExitForProtectionCheck(const char* commandLine);
  bool AcquireInstanceMutex(HANDLE* mutex);

}

#endif
