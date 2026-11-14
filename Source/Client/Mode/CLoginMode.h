#ifndef RAGNAROK_SOURCE_CLIENT_MODE_CLOGINMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_CLOGINMODE_H

#include "Client/Mode/CMode.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

class UIWindowMgr;
class UIWindow;
class UIWaitWnd;

struct CharacterListPacket;
struct CharacterSlotsPacket;
struct CharacterResultPacket;
struct CharacterDeletePacket;
struct AcceptEnterPacket;
struct MapStatePacket;
struct LoginErrorPacket;
struct LoginErrorDetailPacket;
struct CharacterPagePacket;
struct RefuseLoginPacket;
struct NotifyZoneServerPacket;
struct NotifyZoneServer2Packet;
struct PasswordModePacket;
struct AcceptEnterPositionPacket;
struct AcceptLoginPacket;
struct ConnectionEndpoint;

#pragma pack(push, 1)
struct CharacterInfo {
  /* 0000 */ std::uint32_t gid;
  /* 0004 */ std::int64_t exp;
  /* 000C */ std::int32_t zeny;
  /* 0010 */ std::int64_t jobExp;
  /* 0018 */ std::int32_t jobLevel;
  /* 001C */ std::int32_t bodyState;
  /* 0020 */ std::int32_t effectState;
  /* 0024 */ std::uint32_t jobFlags;
  /* 0028 */ std::int32_t karma;
  /* 002C */ std::int32_t manner;
  /* 0030 */ std::int16_t jobPoint;
  /* 0032 */ std::int64_t hp;
  /* 003A */ std::int64_t maxHp;
  /* 0042 */ std::int64_t sp;
  /* 004A */ std::int64_t maxSp;
  /* 0052 */ std::int16_t speed;
  /* 0054 */ std::int16_t job;
  /* 0056 */ std::int16_t head;
  /* 0058 */ std::int16_t bodyCostume;
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
  /* 008B */ unsigned char characterFlags[0x3];
  /* 008E */ char mapName[16];
  /* 009E */ std::int32_t deleteDate;
  /* 00A2 */ std::int32_t robe;
  /* 00A6 */ std::int32_t slotChangeCount;
  /* 00AA */ std::int32_t nameChangeCount;
  /* 00AE */ unsigned char sex;
};

struct CharacterSlotExtension {
  /* 0000 */ unsigned char padding_0000;
  /* 0001 */ std::uint32_t code;
  /* 0005 */ std::uint32_t time1;
  /* 0009 */ std::uint32_t time2;
  /* 000D */ unsigned char padding_000D[7];
};
#pragma pack(pop)

struct LoginStringRecord {
  /* 0000 */ std::string display;
  /* 0018 */ std::string description;
  /* 0030 */ std::string balloon;
  /* 0048 */ std::string address;
  /* 0060 */ std::string port;
  /* 0078 */ unsigned char typeAndVersion[8];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LoginStringRecord) == 0x80);
#endif

struct MapStateData {
  /* 0000 */ std::uint32_t values[4];
};

struct MapStateEntry {
  /* 0000 */ int index;
  /* 0004 */ MapStateData data;
};

struct ServerAddress {
  /* 0000 */ std::uint32_t ip;
  /* 0004 */ std::int16_t port;
  /* 0006 */ char name[20];
  /* 001A */ std::uint16_t userCount;
  /* 001C */ std::int16_t state;
  /* 001E */ std::int16_t property;
};

struct ServerAddress2 {
  /* 0000 */ ServerAddress address;
  /* 0020 */ char domain[128];
};

static_assert(sizeof(ServerAddress) == 0x20);
static_assert(sizeof(ServerAddress2) == 0xA0);

class CLoginMode : public CMode {
public:
  CLoginMode();
  virtual ~CLoginMode();
  void SubD97780(int value);
  void SendKeepalive();
  /* 0058 */ int charParams[6];
  /* 0070 */ int characterParameter7;
  /* 0074 */ int characterParameter8;
  /* 0078 */ bool makingCharNameFirstByte;
  /* 0079 */ unsigned char makingCharNameTail[0x3F];
  /* 00B8 */ unsigned char billingInfo[0x80];
  /* 0138 */ char m_password[0x40];
  /* 0178 */ char m_accountId[0x40];
  /* 01B8 */ char m_authCode[0xC];
  /* 01C4 */ int serverCount;
  /* 01C8 */ int selectedServer;
  /* 01CC */ int characterCount;
  /* 01D0 */ unsigned char loginExtra[0x8];
  /* 01D8 */ int receivedCharacterCount;
  /* 01DC */ int padding_01DC;
  /* 01E0 */ bool characterDeleted;
  /* 01E1 */ unsigned char padding_01E1[0x3];
  /* 01E4 */ std::uint32_t otpStarted;
  /* 01E8 */ ServerAddress2 serverList[100];
  /* 4068 */ CharacterInfo characters[68];
  /* 6EE4 */ std::string wallpaperName;
  /* 6EFC */ CharacterSlotExtension slotExtension;
  /* 6F10 */ std::uint32_t nextKeepaliveTime;
  /* 6F14 */ UIWaitWnd* waitWnd;
  /* 6F18 */ int passwordRetryCount;
  /* 6F1C */ std::vector<int> noticeCommands;
  /* 6F28 */ int authChallengeId;
  /* 6F2C */ std::vector<int> padding_6F2C;
  /* 6F38 */ std::vector<LoginStringRecord> accountInfo;
  /* 6F44 */ unsigned char savedInlineCharset;
  /* 6F45 */ unsigned char padding_6F45[0x3];
  /* 6F48 */ int selectedAccount;
  /* 6F4C */ int selectedSecondaryAccount;
  /* 6F50 */ std::string keycryptErrorInfo;
  /* 6F68 */ std::vector<MapStateEntry> mapStates;
  /* 6F74 */ bool loginResponseReceived;
  /* 6F75 */ bool otpPending;
  /* 6F76 */ bool otpTimedOut;
  /* 6F77 */ unsigned char padding_6F77[0x1];
  /* 6F78 */ int remainingLoginSeconds;

  int Run() override;
  void Initialize(const char* name) override;
  void End() override;
  void Update() override;
  void OnUpdateEx(int value) override;
  int SendMsg(int event, int argument1, int argument2, int argument3, int argument4) override;
  void OnChangeState(int value) override;

  void SubD8D4F0(int);
  void SubD8D500(const char* packet);
  void SubD90C90(const PasswordModePacket* packet);
  void SubD911C0(const PasswordModePacket* packet);
  int GetMaxCharacterSlot();
  void ReceiveCharacterSlots(const CharacterSlotsPacket* packet);
  void ReceiveAcceptEnter(const AcceptEnterPacket* packet);
  void SubD8EDC0(int);
  void ReceiveCharacterResult(const CharacterResultPacket* packet);
  void SubD8F560(int);
  void ReceiveCharacterPage(const CharacterPagePacket* packet);
  void ReceiveCharacterList(const CharacterListPacket* packet);
  void ReceiveDeleteCharacter(const CharacterDeletePacket* packet);
  void ReceiveDeleteCancel(const CharacterDeletePacket* packet);
  void ReceiveDeleteReservation(const CharacterDeletePacket* packet);
  void ReceiveMapStates(const MapStatePacket* packet);
  void SubD91780(int);
  bool ShowNotice(int notice, bool append);
  void ReceiveRefuseLogin(const RefuseLoginPacket* packet);
  void SubD97B20(int);
  void SubD97EF0(int);
  void SubD97F00(int);
  void OpenAccountRegistration();
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
  void ReceiveAcceptLogin(const AcceptLoginPacket* packet, int version);
  void PollNetworkStatus();
};

extern int g_charServerAccountIdPending;
extern int g_loginStartMode;
extern int g_isStopByLogin;
extern int g_modalImmediate;
extern bool g_pingLogEnabled;
extern std::uint32_t g_pingLogFirstTick;
extern std::list<std::uint32_t> g_pingLogTicks;
extern ConnectionEndpoint g_zoneServerEndpoint;
extern CharacterInfo g_charInfo;
extern bool g_replayMode;
extern int g_restoreSurfaces;
extern int g_isAppActive;

int GetClientType();
std::string GetMacAddress();
const char* GetUserCountText(unsigned short userCount);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CharacterInfo) == 0xAF);
static_assert(offsetof(CharacterInfo, name) == 0x6C);
static_assert(offsetof(CharacterInfo, slot) == 0x8A);
static_assert(offsetof(CharacterInfo, mapName) == 0x8E);
static_assert(offsetof(CharacterInfo, sex) == 0xAE);
static_assert(sizeof(MapStateEntry) == 0x14);
static_assert(sizeof(CharacterSlotExtension) == 0x14);
static_assert(offsetof(CLoginMode, characterCount) == 0x1CC);
static_assert(offsetof(CLoginMode, receivedCharacterCount) == 0x1D8);
static_assert(offsetof(CLoginMode, characters) == 0x4068);
static_assert(offsetof(CLoginMode, slotExtension) == 0x6EFC);
static_assert(offsetof(CLoginMode, keycryptErrorInfo) == 0x6F50);
static_assert(offsetof(CLoginMode, mapStates) == 0x6F68);
static_assert(sizeof(CLoginMode) == 0x6F7C);
#endif

extern unsigned char g_loginCancelRequested;

#endif
