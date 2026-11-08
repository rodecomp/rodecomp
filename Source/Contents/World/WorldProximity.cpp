#include "Contents/World/WorldProximity.h"
#include "Contents/World/World.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/World/TerrainResource.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <functional>
#include "Contents/GuildInfo.h"

#pragma pack(push, 1)
struct PACKET_CZ_APPROXIMATE_ACTOR {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::uint32_t accountId;
  /* 0006 */ std::uint16_t range;
  /* 0008 */ std::uint8_t enabled;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PACKET_CZ_APPROXIMATE_ACTOR) == 9);
static_assert(offsetof(PACKET_CZ_APPROXIMATE_ACTOR, accountId) == 2);
static_assert(offsetof(PACKET_CZ_APPROXIMATE_ACTOR, range) == 6);
#endif

// INLINED
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

// OFFSET: 0x00AC93C0
// FUNCTION: fn_17683
void CWorld::UpdateGuildMasterApproach(std::function<bool()> isNearby) {
  if (this->guildMasterNearby == true) {
    if (!isNearby()) {
      this->guildMasterNearby = false;
    }

    return;
  }

  if (isNearby() == true) {
    PACKET_CZ_APPROXIMATE_ACTOR packet;
    packet.packetType = ID_CZ_APPROXIMATE_ACTOR;
    packet.accountId = g_guildInfo.masterCharacterId;
    packet.range = 1;
    packet.enabled = 1;
    GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
    this->guildMasterNearby = true;
  }
}

// OFFSET: 0x00AC8A40
// FUNCTION: fn_17678
__declspec(noinline) bool CWorld::FindActorInCell(const float* position, std::function<bool(std::uint32_t)> predicate) const {
  const float key = static_cast<float>(this->terrain->width) * position[1] + position[0];
  const auto found = this->actorCells.find(key);

  if (found == this->actorCells.end()) {
    return false;
  }

  const std::list<std::uint32_t>& ids = found->second;
  return std::find_if(ids.begin(), ids.end(), predicate) != ids.end();
}

// OFFSET: 0x00AC8BA0
// FUNCTION: fn_17679
bool CWorld::IsGuildMaster(std::uint32_t aid) {
  for (CRenderObject* actor : this->actors) {
    if (actor->render.accountId == aid) {
      CGameActor* gameActor = static_cast<CGameActor*>(actor);
      if (!gameActor) {
        return false;
      }

      return gameActor->GetPkState() == static_cast<int>(g_guildInfo.masterCharacterId);
    }
  }

  return false;
}

// OFFSET: 0x00AC8750
// FUNCTION: fn_17677
bool CWorld::IsExistGuildMasterNearby(const float& x, const float& z) {
  if (!this->player || this->player->GetGdid() < 1) {
    return false;
  }

  if (!this->actorCells.empty()) {
    for (float cellX = x - static_cast<float>(this->nearbyRange); cellX <= static_cast<float>(this->nearbyRange) + x; cellX += 1.0f) {
      for (float cellZ = z - static_cast<float>(this->nearbyRange); cellZ <= static_cast<float>(this->nearbyRange) + z; cellZ += 1.0f) {
        const float position[2] = {cellX, cellZ};
        if (this->FindActorInCell(position, [this](std::uint32_t aid) { return this->IsGuildMaster(aid); }) == true) {
          return true;
        }
      }
    }
  }

  for (std::uint32_t aid : this->unindexedActorIds) {
    if (!this->IsGuildMaster(aid)) {
      continue;
    }

    CRenderObject* actor = g_session.FindGameActor(aid);
    if (actor->render.state == 1) {
      float centerX = 0.0f;
      float centerZ = 0.0f;
      int column = 0;
      int row = 0;
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->WorldCell(actor->render.position.x + 0.5f, actor->render.position.z + 0.5f, column, row);
      mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->CellCenter(column, row, centerX, centerZ);
      return std::fabs(centerX - x) <= static_cast<float>(this->nearbyRange) && std::fabs(centerZ - z) <= static_cast<float>(this->nearbyRange);
    }

    return std::fabs(actor->render.position.x - x) <= static_cast<float>(this->nearbyRange) &&
      std::fabs(actor->render.position.z - z) <= static_cast<float>(this->nearbyRange);
  }

  return false;
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

      if (notify && this->guildMasterNearby) {
        if (this->IsGuildMaster(aid)) {
          this->UpdateGuildMasterApproach([this]() { return false; });
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

    if (notify && this->guildMasterNearby) {
      if (this->IsGuildMaster(aid)) {
        this->UpdateGuildMasterApproach([this]() { return false; });
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
// FUNCTION: fn_17719
bool WorldProximityGuildMasterCallback::Invoke(const std::uint32_t& aidRef) {
  const std::uint32_t aid = aidRef;
  auto it = std::find_if(this->world->actors.begin(), this->world->actors.end(), [aid](CRenderObject* actor) {
    return actor->render.accountId == aid;
  });
  if (it == this->world->actors.end()) {
    return false;
  }

  CGameActor* gameActor = static_cast<CGameActor*>(*it);
  if (!gameActor) {
    return false;
  }

  if (static_cast<int>(g_guildInfo.masterCharacterId) != gameActor->GetPkState()) {
    return false;
  }

  return true;
}

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
// FUNCTION: fn_17720
bool WorldProximityRangeCallback::Invoke() {
  return std::fabs(this->cellX - this->playerX) <= static_cast<float>(this->range) &&
    std::fabs(this->cellZ - this->playerZ) <= static_cast<float>(this->range);
}

// OFFSET: 0x00AC9D50
// FUNCTION: fn_17686
__declspec(noinline) void CWorld::PcApproachToPlayer(std::uint32_t aid, const CellFloatPos& pos) {
  if (this->player && this->player->GetGdid() > 0 && this->IsGuildMaster(aid)) {
    auto isNearby = [](const CellFloatPos& cell, const CellFloatPos& player, int range) {
      return std::fabs(cell.x - player.x) <= static_cast<float>(range) && std::fabs(cell.z - player.z) <= static_cast<float>(range);
    };

    const CellFloatPos player = {this->player->render.position.x, this->player->render.position.z};
    this->UpdateGuildMasterApproach(std::bind(isNearby, pos, player, this->nearbyRange));
  }
}

// OFFSET: 0x00AC9E70
// FUNCTION: fn_17687
void CWorld::PlayerMove(const float& x, const float& z) {
  auto isNearby = [this](const float& cellX, const float& cellZ) {
    return this->IsExistGuildMasterNearby(cellX, cellZ);
  };

  this->UpdateGuildMasterApproach(std::bind(isNearby, x, z));
}

// OFFSET: 0x00ACA9F0
// FUNCTION: fn_17699
__declspec(noinline) void CWorld::AddPcPos(std::uint32_t aid, const CellFloatPos& cell) {
  this->RemovePcPos(aid, false);

  for (auto entry = this->unindexedActorIds.begin(); entry != this->unindexedActorIds.end(); ++entry) {
    if (*entry != aid) {
      continue;
    }

    this->unindexedActorIds.erase(entry);
    break;
  }

  const float key = static_cast<float>(this->terrain->width) * cell.z + cell.x;
  this->actorCells[key].emplace_back(aid);
  this->PcApproachToPlayer(aid, cell);
}

// OFFSET: 0x00ACAB90
// FUNCTION: fn_17700
void CWorld::AddPcPosFromCell(std::uint32_t aid, const int* cell) {
  CellFloatPos pos;
  pos.x = 0.0f;
  pos.z = 0.0f;
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(cell[0], cell[1], pos.x, pos.z);
  this->AddPcPos(aid, pos);
}

// OFFSET: 0x00ACABE0
// FUNCTION: fn_17701
void CWorld::AddPcPosUnindexed(std::uint32_t aid, const CellFloatPos& cell) {
  this->RemovePcPos(aid, false);

  for (auto entry = this->unindexedActorIds.begin(); entry != this->unindexedActorIds.end(); ++entry) {
    if (*entry != aid) {
      continue;
    }

    this->unindexedActorIds.erase(entry);
    break;
  }

  this->unindexedActorIds.emplace_back(aid);
  this->PcApproachToPlayer(aid, cell);
}
