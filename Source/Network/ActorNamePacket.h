#ifndef RAGNAROK_SOURCE_NETWORK_ACTORNAMEPACKET_H
#define RAGNAROK_SOURCE_NETWORK_ACTORNAMEPACKET_H

#include <cstddef>
#include <cstdint>
#include <string>

#pragma pack(push, 1)
struct ActorNameAllPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t actorId;
  /* 0006 */ char name[24];
  /* 001E */ char partyName[24];
  /* 0036 */ char guildName[24];
  /* 004E */ char positionName[24];
};

struct ActorNameTitlePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t actorId;
  /* 0006 */ std::uint32_t groupId;
  /* 000A */ char name[24];
  /* 0022 */ char title[24];
};

struct PlayerNameUpdatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t type;
  /* 0004 */ std::uint32_t characterId;
  /* 0008 */ char name[24];
};
#pragma pack(pop)

void SetPlayerName(std::uint32_t id, std::string name);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ActorNameAllPacket, positionName) == 0x4E);
static_assert(sizeof(ActorNameAllPacket) == 0x66);
static_assert(offsetof(ActorNameTitlePacket, title) == 0x22);
static_assert(sizeof(ActorNameTitlePacket) == 0x3A);
static_assert(sizeof(PlayerNameUpdatePacket) == 0x20);
#endif

#endif
