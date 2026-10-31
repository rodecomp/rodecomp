#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/GuildMgr.h"

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

  if (actor->render.unknown_0070 == 1) {
    packet.resize(sizeof(ActorMoveEntryPacket));
    ActorMoveEntryPacket* entry = reinterpret_cast<ActorMoveEntryPacket*>(packet.data());
    entry->hp = actor->GetGaugeCurrent();
    entry->maxHp = actor->GetGaugeMaximum();
    entry->isBoss = actor->unknown_0304;
    entry->moveStartTime = actor->routeStartTick;

    int metadata = 0;
    float x = 0.0f;
    float y = 0.0f;
    actor->route.Sample(timeGetTime(), &x, &y, &metadata);

    int cellX;
    int subX;
    if (x > 0.0f) {
      const double position = x + 0.5;
      cellX = static_cast<int>(position);
      subX = static_cast<int>(static_cast<float>(position - cellX) * 16.0f);
    } else {
      cellX = 0;
      subX = 0;
    }

    int cellY;
    int subY;
    if (y > 0.0f) {
      const double position = y + 0.5;
      cellY = static_cast<int>(position);
      subY = static_cast<int>(static_cast<float>(position - cellY) * 16.0f);
    } else {
      cellY = 0;
      subY = 0;
    }

    const int destinationX = actor->unknown_0158[5];
    const int destinationY = *reinterpret_cast<const int*>(actor->unknown_0170);
    entry->moveData[0] = static_cast<std::uint8_t>(cellX >> 2);
    entry->moveData[1] = static_cast<std::uint8_t>((cellX << 6) | ((cellY >> 4) & 0x3F));
    entry->moveData[2] = static_cast<std::uint8_t>((cellY << 4) | ((destinationX >> 6) & 0x0F));
    entry->moveData[3] = static_cast<std::uint8_t>((destinationX << 2) | ((destinationY >> 8) & 0x03));
    entry->moveData[4] = static_cast<std::uint8_t>(destinationY);
    entry->moveData[5] = static_cast<std::uint8_t>((subX << 4) | (subY & 0x0F));

    entry->packetType = packetType;
    entry->packetLength = static_cast<std::uint16_t>(name.size() + packet.size());
    entry->objectType = actor->unknown_0314;
    entry->accountId = actor->render.accountId;
    entry->speed = *reinterpret_cast<const std::uint16_t*>(actor->unknown_0170 + 4);
    entry->bodyState = static_cast<std::uint16_t>(actor->bodyState);
    entry->healthState = static_cast<std::uint16_t>(actor->unknown_02C4);
    entry->effectState = actor->Slot13();
    entry->job = static_cast<std::uint16_t>(actor->GetEffectiveJob(-1));
    entry->head = *reinterpret_cast<const std::uint16_t*>(actor->unknown_029C);
    entry->guildId = actor->Slot50();
    entry->emblemVersion = static_cast<std::uint16_t>(g_guildMgr->GetEmblemVersion(actor->Slot50()));
    entry->isPkModeOn = actor->unknown_02C8 != 0;
    entry->sex = static_cast<std::uint8_t>(actor->unknown_0260);
    entry->xSize = static_cast<std::uint8_t>(actor->unknown_0294);
    entry->ySize = static_cast<std::uint8_t>(actor->unknown_0298);
    entry->level = actor->unknown_0210;
    entry->font = static_cast<std::uint16_t>(actor->render.unknown_00FC);
    entry->bodyPalette = static_cast<std::uint16_t>(actor->render.unknown_0048);

    if (actor->unknown_0314 == 0) {
      CPc* pc = static_cast<CPc*>(actor);
      entry->honor = static_cast<std::uint16_t>(pc->unknown_0430);
      entry->virtue = pc->unknown_0434;
      entry->headDirection = static_cast<std::uint16_t>(pc->unknown_043C);
      entry->weapon = pc->unknown_0434;
      entry->accessory = static_cast<std::uint16_t>(pc->unknown_0450);
      entry->accessory2 = static_cast<std::uint16_t>(pc->unknown_0454);
      entry->accessory3 = static_cast<std::uint16_t>(pc->unknown_0458);
      entry->headPalette = static_cast<std::uint16_t>(pc->unknown_0444);
      entry->robe = static_cast<std::uint16_t>(pc->unknown_049C);
      entry->characterId = pc->Slot42();
      entry->body = static_cast<std::uint16_t>(pc->Slot46());
    }
  } else {
    packet.resize(sizeof(ActorStandEntryPacket));
    ActorStandEntryPacket* entry = reinterpret_cast<ActorStandEntryPacket*>(packet.data());
    entry->hp = actor->GetGaugeCurrent();
    entry->maxHp = actor->GetGaugeMaximum();
    entry->isBoss = actor->unknown_0304;

    const int direction = (static_cast<int>(actor->render.angle) + 180) / 45 % 8;
    const int cellX = actor->unknown_0158[5];
    const int cellY = *reinterpret_cast<const int*>(actor->unknown_0170);
    entry->posDir[0] = static_cast<std::uint8_t>(cellX >> 2);
    entry->posDir[1] = static_cast<std::uint8_t>((cellX << 6) | ((cellY >> 4) & 0x3F));
    entry->posDir[2] = static_cast<std::uint8_t>((cellY << 4) | (direction & 0x0F));

    if (actor->render.unknown_0070 == 3) {
      entry->state = 1;
    } else {
      entry->state = actor->render.unknown_0070 == 6 ? 2 : 0;
    }

    entry->packetType = packetType;
    entry->packetLength = static_cast<std::uint16_t>(name.size() + packet.size());
    entry->objectType = actor->unknown_0314;
    entry->accountId = actor->render.accountId;
    entry->speed = *reinterpret_cast<const std::uint16_t*>(actor->unknown_0170 + 4);
    entry->bodyState = static_cast<std::uint16_t>(actor->bodyState);
    entry->healthState = static_cast<std::uint16_t>(actor->unknown_02C4);
    entry->effectState = actor->Slot13();
    entry->job = static_cast<std::uint16_t>(actor->GetEffectiveJob(-1));
    entry->head = *reinterpret_cast<const std::uint16_t*>(actor->unknown_029C);
    entry->guildId = actor->Slot50();
    entry->emblemVersion = static_cast<std::uint16_t>(g_guildMgr->GetEmblemVersion(actor->Slot50()));
    entry->isPkModeOn = actor->unknown_02C8 != 0;
    entry->sex = static_cast<std::uint8_t>(actor->unknown_0260);
    entry->xSize = static_cast<std::uint8_t>(actor->unknown_0294);
    entry->ySize = static_cast<std::uint8_t>(actor->unknown_0298);
    entry->level = actor->unknown_0210;
    entry->font = static_cast<std::uint16_t>(actor->render.unknown_00FC);
    entry->bodyPalette = static_cast<std::uint16_t>(actor->render.unknown_0048);
    entry->emblemVersion = static_cast<std::uint16_t>(g_guildMgr->GetEmblemVersion(actor->Slot50()));

    if (actor->unknown_0314 == 0) {
      CPc* pc = static_cast<CPc*>(actor);
      entry->honor = static_cast<std::uint16_t>(pc->unknown_0430);
      entry->virtue = pc->unknown_0434;
      entry->headDirection = static_cast<std::uint16_t>(pc->unknown_043C);
      entry->weapon = pc->unknown_0434;
      entry->accessory = static_cast<std::uint16_t>(pc->unknown_0450);
      entry->accessory2 = static_cast<std::uint16_t>(pc->unknown_0454);
      entry->accessory3 = static_cast<std::uint16_t>(pc->unknown_0458);
      entry->headPalette = static_cast<std::uint16_t>(pc->unknown_0444);
      entry->robe = static_cast<std::uint16_t>(pc->unknown_049C);
      entry->characterId = pc->Slot42();
      entry->body = static_cast<std::uint16_t>(pc->Slot46());
    }
  }

  packet.insert(packet.end(), name.begin(), name.end());
  return packet;
}
