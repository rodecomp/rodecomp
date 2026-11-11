#ifndef RAGNAROK_SOURCE_NETWORK_PACKETS_EXECUTABLEDIGESTPACKET_H
#define RAGNAROK_SOURCE_NETWORK_PACKETS_EXECUTABLEDIGESTPACKET_H

#include <windows.h>

class CConnection;
class PacketLengths;

void SendExecutableDigest(const int& serviceType, const int& serviceMode, HINSTANCE instance,
  CConnection& connection, const PacketLengths& lengths);

#endif
