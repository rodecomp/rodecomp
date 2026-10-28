#include "Network/ClientKeepalive.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"

#include <timeapi.h>

namespace {
#pragma pack(push, 1)
  struct KeepalivePacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::uint32_t accountId;
  };
#pragma pack(pop)
  static_assert(sizeof(KeepalivePacket) == 6);
}

// OFFSET: 0x00D8D960
// FUNCTION: fn_22268
void SendClientKeepalive(int subMode, std::uint32_t& nextPingTime, CConnection& connection, const PacketLengths& lengths, const std::uint32_t& accountId) {
  if (subMode != 7 && subMode != 8 && subMode != 11 && subMode != 19) {
    return;
  }

  if (timeGetTime() > nextPingTime) {
    nextPingTime = timeGetTime() + 12000;

    KeepalivePacket packet;
    packet.id = 0x0187;
    packet.accountId = accountId;
    connection.Send(lengths.FixedLength(packet.id), &packet);
  }
}
