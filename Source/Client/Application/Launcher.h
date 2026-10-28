#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_LAUNCHER_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_LAUNCHER_H

#include <cstdint>
#include <windows.h>

extern std::uint32_t g_clientVersion;
extern char g_applicationDirectory[0x80];

bool RestartFromCommandLine(char* commandLine);

void ExitForProtectionCheck(const char* commandLine);
bool AcquireInstanceMutex(HANDLE* mutex);

#endif
