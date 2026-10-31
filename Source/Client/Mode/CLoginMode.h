#ifndef RAGNAROK_SOURCE_CLIENT_MODE_CLOGINMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_CLOGINMODE_H

#include "Client/Mode/CMode.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

class UIWindow;

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
struct NotifyZoneServerPacket;
struct NotifyZoneServer2Packet;
struct PasswordModePacket;
struct AcceptEnterPositionPacket;

#pragma pack(push, 1)
struct CharacterInfo {
  /* 0000 */ std::uint32_t gid;
  /* 0004 */ std::int64_t exp;
  /* 000C */ unsigned char unknown_000C[0x24];
  /* 0030 */ std::int16_t jobPoint;
  /* 0032 */ std::int64_t hp;
  /* 003A */ std::int64_t maxHp;
  /* 0042 */ std::int64_t sp;
  /* 004A */ std::int64_t maxSp;
  /* 0052 */ std::int16_t speed;
  /* 0054 */ std::int16_t job;
  /* 0056 */ std::int16_t head;
  /* 0058 */ std::int16_t unknown_0058;
  /* 005A */ std::int16_t weapon;
  /* 005C */ std::int16_t level;
  /* 005E */ std::int16_t skillPoint;
  /* 0060 */ std::int16_t accessory;
  /* 0062 */ std::int16_t shield;
  /* 0064 */ std::int16_t accessory2;
  /* 0066 */ std::int16_t accessory3;
  /* 0068 */ std::int16_t headPalette;
  /* 006A */ std::int16_t bodyStyle;
  /* 006C */ char name[24];
  /* 0084 */ unsigned char stats[6];
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
  CLoginMode();
  void SubD97780(int value);
  /* 0058 */ int charParams[6];
  /* 0070 */ int unknown_0070;
  /* 0074 */ int unknown_0074;
  /* 0078 */ bool unknown_0078;
  /* 0079 */ unsigned char unknown_0079[0x3F];
  /* 00B8 */ unsigned char billingInfo[0x80];
  /* 0138 */ unsigned char unknown_0138[0x8C];
  /* 01C4 */ int unknown_01C4;
  /* 01C8 */ unsigned char unknown_01C8[0x4];
  /* 01CC */ int characterCount;
  /* 01D0 */ unsigned char unknown_01D0[0x8];
  /* 01D8 */ int unknown_01D8;
  /* 01DC */ int unknown_01DC;
  /* 01E0 */ bool characterDeleted;
  /* 01E1 */ unsigned char unknown_01E1[0x3];
  /* 01E4 */ std::uint32_t otpStarted;
  /* 01E8 */ unsigned char unknown_01E8[0x3E80];
  /* 4068 */ CharacterInfo characters[68];
  /* 6EE4 */ std::string wallpaperName;
  /* 6EFC */ CharacterSlotExtension slotExtension;
  /* 6F10 */ std::uint32_t unknown_6F10;
  /* 6F14 */ UIWindow* window_6F14;
  /* 6F18 */ int passwordRetryCount;
  /* 6F1C */ std::vector<int> unknown_6F1C;
  /* 6F28 */ int unknown_6F28;
  /* 6F2C */ std::vector<int> unknown_6F2C;
  /* 6F38 */ std::vector<int> unknown_6F38;
  /* 6F44 */ unsigned char savedInlineCharset;
  /* 6F45 */ unsigned char unknown_6F45[0x3];
  /* 6F48 */ int unknown_6F48;
  /* 6F4C */ int unknown_6F4C;
  /* 6F50 */ std::string unknown_6F50;
  /* 6F68 */ std::vector<MapStateEntry> mapStates;
  /* 6F74 */ bool unknown_6F74;
  /* 6F75 */ bool unknown_6F75;
  /* 6F76 */ bool unknown_6F76;
  /* 6F77 */ unsigned char unknown_6F77[0x1];
  /* 6F78 */ int unknown_6F78;

  int Run() override;
  void Initialize(const char* name) override;
  void End() override;

  void SubD8D4F0(int);
  void SubD8D500(const char* packet);
  void SubD90C90(const PasswordModePacket* packet);
  void SubD911C0(const PasswordModePacket* packet);
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
  void ConnectZoneServer(std::uint32_t gid, std::string mapName, std::uint32_t ip, int port, std::string host);
  void ReceiveNotifyZoneServer(const NotifyZoneServerPacket* packet);
  void ReceiveNotifyZoneServer2(const NotifyZoneServer2Packet* packet);
  void Zc_Accept_Enter3(const AcceptEnterPositionPacket* packet);
  void Zc_Accept_Enter2(const AcceptEnterPositionPacket* packet);
  void ShowLoginError(int code, int argument);
};

extern bool g_loginModeFlag;
extern int g_loginStartMode;
extern int g_isStopByLogin;
extern int g_modalImmediate;
extern bool g_pingLogEnabled;
extern std::uint32_t g_pingLogFirstTick;
extern std::list<std::uint32_t> g_pingLogTicks;
extern char g_zoneServerIp[16];

int GetClientType();
std::string GetMacAddress();
const char* GetUserCountText(unsigned short userCount);
void OpenAccountRegistration();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CharacterInfo) == 0xAF);
static_assert(offsetof(CharacterInfo, name) == 0x6C);
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
