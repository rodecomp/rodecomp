#include "Contents/World/WorldProximity.h"
#include "Contents/World/WorldScene.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/World/TerrainResource.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00AC93C0
// FUNCTION: fn_17683
void UpdateWorldProximityNotification(
  unsigned char& notified, std::function<bool()> nearby, CConnection& connection, const PacketLengths& lengths, const std::uint32_t& accountId
) {
  if (notified == 1) {
    if (!nearby()) {
      notified = 0;
    }

    return;
  }

  if (nearby()) {
    unsigned char packet[9];
    const std::uint16_t id = 0x0BB0;
    const std::uint16_t enabled = 1;
    std::memcpy(packet, &id, sizeof(id));
    std::memcpy(packet + 2, &accountId, sizeof(accountId));
    std::memcpy(packet + 6, &enabled, sizeof(enabled));
    packet[8] = 1;
    connection.Send(lengths.FixedLength(id), packet);
    notified = 1;
  }
}

// OFFSET: 0x00AC8A40

// OFFSET: 0x00ACA180
// FUNCTION: fn_17689
void WorldScene::ClearActorCells() {
  this->actorCells.clear();
  this->unindexedActorIds.clear();
}
