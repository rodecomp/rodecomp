#include "Network/Connection/ConnectionRuntime.h"

#include <windows.h>

namespace {
  // GLOBAL: gl_24389
  WSADATA wsaData;

  // STATIC: 0x016FDC94
  // GLOBAL: gl_24393
  HMODULE module;
}
// STATIC: 0x016FDC8C
// GLOBAL: gl_24391
int(WSAAPI* g_connectionSend)(SOCKET, const char*, int, int);

// STATIC: 0x016FDC90
// GLOBAL: gl_24392
int(WSAAPI* g_connectionReceive)(SOCKET, char*, int, int);

// OFFSET: 0x00C7C400
// FUNCTION: fn_20387
bool CheckConnectionRuntime() {
  const char* names[] = {"connect", "closesocket", "send", "select"};
  HMODULE library = LoadLibraryA("ws2_32.dll");
  if (!library) {
    return true;
  }

  for (unsigned index = 0; index < 4; ++index) {
    if (GetProcAddress(library, names[index]) != GetProcAddress(module, names[index])) {
      FreeLibrary(library);
      return false;
    }
  }

  FreeLibrary(library);
  return true;
}

// OFFSET: 0x00C7D020
// FUNCTION: fn_20408
bool InitializeConnections() {
  WSAStartup(MAKEWORD(1, 1), &wsaData);
  module = LoadLibraryA("ws2_32.dll");
  g_connectionSend = reinterpret_cast<decltype(g_connectionSend)>(GetProcAddress(module, "send"));
  g_connectionReceive = reinterpret_cast<decltype(g_connectionReceive)>(GetProcAddress(module, "recv"));
  CheckConnectionRuntime();

  return true;
}

// OFFSET: 0x00C7C4B0
// FUNCTION: fn_20388
void Cleanup() {
  WSACleanup();
  FreeLibrary(module);
}
