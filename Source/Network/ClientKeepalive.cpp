#include "Client/Mode/CLoginMode.h"
#include "Client/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/PacketId.h"

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
void CLoginMode::SendKeepalive() {
  if (this->cursorType != 7 && this->cursorType != 8 && this->cursorType != 11 && this->cursorType != 19) {
    return;
  }

  if (timeGetTime() > this->nextKeepaliveTime) {
    this->nextKeepaliveTime = timeGetTime() + 12000;

    KeepalivePacket packet;
    packet.id = ID_PING;
    packet.accountId = g_session.m_aid;
    GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
  }
}
