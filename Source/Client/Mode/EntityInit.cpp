#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Guild/GuildMgr.h"
#include "Network/Packets/ActorEntryPacket.h"
#include "Network/Packets/PacketId.h"

#include <cstdint>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>

namespace {
#pragma pack(push, 1)
  struct ActorMoveEntryPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t packetLength;
    /* 0004 */ std::uint8_t objectType;
    /* 0005 */ std::uint32_t accountId;
    /* 0009 */ std::uint32_t characterId;
    /* 000D */ std::uint16_t speed;
    /* 000F */ std::uint16_t bodyState;
    /* 0011 */ std::uint16_t healthState;
    /* 0013 */ std::uint32_t effectState;
    /* 0017 */ std::uint16_t job;
    /* 0019 */ std::uint16_t head;
    /* 001B */ std::int64_t weapon;
    /* 0023 */ std::uint16_t accessory;
    /* 0025 */ std::uint32_t moveStartTime;
    /* 0029 */ std::uint16_t accessory2;
    /* 002B */ std::uint16_t accessory3;
    /* 002D */ std::uint16_t headPalette;
    /* 002F */ std::uint16_t bodyPalette;
    /* 0031 */ std::uint16_t headDirection;
    /* 0033 */ std::uint16_t robe;
    /* 0035 */ std::uint32_t guildId;
    /* 0039 */ std::uint16_t emblemVersion;
    /* 003B */ std::uint16_t honor;
    /* 003D */ std::uint32_t virtue;
    /* 0041 */ std::uint8_t isPkModeOn;
    /* 0042 */ std::uint8_t sex;
    /* 0043 */ std::uint8_t moveData[6];
    /* 0049 */ std::uint8_t xSize;
    /* 004A */ std::uint8_t ySize;
    /* 004B */ std::uint16_t level;
    /* 004D */ std::uint16_t font;
    /* 004F */ std::int32_t maxHp;
    /* 0053 */ std::int32_t hp;
    /* 0057 */ std::uint8_t isBoss;
    /* 0058 */ std::uint16_t body;
  };

  struct ActorStandEntryPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t packetLength;
    /* 0004 */ std::uint8_t objectType;
    /* 0005 */ std::uint32_t accountId;
    /* 0009 */ std::uint32_t characterId;
    /* 000D */ std::uint16_t speed;
    /* 000F */ std::uint16_t bodyState;
    /* 0011 */ std::uint16_t healthState;
    /* 0013 */ std::uint32_t effectState;
    /* 0017 */ std::uint16_t job;
    /* 0019 */ std::uint16_t head;
    /* 001B */ std::int64_t weapon;
    /* 0023 */ std::uint16_t accessory;
    /* 0025 */ std::uint16_t accessory2;
    /* 0027 */ std::uint16_t accessory3;
    /* 0029 */ std::uint16_t headPalette;
    /* 002B */ std::uint16_t bodyPalette;
    /* 002D */ std::uint16_t headDirection;
    /* 002F */ std::uint16_t robe;
    /* 0031 */ std::uint32_t guildId;
    /* 0035 */ std::uint16_t emblemVersion;
    /* 0037 */ std::uint16_t honor;
    /* 0039 */ std::uint32_t virtue;
    /* 003D */ std::uint8_t isPkModeOn;
    /* 003E */ std::uint8_t sex;
    /* 003F */ std::uint8_t posDir[3];
    /* 0042 */ std::uint8_t xSize;
    /* 0043 */ std::uint8_t ySize;
    /* 0044 */ std::uint8_t state;
    /* 0045 */ std::uint16_t level;
    /* 0047 */ std::uint16_t font;
    /* 0049 */ std::int32_t maxHp;
    /* 004D */ std::int32_t hp;
    /* 0051 */ std::uint8_t isBoss;
    /* 0052 */ std::uint16_t body;
  };
#pragma pack(pop)

  static_assert(sizeof(ActorMoveEntryPacket) == 0x5A);
  static_assert(sizeof(ActorStandEntryPacket) == 0x54);
}

// OFFSET: 0x00CD47F0
// FUNCTION: fn_21275
std::vector<char> CGameMode::BuildActorEntryPacket(CGameActor* actor, std::uint16_t packetType) {
  std::vector<char> packet;
  std::string name = this->nameCard.Name(actor->render.accountId);

  if (actor->render.state == ST_MOVE) {
    packet.resize(sizeof(ActorMoveEntryPacket), 0);
    ActorMoveEntryPacket* entry = reinterpret_cast<ActorMoveEntryPacket*>(packet.data());
    entry->hp = actor->GetGaugeCurrent();
    entry->maxHp = actor->GetGaugeMaximum();
    entry->isBoss = actor->hpGaugeFlags;
    entry->moveStartTime = actor->routeStartTick;

    int metadata = 0;
    float x = 0.0f;
    float y = 0.0f;
    const DWORD now = timeGetTime();
    actor->route.Sample(now, &x, &y, &metadata);

    int cellX;
    int subX;
    if (x <= 0.0f) {
      cellX = 0;
      subX = 0;
    } else {
      const double position = x + 0.5;
      cellX = static_cast<int>(position);
      subX = static_cast<int>(static_cast<float>(position - cellX) * 16.0f);
    }

    int cellY;
    int subY;
    if (y <= 0.0f) {
      cellY = 0;
      subY = 0;
    } else {
      const double position = y + 0.5;
      cellY = static_cast<int>(position);
      subY = static_cast<int>(static_cast<float>(position - cellY) * 16.0f);
    }

    const int destinationX = actor->movementState[5];
    const int destinationY = actor->destCellY;
    entry->moveData[0] = static_cast<std::uint8_t>(cellX >> 2);
    entry->moveData[1] = static_cast<std::uint8_t>((cellX << 6) | ((cellY >> 4) & 0x3F));
    entry->moveData[2] = static_cast<std::uint8_t>((cellY << 4) | ((destinationX >> 6) & 0x0F));
    entry->moveData[3] = static_cast<std::uint8_t>((destinationX << 2) | ((destinationY >> 8) & 0x03));
    entry->moveData[4] = static_cast<std::uint8_t>(destinationY);
    entry->moveData[5] = static_cast<std::uint8_t>((subX << 4) | (subY & 0x0F));

    entry->packetType = packetType;
    entry->packetLength = static_cast<std::uint16_t>(packet.size() + name.size());
    entry->objectType = actor->objectType;
    entry->accountId = actor->render.accountId;
    entry->speed = static_cast<std::uint16_t>(actor->moveSpeed);
    entry->bodyState = static_cast<std::uint16_t>(actor->bodyState);
    entry->healthState = static_cast<std::uint16_t>(actor->effectState);
    entry->effectState = actor->GetStateFlags();
    entry->job = static_cast<std::uint16_t>(actor->GetEffectiveJob(-1));
    entry->head = static_cast<std::uint16_t>(actor->headType);
    entry->guildId = actor->GetGdid();
    {
      CGuildMgr* guildMgr = g_guildMgr;
      entry->emblemVersion = static_cast<std::uint16_t>(guildMgr->GetEmblemVersion(actor->GetGdid()));
    }
    entry->isPkModeOn = actor->healthState != 0;
    entry->sex = static_cast<std::uint8_t>(actor->sex);
    entry->xSize = static_cast<std::uint8_t>(actor->xSize);
    entry->ySize = static_cast<std::uint8_t>(actor->ySize);
    entry->level = actor->characterLevel;
    entry->font = static_cast<std::uint16_t>(actor->render.fontSequenceState);
    entry->bodyPalette = static_cast<std::uint16_t>(actor->render.bodyPalette);

    if (actor->objectType == 0) {
      CPc* pc = static_cast<CPc*>(actor);
      entry->honor = static_cast<std::uint16_t>(pc->honor);
      entry->virtue = pc->virtue;
      entry->headDirection = static_cast<std::uint16_t>(pc->headDir);
      entry->weapon = pc->virtue;
      entry->accessory = static_cast<std::uint16_t>(pc->accessory);
      entry->accessory2 = static_cast<std::uint16_t>(pc->accessory2);
      entry->accessory3 = static_cast<std::uint16_t>(pc->accessory3);
      entry->headPalette = static_cast<std::uint16_t>(pc->headPalette);
      entry->robe = static_cast<std::uint16_t>(pc->robe);
      entry->characterId = pc->GetPkState();
      entry->body = static_cast<std::uint16_t>(pc->GetBodyCostume());
    }
  } else {
    packet.resize(sizeof(ActorStandEntryPacket), 0);
    ActorStandEntryPacket* entry = reinterpret_cast<ActorStandEntryPacket*>(packet.data());
    entry->hp = actor->GetGaugeCurrent();
    entry->maxHp = actor->GetGaugeMaximum();
    entry->isBoss = actor->hpGaugeFlags;

    const int direction = (static_cast<int>(actor->render.angle) + 180) / 45 % 8;
    const int cellX = actor->movementState[5];
    const int cellY = actor->destCellY;
    entry->posDir[0] = static_cast<std::uint8_t>(cellX >> 2);
    entry->posDir[1] = static_cast<std::uint8_t>((cellX << 6) | ((cellY >> 4) & 0x3F));
    entry->posDir[2] = static_cast<std::uint8_t>((cellY << 4) | (direction & 0x0F));

    if (actor->render.state == ST_DEAD) {
      entry->state = 1;
    } else if (actor->render.state == ST_SIT) {
      entry->state = 2;
    } else {
      entry->state = 0;
    }

    entry->packetType = packetType;
    entry->packetLength = static_cast<std::uint16_t>(packet.size() + name.size());
    entry->objectType = actor->objectType;
    entry->accountId = actor->render.accountId;
    entry->speed = static_cast<std::uint16_t>(actor->moveSpeed);
    entry->bodyState = static_cast<std::uint16_t>(actor->bodyState);
    entry->healthState = static_cast<std::uint16_t>(actor->effectState);
    entry->effectState = actor->GetStateFlags();
    entry->job = static_cast<std::uint16_t>(actor->GetEffectiveJob(-1));
    entry->head = static_cast<std::uint16_t>(actor->headType);
    entry->guildId = actor->GetGdid();
    {
      CGuildMgr* guildMgr = g_guildMgr;
      entry->emblemVersion = static_cast<std::uint16_t>(guildMgr->GetEmblemVersion(actor->GetGdid()));
    }
    entry->isPkModeOn = actor->healthState != 0;
    entry->sex = static_cast<std::uint8_t>(actor->sex);
    entry->xSize = static_cast<std::uint8_t>(actor->xSize);
    entry->ySize = static_cast<std::uint8_t>(actor->ySize);
    entry->level = actor->characterLevel;
    entry->font = static_cast<std::uint16_t>(actor->render.fontSequenceState);
    entry->bodyPalette = static_cast<std::uint16_t>(actor->render.bodyPalette);
    {
      CGuildMgr* guildMgr = g_guildMgr;
      entry->emblemVersion = static_cast<std::uint16_t>(guildMgr->GetEmblemVersion(actor->GetGdid()));
    }

    if (actor->objectType == 0) {
      CPc* pc = static_cast<CPc*>(actor);
      entry->honor = static_cast<std::uint16_t>(pc->honor);
      entry->virtue = pc->virtue;
      entry->headDirection = static_cast<std::uint16_t>(pc->headDir);
      entry->weapon = pc->virtue;
      entry->accessory = static_cast<std::uint16_t>(pc->accessory);
      entry->accessory2 = static_cast<std::uint16_t>(pc->accessory2);
      entry->accessory3 = static_cast<std::uint16_t>(pc->accessory3);
      entry->headPalette = static_cast<std::uint16_t>(pc->headPalette);
      entry->robe = static_cast<std::uint16_t>(pc->robe);
      entry->characterId = pc->GetPkState();
      entry->body = static_cast<std::uint16_t>(pc->GetBodyCostume());
    }
  }

  packet.insert(packet.end(), name.begin(), name.end());
  return packet;
}

namespace {
  // INLINED
  template <typename Entry>
  void FillLegacyActorEntry(Entry& entry, CGameActor* actor, std::uint16_t packetType, const std::string& name) {
    entry.packetType = packetType;
    entry.packetLength = static_cast<short>(sizeof(Entry) + name.size());
    entry.objectType = actor->objectType;
    entry.aid = actor->render.accountId;
    entry.speed = static_cast<short>(actor->moveSpeed);
    entry.bodyState = static_cast<short>(actor->bodyState);
    entry.healthState = static_cast<short>(actor->effectState);
    entry.effectState = actor->GetStateFlags();
    entry.job = static_cast<short>(actor->GetEffectiveJob(-1));
    entry.head = static_cast<short>(actor->headType);
    entry.guildId = actor->GetGdid();
    CGuildMgr* guildMgr = g_guildMgr;
    entry.guildVersion = static_cast<short>(guildMgr->GetEmblemVersion(actor->GetGdid()));
    entry.pkMode = actor->healthState != 0;
    entry.sex = static_cast<unsigned char>(actor->sex);
    entry.xSize = static_cast<unsigned char>(actor->xSize);
    entry.ySize = static_cast<unsigned char>(actor->ySize);
    entry.level = actor->characterLevel;
    entry.font = static_cast<short>(actor->render.fontSequenceState);
    entry.bodyPalette = static_cast<short>(actor->render.bodyPalette);
    guildMgr = g_guildMgr;
    entry.guildVersion = static_cast<short>(guildMgr->GetEmblemVersion(actor->GetGdid()));
  }

  // INLINED
  template <typename Entry>
  void FillLegacyPcAppearance(Entry& entry, const CPc* pc) {
    entry.honor = static_cast<short>(pc->honor);
    entry.virtue = pc->virtue;
    entry.headDir = static_cast<short>(pc->headDir);
    const int equipment = pc->virtue;
    entry.weapon = equipment;
    entry.accessory = static_cast<short>(pc->accessory);
    entry.accessory2 = static_cast<short>(pc->accessory2);
    entry.accessory3 = static_cast<short>(pc->accessory3);
    entry.headPalette = static_cast<short>(pc->headPalette);
    entry.robe = static_cast<short>(pc->robe);
  }

  // INLINED
  void SplitMovementCoordinate(float value, int& cell, int& subcell) {
    if (value <= 0.0f) {
      cell = 0;
      subcell = 0;
    } else {
      const double rounded = static_cast<double>(value) + 0.5;
      cell = static_cast<int>(rounded);
      subcell = static_cast<int>(static_cast<float>(rounded - cell) * 16.0f);
    }
  }
}

// OFFSET: 0x00CD41E0
// FUNCTION: fn_46326
std::vector<char> CGameMode::BuildLegacyActorEntryPacket(CGameActor* actor) {
  std::vector<char> packet;
  const std::string name = this->nameCard.Name(actor->render.accountId);
  if (actor->render.state == ST_MOVE) {
    packet.resize(sizeof(ActorMoveEntryNamedPacket), 0);
    ActorMoveEntryNamedPacket& entry = *reinterpret_cast<ActorMoveEntryNamedPacket*>(packet.data());
    FillLegacyActorEntry(entry, actor, ID_ZC_NOTIFY_MOVEENTRY8, name);
    entry.moveStartTime = actor->routeStartTick;
    float x = 0.0f;
    float y = 0.0f;
    int metadata = 0;
    const DWORD now = timeGetTime();
    actor->route.Sample(now, &x, &y, &metadata);

    int cellX, cellY, subX, subY;
    SplitMovementCoordinate(x, cellX, subX);
    SplitMovementCoordinate(y, cellY, subY);
    const int destinationX = actor->movementState[5];
    const int destinationY = actor->destCellY;
    entry.moveData[0] = static_cast<unsigned char>(cellX >> 2);
    entry.moveData[1] = static_cast<unsigned char>((static_cast<unsigned int>(cellX) << 6) | ((cellY >> 4) & 0x3F));
    entry.moveData[2] = static_cast<unsigned char>((static_cast<unsigned int>(cellY) << 4) | ((destinationX >> 6) & 0x0F));
    entry.moveData[3] = static_cast<unsigned char>((destinationX << 2) | ((destinationY >> 8) & 3));
    entry.moveData[4] = static_cast<unsigned char>(destinationY);
    entry.moveData[5] = static_cast<unsigned char>((static_cast<unsigned int>(subX) << 4) | (subY & 0x0F));
    if (actor->objectType == 0) {
      FillLegacyPcAppearance(entry, static_cast<const CPc*>(actor));
    }

    packet.insert(packet.end(), name.begin(), name.end());
  } else {
    packet.resize(sizeof(ActorStateEntryNamedOldPacket), 0);
    ActorStateEntryNamedOldPacket& entry = *reinterpret_cast<ActorStateEntryNamedOldPacket*>(packet.data());
    FillLegacyActorEntry(entry, actor, ID_ZC_NOTIFY_STANDENTRY7, name);
    const int direction = (static_cast<int>(actor->render.angle) + 180) / 45 % 8;
    const int cellX = actor->movementState[5];
    const int cellY = actor->destCellY;
    entry.posDir[0] = static_cast<unsigned char>(cellX >> 2);
    entry.posDir[1] = static_cast<unsigned char>((cellX << 6) | ((cellY >> 4) & 0x3F));
    entry.posDir[2] = static_cast<unsigned char>((cellY << 4) | (direction & 0x0F));
    if (actor->render.state == ST_DEAD) {
      entry.state = 1;
    } else if (actor->render.state == ST_SIT) {
      entry.state = 2;
    } else {
      entry.state = 0;
    }
    if (actor->objectType == 0) {
      FillLegacyPcAppearance(entry, static_cast<const CPc*>(actor));
    }

    packet.insert(packet.end(), name.begin(), name.end());
  }

  return packet;
}
