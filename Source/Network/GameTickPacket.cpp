#include "Network/GameTickPacket.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"

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
void SendGameTick(bool force, std::uint32_t& pendingTick, std::uint32_t& lastTick, const std::uint32_t& interval,
  CConnection& connection, const PacketLengths& lengths, const bool& recording, std::uint32_t& firstRecordedTick,
  std::list<std::uint32_t>& recordedTicks) {
  if (!force && timeGetTime() <= lastTick + interval) {
    return;
  }

  pendingTick = 0;
  lastTick = timeGetTime();

  GameTickPacket packet;
  packet.id = 0x0360;
  packet.tick = timeGetTime();
  connection.Send(lengths.FixedLength(packet.id), &packet);

  if (recording) {
    recordedTicks.push_back(timeGetTime());
    if (firstRecordedTick == 0) {
      firstRecordedTick = recordedTicks.front();
    }
  }
}
