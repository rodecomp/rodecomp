#ifndef RAGNAROK_SOURCE_NETWORK_PACKETS_PARTYJOINPACKET_H
#define RAGNAROK_SOURCE_NETWORK_PACKETS_PARTYJOINPACKET_H

#include <cstdint>
#include <string>

class CConnection;
struct PacketLengths;
class WindowManagerWindows;

void SendPartyJoin(CConnection& connection, const PacketLengths& lengths, const std::uint32_t* accountId, const std::uint32_t* characterId);
void SendPartyJoinDecision(CConnection& connection, const PacketLengths& lengths, std::uint32_t characterId, bool accepted, const std::uint32_t* accountId);
void ReceivePartyJoinRequest(const void* packet);
void SendPartyInvitationDecision(
  CConnection& connection, const PacketLengths& lengths, WindowManagerWindows& windows, std::string& pendingName, std::uint32_t identity, bool accepted
);

#endif
