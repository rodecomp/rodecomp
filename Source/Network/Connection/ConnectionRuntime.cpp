#include "Network/Connection/ConnectionRuntime.h"

#include <windows.h>

namespace {
  // STATIC: 0x016FDC94
  // GLOBAL: gl_24393
  HMODULE module;
}
// STATIC: 0x016FDC8C
// STATIC: 0x016FDC90
int(WSAAPI* g_connectionSend)(SOCKET, const char*, int, int);
int(WSAAPI* g_connectionReceive)(SOCKET, char*, int, int);

// OFFSET: 0x00C7C400
// FUNCTION: fn_20387
bool CheckConnectionRuntime() {
  HMODULE library = LoadLibraryA("ws2_32.dll");
  if (!library) {
    return true;
  }

  static const char* names[] = {"connect", "closesocket", "send", "select"};

  for (const char* name : names) {
    if (GetProcAddress(library, name) != GetProcAddress(module, name)) {
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
  WSADATA data;
  WSAStartup(MAKEWORD(1, 1), &data);
  module = LoadLibraryA("ws2_32.dll");
  g_connectionSend = reinterpret_cast<decltype(g_connectionSend)>(GetProcAddress(module, "send"));
  g_connectionReceive = reinterpret_cast<decltype(g_connectionReceive)>(GetProcAddress(module, "recv"));
  CheckConnectionRuntime();

  return true;
}

// OFFSET: 0x00C7C4B0
// FUNCTION: fn_20388
void ShutdownConnections() {
  WSACleanup();
  FreeLibrary(module);
}
