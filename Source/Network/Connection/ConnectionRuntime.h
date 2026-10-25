#ifndef RAGNAROK_SOURCE_NETWORK_CONNECTION_CONNECTIONRUNTIME_H
#define RAGNAROK_SOURCE_NETWORK_CONNECTION_CONNECTIONRUNTIME_H

#include <winsock2.h>

extern int(WSAAPI* g_connectionSend)(SOCKET socket, const char* data, int bytes, int flags);
extern int(WSAAPI* g_connectionReceive)(SOCKET socket, char* data, int bytes, int flags);

bool InitializeConnections();
void ShutdownConnections();
bool CheckConnectionRuntime();

#endif
