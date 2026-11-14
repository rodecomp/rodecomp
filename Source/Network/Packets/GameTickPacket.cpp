#include "Network/Packets/GameTickPacket.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CLoginMode.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Security/NProtectGameGuardMgr.h"
#include "Network/Packets/PacketId.h"

#include <timeapi.h>

namespace {
#pragma pack(push, 1)
  struct GameTickPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::uint32_t tick;
  };
#pragma pack(pop)
  static_assert(sizeof(GameTickPacket) == 6);
}

// OFFSET: 0x00CED520
// FUNCTION: fn_21367
void CGameMode::SendSyncRequest(bool force) {
  if (!force && timeGetTime() <= this->syncRequestTick + g_gameTickInterval) {
    return;
  }

  g_nProtectGameGuardMgr->Tick();
  this->syncAcknowledged = 0;
  this->syncRequestTick = timeGetTime();

  GameTickPacket packet;
  packet.id = ID_CZ_REQUEST_TIME2;
  packet.tick = timeGetTime();
  GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);

  if (g_pingLogEnabled) {
    g_pingLogTicks.push_back(timeGetTime());
    if (g_pingLogFirstTick == 0) {
      g_pingLogFirstTick = g_pingLogTicks.front();
    }
  }
}

// INLINED
void SendGameTick(bool force, std::uint32_t& pendingTick, std::uint32_t& lastTick, const std::uint32_t& interval,
  CConnection& connection, const PacketLengths& lengths, const bool& recording, std::uint32_t& firstRecordedTick,
  std::list<std::uint32_t>& recordedTicks) {
  reinterpret_cast<CGameMode*>(reinterpret_cast<char*>(&pendingTick) - offsetof(CGameMode, syncAcknowledged))
    ->SendSyncRequest(force);
}
