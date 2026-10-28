#ifndef RAGNAROK_SOURCE_NETWORK_ACTORNAMEPACKET_H
#define RAGNAROK_SOURCE_NETWORK_ACTORNAMEPACKET_H

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)
struct ActorNameAllPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t actorId;
  /* 0006 */ char name[24];
  /* 001E */ char partyName[24];
  /* 0036 */ char guildName[24];
  /* 004E */ char positionName[24];
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ActorNameAllPacket, positionName) == 0x4E);
static_assert(sizeof(ActorNameAllPacket) == 0x66);
#endif

#endif
