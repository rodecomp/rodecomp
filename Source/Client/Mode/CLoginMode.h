#ifndef RAGNAROK_SOURCE_CLIENT_MODE_CLOGINMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_CLOGINMODE_H

#include "Client/Mode/CMode.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct CharacterListPacket;
struct CharacterSlotsPacket;
struct CharacterResultPacket;
struct CharacterDeletePacket;
struct AcceptEnterPacket;
struct MapStatePacket;
struct WindowDialogs;
struct LoginErrorPacket;
struct LoginErrorDetailPacket;
struct CharacterPagePacket;
struct RefuseLoginPacket;

#pragma pack(push, 1)
struct CharacterInfo {
  /* 0000 */ std::uint32_t gid;
  /* 0004 */ unsigned char unknown_0004[0x50];
  /* 0054 */ std::int16_t job;
  /* 0056 */ unsigned char unknown_0056[0x2];
  /* 0058 */ std::int16_t unknown_0058;
  /* 005A */ unsigned char unknown_005A[0x10];
  /* 006A */ std::int16_t bodyStyle;
  /* 006C */ unsigned char unknown_006C[0x1E];
  /* 008A */ unsigned char slot;
  /* 008B */ unsigned char unknown_008B[0x3];
  /* 008E */ char mapName[16];
  /* 009E */ std::int32_t deleteDate;
  /* 00A2 */ unsigned char unknown_00A2[0x0C];
  /* 00AE */ unsigned char sex;
};

struct CharacterSlotExtension {
  /* 0000 */ unsigned char unknown_0000;
  /* 0001 */ std::uint32_t unknown_0001;
  /* 0005 */ std::uint32_t unknown_0005;
  /* 0009 */ std::uint32_t unknown_0009;
  /* 000D */ unsigned char unknown_000D[7];
};
#pragma pack(pop)

struct MapStateData {
  /* 0000 */ std::uint32_t values[4];
};

struct MapStateEntry {
  /* 0000 */ int index;
  /* 0004 */ MapStateData data;
};

class CLoginMode : public CMode {
public:
  /* 0058 */ unsigned char unknown_0058[0x174];
  /* 01CC */ int characterCount;
  /* 01D0 */ unsigned char unknown_01D0[0x8];
  /* 01D8 */ int unknown_01D8;
  /* 01DC */ int unknown_01DC;
  /* 01E0 */ bool characterDeleted;
  /* 01E1 */ unsigned char unknown_01E1[0x3];
  /* 01E4 */ std::uint32_t otpStarted;
  /* 01E8 */ unsigned char unknown_01E8[0x3E80];
  /* 4068 */ CharacterInfo characters[68];
  /* 6EE4 */ unsigned char unknown_6EE4[0x18];
  /* 6EFC */ CharacterSlotExtension slotExtension;
  /* 6F10 */ unsigned char unknown_6F10[0x40];
  /* 6F50 */ std::string unknown_6F50;
  /* 6F68 */ std::vector<MapStateEntry> mapStates;
  /* 6F74 */ bool unknown_6F74;
  /* 6F75 */ bool unknown_6F75;
  /* 6F76 */ bool unknown_6F76;
  /* 6F77 */ unsigned char unknown_6F77[0x5];

  int Run() override;
  void End() override;

  void SubD8D4F0(int);
  int GetMaxCharacterSlot();
  void ReceiveCharacterSlots(const CharacterSlotsPacket* packet);
  void ReceiveAcceptEnter(const AcceptEnterPacket* packet, WindowDialogs& dialogs);
  void SubD8EDC0(int);
  void ReceiveCharacterResult(const CharacterResultPacket* packet, WindowDialogs& dialogs);
  void SubD8F560(int);
  void ReceiveCharacterPage(const CharacterPagePacket* packet);
  void ReceiveCharacterList(const CharacterListPacket* packet);
  void ReceiveDeleteCharacter(const CharacterDeletePacket* packet, WindowDialogs& dialogs);
  void ReceiveDeleteCancel(const CharacterDeletePacket* packet, WindowDialogs& dialogs);
  void ReceiveDeleteReservation(const CharacterDeletePacket* packet, WindowDialogs& dialogs);
  void ReceiveMapStates(const MapStatePacket* packet, WindowDialogs& dialogs);
  void SubD91780(int);
  void ReceiveRefuseLogin(const RefuseLoginPacket* packet);
  void SubD97B20(int);
  void SubD97EF0(int);
  void SubD97F00(int);
  void ReceiveLoginErrorDetail(const LoginErrorDetailPacket* packet);
  void ReceiveLoginError(const LoginErrorPacket* packet);
  void ReceiveBillingInfo(const unsigned char* packet);
  void ReceiveNotifyBan(const unsigned char* packet);
  void SubD9E170(int);
};

extern bool g_loginModeFlag;

int GetClientType();
std::string GetMacAddress();
const char* GetUserCountText(unsigned short userCount);
void OpenAccountRegistration();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CharacterInfo) == 0xAF);
static_assert(offsetof(CharacterInfo, slot) == 0x8A);
static_assert(offsetof(CharacterInfo, mapName) == 0x8E);
static_assert(offsetof(CharacterInfo, sex) == 0xAE);
static_assert(sizeof(MapStateEntry) == 0x14);
static_assert(sizeof(CharacterSlotExtension) == 0x14);
static_assert(offsetof(CLoginMode, characterCount) == 0x1CC);
static_assert(offsetof(CLoginMode, unknown_01D8) == 0x1D8);
static_assert(offsetof(CLoginMode, characters) == 0x4068);
static_assert(offsetof(CLoginMode, slotExtension) == 0x6EFC);
static_assert(offsetof(CLoginMode, unknown_6F50) == 0x6F50);
static_assert(offsetof(CLoginMode, mapStates) == 0x6F68);
static_assert(sizeof(CLoginMode) == 0x6F7C);
#endif

#endif
