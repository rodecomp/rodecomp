#include "Contents/World/WorldProximity.h"
#include "Contents/World/World.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/World/TerrainResource.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <functional>

// GLOBAL: gl_25002
std::uint32_t g_proximityCompareId;

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

static void CallWorldProximityUpdate(CWorld* world, std::function<bool()> nearby) {
  if (world->unknown_00F4) {
    if (!nearby()) {
      world->unknown_00F4 = false;
    }
    return;
  }

  if (nearby()) {
    unsigned char packet[9];
    const std::uint16_t id = 0x0BB0;
    const std::uint16_t enabled = 1;
    std::memcpy(packet, &id, sizeof(id));
    std::memcpy(packet + 2, &g_proximityCompareId, sizeof(g_proximityCompareId));
    std::memcpy(packet + 6, &enabled, sizeof(enabled));
    packet[8] = 1;
    GetRagConnection().Send(g_packetLengths.FixedLength(id), packet);
    world->unknown_00F4 = true;
  }
}

static bool ProximityCompareAidMatches(CWorld* world, std::uint32_t aid) {
  for (CRenderObject* actor : world->actors) {
    if (actor->render.accountId != aid) {
      continue;
    }

    if (actor == nullptr) {
      return false;
    }

    return static_cast<CGameActor*>(actor)->Slot42() == static_cast<int>(g_proximityCompareId);
  }

  return false;
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

// OFFSET: 0x00ACA670
// FUNCTION: fn_17695
void CWorld::RemovePcPos(std::uint32_t aid, bool notify) {
  for (auto cell = this->actorCells.begin(); cell != this->actorCells.end(); ++cell) {
    std::list<std::uint32_t>& ids = cell->second;

    for (auto entry = ids.begin(); entry != ids.end(); ++entry) {
      if (*entry != aid) {
        continue;
      }

      if (notify && this->unknown_00F4) {
        if (ProximityCompareAidMatches(this, aid)) {
          CallWorldProximityUpdate(this, std::function<bool()>([]() { return false; }));
        }
      }

      ids.erase(entry);
      return;
    }
  }
}

// OFFSET: 0x00ACA7B0
// FUNCTION: fn_17696
void CWorld::RemovePcPosForMove(std::uint32_t aid, bool notify) {
  for (auto entry = this->unindexedActorIds.begin(); entry != this->unindexedActorIds.end(); ++entry) {
    if (*entry != aid) {
      continue;
    }

    if (notify && this->unknown_00F4) {
      if (ProximityCompareAidMatches(this, aid)) {
        CallWorldProximityUpdate(this, std::function<bool()>([]() { return false; }));
      }
    }

    this->unindexedActorIds.erase(entry);
    return;
  }
}

struct WorldProximityGuildMasterCallback {
  void* unknown_0000;
  CWorld* world;

  bool Invoke(const std::uint32_t& aid);
};

// OFFSET: 0x00ACBA10
// FUNCTION: fn_17717
bool WorldProximityRejectA() {
  return false;
}

// OFFSET: 0x00ACBA20
// FUNCTION: fn_17718
bool WorldProximityRejectB() {
  return false;
}

// OFFSET: 0x00ACBA30

struct WorldProximityRangeCallback {
  void* unknown_0000;
  int range;
  float playerX;
  float playerZ;
  float cellX;
  float cellZ;

  bool Invoke();
};

// OFFSET: 0x00ACBA80
