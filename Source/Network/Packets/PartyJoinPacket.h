#ifndef RAGNAROK_SOURCE_NETWORK_PACKETS_PARTYJOINPACKET_H
#define RAGNAROK_SOURCE_NETWORK_PACKETS_PARTYJOINPACKET_H

#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdint>
#include <cstring>
#include <string>

class CConnection;
class PacketLengths;
class WindowManagerWindows;

void SendPartyJoin(CConnection& connection, const PacketLengths& lengths, const std::uint32_t* accountId, const std::uint32_t* characterId);
void SendPartyJoinDecision(CConnection& connection, const PacketLengths& lengths, std::uint32_t characterId, bool accepted, const std::uint32_t* accountId);
void ReceivePartyJoinRequest(const void* packet);
void SendPartyInvitationDecision(
  CConnection& connection, const PacketLengths& lengths, WindowManagerWindows& windows, std::string& pendingName, std::uint32_t identity, bool accepted
);

// INLINED
inline void SendPartyJoin(CConnection& connection, const PacketLengths& lengths, const std::uint32_t* accountId, const std::uint32_t* characterId) {
  unsigned char packet[10];
  const std::uint16_t id = 0x0AE6;
  std::memcpy(packet, &id, sizeof(id));
  std::memcpy(packet + 6, accountId, sizeof(*accountId));
  std::memcpy(packet + 2, characterId, sizeof(*characterId));
  connection.Send(lengths.FixedLength(id), packet);
}

// INLINED
inline void SendPartyJoinDecision(CConnection& connection, const PacketLengths& lengths, std::uint32_t characterId, bool accepted, const std::uint32_t* accountId) {
  unsigned char packet[11];
  const std::uint16_t id = 0x0AF8;
  std::memcpy(packet, &id, sizeof(id));
  std::memcpy(packet + 2, &characterId, sizeof(characterId));
  packet[10] = accepted;
  std::memcpy(packet + 6, accountId, sizeof(*accountId));
  connection.Send(lengths.FixedLength(id), packet);
}

// INLINED
inline void SendPartyInvitationDecision(
  CConnection& connection, const PacketLengths& lengths, WindowManagerWindows& windows, std::string& pendingName, std::uint32_t identity, bool accepted
) {
  if (accepted) {
    windows.EnsureWindow(34);
  } else {
    pendingName.assign("", 0);
  }

  unsigned char packet[7];
  const std::uint16_t id = 0x02C7;
  std::memcpy(packet, &id, sizeof(id));
  packet[6] = accepted;
  std::memcpy(packet + 2, &identity, sizeof(identity));
  connection.Send(lengths.FixedLength(id), packet);
}

#endif
