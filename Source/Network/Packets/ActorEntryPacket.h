#ifndef RAGNAROK_SOURCE_NETWORK_PACKETS_ACTORENTRYPACKET_H
#define RAGNAROK_SOURCE_NETWORK_PACKETS_ACTORENTRYPACKET_H

#include <cstdint>

#pragma pack(push, 1)
struct ActorMoveEntryNamedPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ short speed;
  /* 000B */ short bodyState;
  /* 000D */ short healthState;
  /* 000F */ int effectState;
  /* 0013 */ short job;
  /* 0015 */ short head;
  /* 0017 */ std::int64_t weapon;
  /* 001F */ short accessory;
  /* 0021 */ std::uint32_t moveStartTime;
  /* 0025 */ short accessory2;
  /* 0027 */ short accessory3;
  /* 0029 */ short headPalette;
  /* 002B */ short bodyPalette;
  /* 002D */ short headDir;
  /* 002F */ short robe;
  /* 0031 */ int guildId;
  /* 0035 */ short guildVersion;
  /* 0037 */ short honor;
  /* 0039 */ int virtue;
  /* 003D */ unsigned char pkMode;
  /* 003E */ unsigned char sex;
  /* 003F */ unsigned char moveData[6];
  /* 0045 */ unsigned char xSize;
  /* 0046 */ unsigned char ySize;
  /* 0047 */ short level;
  /* 0049 */ short font;
};

struct ActorStateEntryNamedOldPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ short speed;
  /* 000B */ short bodyState;
  /* 000D */ short healthState;
  /* 000F */ int effectState;
  /* 0013 */ short job;
  /* 0015 */ short head;
  /* 0017 */ std::int64_t weapon;
  /* 001F */ short accessory;
  /* 0021 */ short accessory2;
  /* 0023 */ short accessory3;
  /* 0025 */ short headPalette;
  /* 0027 */ short bodyPalette;
  /* 0029 */ short headDir;
  /* 002B */ short robe;
  /* 002D */ int guildId;
  /* 0031 */ short guildVersion;
  /* 0033 */ short honor;
  /* 0035 */ int virtue;
  /* 0039 */ unsigned char pkMode;
  /* 003A */ unsigned char sex;
  /* 003B */ unsigned char posDir[3];
  /* 003E */ unsigned char xSize;
  /* 003F */ unsigned char ySize;
  /* 0040 */ unsigned char state;
  /* 0041 */ short level;
  /* 0043 */ short font;
};
#pragma pack(pop)

static_assert(sizeof(ActorMoveEntryNamedPacket) == 0x4B);
static_assert(sizeof(ActorStateEntryNamedOldPacket) == 0x45);

#endif
