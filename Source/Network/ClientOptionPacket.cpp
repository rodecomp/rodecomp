#include "Network/ClientOptionPacket.h"
#include "Client/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"

#include <cstdint>

namespace {
#pragma pack(push, 1)
  struct ClientOptionPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ int option;
    /* 0006 */ int value;
  };
#pragma pack(pop)
  static_assert(sizeof(ClientOptionPacket) == 10);
}

// OFFSET: 0x00E48400
// FUNCTION: fn_24219
void SendClientOption(unsigned char value, CConnection& connection, const PacketLengths& lengths) {
  ClientOptionPacket packet;
  packet.id = 0x02D8;
  packet.option = 6;
  packet.value = value;
  connection.Send(lengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00E48BA0
// FUNCTION: fn_24227
void CSession::SendHomunculusAutoFeedOption(unsigned char enabled) {
  ClientOptionPacket packet;
  packet.id = 0x02D8;
  packet.option = 3;
  packet.value = enabled;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00E49380
// FUNCTION: fn_24234
void CSession::SendPetAutoFeedOption(unsigned char enabled) {
  ClientOptionPacket packet;
  packet.id = 0x02D8;
  packet.option = 2;
  packet.value = enabled;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}
