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

enum CharacterRenameResult : int {
  CHARACTER_RENAME_SUCCESS = 0,
  CHARACTER_RENAME_ALREADY_CHANGED = 1,
  CHARACTER_RENAME_USER_NOT_REGISTERED = 2,
  CHARACTER_RENAME_FAILED = 3,
  CHARACTER_RENAME_DUPLICATE_NAME = 4,
  CHARACTER_RENAME_IN_GUILD = 5,
  CHARACTER_RENAME_IN_PARTY = 6,
  CHARACTER_RENAME_NAME_TOO_LONG = 7,
  CHARACTER_RENAME_PREVENTED_NAME = 8,
  CHARACTER_RENAME_FORBIDDEN_NAME = 9,
  CHARACTER_RENAME_UNKNOWN_FAILURE = 10,
};

enum CharacterDeleteResult : int {
  CHARACTER_DELETE_UNKNOWN_FAILURE = 0,
  CHARACTER_DELETE_SUCCESS = 1,
  CHARACTER_DELETE_CONFIGURATION_FAILURE = 2,
  CHARACTER_DELETE_DATABASE_FAILURE = 3,
  CHARACTER_DELETE_DATE_FAILURE = 4,
  CHARACTER_DELETE_BIRTHDATE_FAILURE = 5,
  CHARACTER_DELETE_NAME_FAILURE = 6,
  CHARACTER_DELETE_EMAIL_FAILURE = 7,
  CHARACTER_DELETE_PERSONAL_NUMBER_FAILURE = 8,
};

enum CharacterDeleteCancelResult : int {
  CHARACTER_DELETE_CANCEL_UNKNOWN_FAILURE = 0,
  CHARACTER_DELETE_CANCEL_SUCCESS = 1,
  CHARACTER_DELETE_CANCEL_DATABASE_FAILURE = 2,
};

enum CharacterDeleteReservationResult : int {
  CHARACTER_DELETE_RESERVE_UNKNOWN_FAILURE = 0,
  CHARACTER_DELETE_RESERVE_SUCCESS = 1,
  CHARACTER_DELETE_RESERVE_PENDING = 2,
  CHARACTER_DELETE_RESERVE_DATABASE_FAILURE = 3,
  CHARACTER_DELETE_RESERVE_GUILD_FAILURE = 4,
  CHARACTER_DELETE_RESERVE_PARTY_FAILURE = 5,
  CHARACTER_DELETE_RESERVE_ASSISTANT_FAILURE = 6,
  CHARACTER_DELETE_RESERVE_ZENYLOTTO_FAILURE = 7,
};

enum CharacterCreateFailure : unsigned char {
  CHARACTER_CREATE_NAME_EXISTS = 0,
  CHARACTER_CREATE_AGE_LIMIT = 1,
  CHARACTER_CREATE_DELETE_LIMIT = 2,
  CHARACTER_CREATE_INVALID_SLOT = 3,
  CHARACTER_CREATE_PREMIUM_REQUIRED = 11,
  CHARACTER_CREATE_INVALID_NAME = 12,
};

enum CharacterDeleteFailure : unsigned char {
  CHARACTER_DELETE_REFUSE_INVALID_IDENTITY = 0,
  CHARACTER_DELETE_REFUSE_INVALID_SLOT = 1,
  CHARACTER_DELETE_REFUSE_FAILED = 2,
};

enum CharacterSelectFailure : unsigned char {
  CHARACTER_SELECT_INVALID_SLOT = 0,
  CHARACTER_SELECT_INVALID_BASE_LEVEL = 1,
  CHARACTER_SELECT_INVALID_JOB_LEVEL = 2,
  CHARACTER_SELECT_INVALID_JOB = 3,
  CHARACTER_SELECT_INVALID_MAP = 4,
  CHARACTER_SELECT_INVALID_ZENY = 5,
};

enum SecondPasswordMakeResult : unsigned short {
  SECOND_PASSWORD_MAKE_SUCCESS = 0,
  SECOND_PASSWORD_MAKE_DUPLICATE = 1,
  SECOND_PASSWORD_MAKE_RESTRICTED = 2,
  SECOND_PASSWORD_MAKE_PERSONAL_NUMBER = 3,
  SECOND_PASSWORD_MAKE_FAILED = 4,
};

enum SecondPasswordEditResult : unsigned short {
  SECOND_PASSWORD_EDIT_SUCCESS = 0,
  SECOND_PASSWORD_EDIT_FAILED = 1,
  SECOND_PASSWORD_EDIT_RESTRICTED = 2,
  SECOND_PASSWORD_EDIT_PERSONAL_NUMBER = 3,
};

enum SecondPasswordToggleResult : unsigned short {
  SECOND_PASSWORD_TOGGLE_SUCCESS = 0,
  SECOND_PASSWORD_TOGGLE_FAILED = 1,
};

enum ServerLoad : unsigned short {
  SERVER_LOAD_NICE = 0,
  SERVER_LOAD_NORMAL = 1,
  SERVER_LOAD_BUSY = 2,
  SERVER_LOAD_VERY_BUSY = 3,
};

enum ServerAccessType : short {
  SERVER_ACCESS_ADULT = 2,
  SERVER_ACCESS_PAY = 3,
  SERVER_ACCESS_FREE = 4,
};

enum TarenLoginFailure : unsigned char {
  TAREN_LOGIN_ACCOUNT_NOT_FOUND = 1,
  TAREN_LOGIN_INVALID_PASSWORD = 2,
  TAREN_LOGIN_INVALID_IP = 3,
  TAREN_LOGIN_INVALID_PERSONAL_NUMBER = 4,
  TAREN_LOGIN_ACCOUNT_BLOCKED = 5,
  TAREN_LOGIN_SYSTEM_FAILURE = 6,
  TAREN_LOGIN_NOT_ACTIVATED = 7,
  TAREN_LOGIN_TEENAGER_BLOCKED = 9,
};

enum LoginErrorCode : int {
  LOGIN_ERROR_CONNECT_ACCOUNT_SERVER = 0,
  LOGIN_ERROR_OTP_REQUIRED = 400,
  LOGIN_ERROR_OTP_FAILED = 500,
  LOGIN_ERROR_INCORRECT_PASSWORD = 600,
  LOGIN_ERROR_SERVICE_CHECKING = 700,
  LOGIN_ERROR_RETRY_CONNECT = 800,
  LOGIN_ERROR_FORCED_WEB_LOGIN = 900,
  LOGIN_ERROR_CANNOT_LOGIN = 1200,
};

enum RefuseEnterResult : unsigned char {
  REFUSE_ENTER_ACCESS_DENIED = 0,
  REFUSE_ENTER_ID_MISMATCH = 1,
  REFUSE_ENTER_OVER_CONNECT_USER = 111,
  REFUSE_ENTER_RESTRICTED_MAKE_CHAR = 118,
  REFUSE_ENTER_LIMIT_USER = 119,
};

enum LoginNotice : unsigned int {
  LOGIN_NOTICE_RESERVE_CHANGE_CHARACTER_SLOT = 45,
  LOGIN_NOTICE_CHARACTER_SLOT_CHANGE = 46,
  LOGIN_NOTICE_CHANGE_CHARACTER_NAME = 47,
};

enum LoginState : int {
  LOGIN_STATE_WARNING = 0,
  LOGIN_STATE_LICENSE = 1,
  LOGIN_STATE_SELECT_ACCOUNT = 2,
  LOGIN_STATE_LOGIN = 3,
  LOGIN_STATE_CONNECT_ACCOUNT_SERVER = 4,
  LOGIN_STATE_CONNECT_CHARACTER_SERVER = 5,
  LOGIN_STATE_SELECT_SERVER = 6,
  LOGIN_STATE_SELECT_CHARACTER = 7,
  LOGIN_STATE_CREATE_CHARACTER = 8,
  LOGIN_STATE_REQUEST_SELECT_CHARACTER = 9,
  LOGIN_STATE_REQUEST_CREATE_CHARACTER = 10,
  LOGIN_STATE_REQUEST_DELETE_CHARACTER = 11,
  LOGIN_STATE_CONNECT_ZONE_SERVER = 12,
  LOGIN_STATE_RESET_LOGIN = 13,
  LOGIN_STATE_SELECT_UNION_CHARACTER = 19,
  LOGIN_STATE_MOBILE_OTP = 24,
  LOGIN_STATE_OPEN_REPLAY = 25,
  LOGIN_STATE_REPLAY_SERVER_LIST = 26,
  LOGIN_STATE_SELECT_REPLAY = 27,
  LOGIN_STATE_PLAY_REPLAY = 28,
  LOGIN_STATE_REPLAY_WARNING = 29,
  LOGIN_STATE_SELECT_MAP = 30,
  LOGIN_STATE_REQUEST_SELECT_MAP = 31,
  LOGIN_STATE_CONNECT_AGENCY = 38,
};

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
  /* 008B */ unsigned char characterFlags;
  /* 008C */ unsigned char padding_008C[0x2];
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
  /* 00B8 */ char billingInfo[0x80];
  /* 0138 */ char m_password[0x40];
  /* 0178 */ char m_accountId[0x40];
  /* 01B8 */ char m_authCode[0xC];
  /* 01C4 */ int serverCount;
  /* 01C8 */ int selectedServer;
  /* 01CC */ int characterCount;
  /* 01D0 */ char loginExtra[0x8];
  /* 01D8 */ int receivedCharacterCount;
  /* 01DC */ int receivedCharacterPages;
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
  /* 6F1C */ std::vector<unsigned int> noticeCommands;
  /* 6F28 */ int authChallengeId;
  /* 6F2C */ std::vector<LoginStringRecord> padding_6F2C;
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
static_assert(offsetof(CharacterInfo, characterFlags) == 0x8B);
static_assert(offsetof(CharacterInfo, mapName) == 0x8E);
static_assert(offsetof(CharacterInfo, sex) == 0xAE);
static_assert(sizeof(MapStateEntry) == 0x14);
static_assert(sizeof(CharacterSlotExtension) == 0x14);
static_assert(offsetof(CLoginMode, characterCount) == 0x1CC);
static_assert(offsetof(CLoginMode, billingInfo) == 0xB8);
static_assert(offsetof(CLoginMode, loginExtra) == 0x1D0);
static_assert(offsetof(CLoginMode, receivedCharacterCount) == 0x1D8);
static_assert(offsetof(CLoginMode, receivedCharacterPages) == 0x1DC);
static_assert(offsetof(CLoginMode, characters) == 0x4068);
static_assert(offsetof(CLoginMode, slotExtension) == 0x6EFC);
static_assert(offsetof(CLoginMode, keycryptErrorInfo) == 0x6F50);
static_assert(offsetof(CLoginMode, mapStates) == 0x6F68);
static_assert(sizeof(CLoginMode) == 0x6F7C);
#endif

#endif
