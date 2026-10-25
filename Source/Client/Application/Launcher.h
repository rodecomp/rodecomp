#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_LAUNCHER_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_LAUNCHER_H

#include <windows.h>

bool RestartFromCommandLine(char* commandLine);

void ExitForProtectionCheck(const char* commandLine);
bool AcquireInstanceMutex(HANDLE* mutex);

#endif
