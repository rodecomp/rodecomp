#ifndef RAGNAROK_SOURCE_NETWORK_CONNECTIONRUNTIME_H
#define RAGNAROK_SOURCE_NETWORK_CONNECTIONRUNTIME_H

#include <winsock2.h>

// Dynamically resolved I/O entries.
extern int(WSAAPI* g_connectionSend)(SOCKET socket, const char* data, int bytes, int flags);
extern int(WSAAPI* g_connectionReceive)(SOCKET socket, char* data, int bytes, int flags);

bool InitializeConnections();
void ShutdownConnections();
bool CheckConnectionRuntime();

#endif
