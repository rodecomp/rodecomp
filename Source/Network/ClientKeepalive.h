#ifndef RAGNAROK_SOURCE_NETWORK_CLIENTKEEPALIVE_H
#define RAGNAROK_SOURCE_NETWORK_CLIENTKEEPALIVE_H

#include <cstdint>

class CConnection;
class PacketLengths;

void SendClientKeepalive(int subMode, std::uint32_t& nextPingTime, CConnection& connection, const PacketLengths& lengths, const std::uint32_t& accountId);

#endif
