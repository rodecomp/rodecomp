#ifndef RAGNAROK_SOURCE_CLIENT_MODE_CLOGINMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_CLOGINMODE_H

#include "Client/Mode/CMode.h"

#include <cstddef>
#include <cstdint>

struct CharacterListPacket;
struct CharacterSlotsPacket;
struct CharacterResultPacket;
struct CharacterDeletePacket;
struct WindowDialogs;

#pragma pack(push, 1)
struct CharacterInfo {
  /* 0000 */ std::uint32_t gid;
  /* 0004 */ unsigned char unknown_0004[0x86];
  /* 008A */ unsigned char slot;
  /* 008B */ unsigned char unknown_008B[0x13];
  /* 009E */ std::int32_t deleteDate;
  /* 00A2 */ unsigned char unknown_00A2[0x0D];
};

struct CharacterSlotExtension {
  /* 0000 */ unsigned char unknown_0000;
  /* 0001 */ std::uint32_t unknown_0001;
  /* 0005 */ std::uint32_t unknown_0005;
  /* 0009 */ std::uint32_t unknown_0009;
  /* 000D */ unsigned char unknown_000D[7];
};
#pragma pack(pop)

class CLoginMode : public CMode {
public:
  /* 0058 */ unsigned char unknown_0058[0x174];
  /* 01CC */ int characterCount;
  /* 01D0 */ unsigned char unknown_01D0[0x8];
  /* 01D8 */ int unknown_01D8;
  /* 01DC */ int unknown_01DC;
  /* 01E0 */ bool characterDeleted;
  /* 01E1 */ unsigned char unknown_01E1[0x3E87];
  /* 4068 */ CharacterInfo characters[68];
  /* 6EE4 */ unsigned char unknown_6EE4[0x18];
  /* 6EFC */ CharacterSlotExtension slotExtension;
  /* 6F10 */ unsigned char unknown_6F10[0x6C];

  void SubD8D4F0(int);
  int GetMaxCharacterSlot();
  void ReceiveCharacterSlots(const CharacterSlotsPacket* packet);
  void SubD8EDC0(int);
  void ReceiveCharacterResult(const CharacterResultPacket* packet, WindowDialogs& dialogs);
  void SubD8F560(int);
  void ReceiveCharacterList(const CharacterListPacket* packet);
  void ReceiveDeleteCharacter(const CharacterDeletePacket* packet, WindowDialogs& dialogs);
  void ReceiveDeleteCancel(const CharacterDeletePacket* packet, WindowDialogs& dialogs);
  void ReceiveDeleteReservation(const CharacterDeletePacket* packet, WindowDialogs& dialogs);
  void SubD91780(int);
};

int GetClientType();
const char* GetUserCountText(unsigned short userCount);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CharacterInfo) == 0xAF);
static_assert(offsetof(CharacterInfo, slot) == 0x8A);
static_assert(sizeof(CharacterSlotExtension) == 0x14);
static_assert(offsetof(CLoginMode, characterCount) == 0x1CC);
static_assert(offsetof(CLoginMode, unknown_01D8) == 0x1D8);
static_assert(offsetof(CLoginMode, characters) == 0x4068);
static_assert(offsetof(CLoginMode, slotExtension) == 0x6EFC);
static_assert(sizeof(CLoginMode) == 0x6F7C);
#endif

#endif
