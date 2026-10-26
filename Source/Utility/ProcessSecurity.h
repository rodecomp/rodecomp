#ifndef RAGNAROK_SOURCE_UTILITY_PROCESSSECURITY_H
#define RAGNAROK_SOURCE_UTILITY_PROCESSSECURITY_H

#include <windows.h>

int RemoveProtectionTools();
BOOL __stdcall SetDebugPrivilege(int enabled);
int LoadLibraryFunctions(HMODULE* module, const char* name, int count, ...);
BOOL IsLegacyWindows();

#endif
