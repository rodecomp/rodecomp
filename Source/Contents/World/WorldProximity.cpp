#include "Contents/World/WorldProximity.h"
#include "Contents/World/World.h"
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
// FUNCTION: fn_17678
bool CWorld::FindActorInCell(const float* position, std::function<bool(std::uint32_t)> predicate) const {
  const float key = static_cast<float>(this->terrain->width) * position[1] + position[0];
  const auto found = this->actorCells.find(key);

  if (found == this->actorCells.end()) {
    return false;
  }

  const std::list<std::uint32_t>& ids = found->second;
  return std::find_if(ids.begin(), ids.end(), predicate) != ids.end();
}

// OFFSET: 0x00ACA180
// FUNCTION: fn_17689
void CWorld::ClearActorCells() {
  this->actorCells.clear();
  this->unindexedActorIds.clear();
}
