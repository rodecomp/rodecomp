#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/ModeResultEnums.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Application/ApplicationMessages.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/ClientInfo.h"
#include "Client/Application/Launcher.h"
#include "Archive/PakRegistry.h"
#include "Client/Input/Mouse.h"
#include "Client/Stove/StoveMgr.h"
#include "Client/Mode/ModeFadeOut.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Client/Replay/Replay.h"
#include "Client/Application/Timer.h"
#include "Client/Input/InputCharset.h"
#include "Client/Stove/StovePCSDK.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "Contents/Events/CheckAttendanceMgr.h"
#include "Contents/Tables/TipOfTheDay.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/PartialShutDownSchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Xml/XmlNode.h"
#include "Resource/ResourcePreload.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Security/NProtectGameGuardMgr.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Shop/UIBartermarketWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Login/UIPasswordWnd.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Login/UISelCharForUServerWnd.h"
#include "Utility/Async/NAsyncWorkMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Hash/GGHashMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextTokens.h"
#include "Client/Mode/CMode.h"
#include "Client/Application/Registry.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Rodex/RodexMailStore.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Network/Packets/ExecutableDigestPacket.h"
#include "Security/ReassemblyPacketMgr.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Replay/UIReplayListBox.h"
#include "UI/Replay/UISelectReplayDataWnd.h"
#include "UI/Login/UISelectServerWnd.h"
#include "Utility/Cipher/AES.h"
#include "Utility/Cipher/Rijndael.h"
#include "Utility/Cipher/CryptName.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <iphlpapi.h>
#include <new>
#include <random>
#include <timeapi.h>

void TraceSelection(int selection);
void TraceSelectionRange(int first, int last);

#pragma pack(push, 1)
struct CharacterSlotsPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ unsigned char normalSlots;
  /* 0005 */ unsigned char premiumSlots;
  /* 0006 */ unsigned char billingSlots;
  /* 0007 */ unsigned char producibleSlots;
  /* 0008 */ unsigned char validSlots;
  /* 0009 */ CharacterSlotExtension extension;
};

struct CharacterResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t result;
};

struct CharacterDeletePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t gid;
  /* 0006 */ int result;
  /* 000A */ int remaining;
};

struct AcceptEnterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ CharacterSlotExtension extension;
  /* 0018 */ unsigned char padding_0018[3];
  /* 001B */ CharacterInfo characters[1];
};

struct MapStateInfo {
  /* 0000 */ int state;
  /* 0004 */ MapStateData data;
};

struct MapStatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ MapStateInfo states[1];
};

struct CharacterListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ CharacterInfo characters[1];
};

struct LoginErrorPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ int error;
  /* 0008 */ char text[1];
};

struct LoginErrorDetailPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ int error;
  /* 0008 */ char detail[20];
  /* 001C */ char text[1];
};

struct CharacterPagePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ CharacterInfo characters[1];
};

struct RefuseLoginPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
  /* 0003 */ char blockDate[20];
};

struct AcceptLoginPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ int authCode;
  /* 0008 */ std::uint32_t aid;
  /* 000C */ int userLevel;
  /* 0010 */ std::uint32_t lastLoginIp;
  /* 0014 */ char lastLoginTime[26];
  /* 002E */ unsigned char sex;
  /* 002F */ unsigned char body[1];
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

struct ResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
};

struct ShortResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t result;
};

struct IntValuePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int value;
};

struct LoginErrorCodePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int code;
  /* 0006 */ char text[20];
};

struct LoginErrorTimePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char code;
  /* 0003 */ int remaining;
};

struct SecondPasswordResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t result;
  /* 0004 */ std::uint32_t accountId;
};

struct SecondPasswordStatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ std::int16_t result;
};

struct ShutdownNoticePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ __time32_t endTime;
  /* 0006 */ __time32_t startTime;
};

struct LoginMessagePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ char text[1];
};

struct BlockedCharacter {
  /* 0000 */ std::uint32_t gid;
  /* 0004 */ char expireDate[20];
};

struct BlockedCharacterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ BlockedCharacter characters[1];
};

struct CharacterInfoPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ CharacterInfo info;
};

struct AcceptEnterNeoUnionPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ CharacterSlotExtension extension;
  /* 0018 */ CharacterInfo characters[1];
};

struct CharacterSlotCountsPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ unsigned char counts[6];
  /* 000A */ CharacterSlotExtension extension;
};

struct GameRoomReplyPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char permission;
};

struct ChangeCharacterNamePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t gid;
};
#pragma pack(pop)

struct CharServerEndpoint {
  /* 0000 */ char ip[16];
  /* 0010 */ int port;
  /* 0014 */ char host[129];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CharServerEndpoint) == 0x98);
#endif

// GLOBAL: gl_25386
CharServerEndpoint g_charServerEndpoint;

// GLOBAL: gl_25387
std::uint32_t g_charServerIp;

// GLOBAL: gl_25054
int g_charServerAccountIdPending;
// GLOBAL: gl_25147
int g_isStopByLogin;
// GLOBAL: gl_25146
int g_loginStartMode = 3;
// GLOBAL: gl_25148
const char* g_loginWallpaperName = "";

struct SessionAccount {
  /* 0000 */ int sex;
  /* 0004 */ int padding_0004;
  /* 0008 */ int authCode;
  /* 000C */ int userLevel;
  /* 0010 */ std::uint32_t lastLoginIp;
  /* 0014 */ char lastLoginTime[27];
};

// GLOBAL: gl_25169
std::uint32_t g_loginAcceptTime;
// GLOBAL: gl_25252
unsigned char g_loginExitFlag;
// GLOBAL: gl_25221
CharacterInfo g_charInfo;
// GLOBAL: gl_25255
bool g_replayMode;
// GLOBAL: gl_25383
int g_restoreSurfaces;
// GLOBAL: gl_25384
int g_isAppActive;

static_assert(sizeof(CharacterSlotsPacket) == 0x1D);
static_assert(offsetof(AcceptLoginPacket, body) == 0x2F);
static_assert(sizeof(ServerAddress) == 0x20);
static_assert(sizeof(ServerAddress2) == 0xA0);
static_assert(sizeof(AcceptEnterNeoUnionPacket) == 0x18 + sizeof(CharacterInfo));
static_assert(offsetof(CharacterSlotCountsPacket, extension) == 0xA);
static_assert(sizeof(BlockedCharacter) == 0x18);
static_assert(offsetof(AcceptEnterPacket, characters) == 0x1B);

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const std::array<char, Length>& source) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

struct CharacterInfo2SizeErrorFormat {
  __declspec(noinline) EncryptedName<75, 0xED15938D333D2DB9ULL>& operator()() const;
};

// OFFSET: 0x00D8CA50
// FUNCTION: fn_22261
EncryptedName<75, 0xED15938D333D2DB9ULL>& CharacterInfo2SizeErrorFormat::operator()() const {
  constexpr std::array<char, 75> source = CRYPT_NAME("CHARACTER_INFO2 size error!! character num : %d  CHARACTER_INFO2 size : %d", 0xED15938D333D2DB9ULL);
  thread_local EncryptedName<sizeof(source), 0xED15938D333D2DB9ULL> name(source);
  return name;
}

struct PasswordRetryExceededMessage {
  __declspec(noinline) EncryptedName<103, 0xBF2B4B31397FF5A7ULL>& operator()() const;
};

// OFFSET: 0x00D8CB50
// FUNCTION: fn_22262
EncryptedName<103, 0xBF2B4B31397FF5A7ULL>& PasswordRetryExceededMessage::operator()() const {
  constexpr std::array<char, 103> source = CRYPT_NAME("비밀번호를 3회 잘못입력하였습니다. 비밀번호를 확인해주세요.              클라이언트 실행을 종료합니다.", 0xBF2B4B31397FF5A7ULL);
  thread_local EncryptedName<sizeof(source), 0xBF2B4B31397FF5A7ULL> name(source);
  return name;
}

// OFFSET: 0x00D8D4F0
// FUNCTION: fn_22265
void CLoginMode::SubD8D4F0(int) {
}

// OFFSET: 0x00D8D500
// FUNCTION: fn_22266
void CLoginMode::SubD8D500(const char* packet) {
  UIWindow* window = g_windowMgr.newSelectCharWnd;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 0:
    g_windowMgr.Confirm(LookupMessage(MSI_CHANGE_NAME_SUCCESS), 0, true, false, 0, "Message", 280, 120, 0);
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME_SUCCESS, 0, 0, 0);
    }
    return;
  case 1:
    if (g_windowMgr.Confirm(LookupMessage(MSI_ALREADY_CHANGED_NAME), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8 && g_windowMgr.changeNameWnd) {
      g_windowMgr.CloseWindow(WID_CHANGENAMEWND);
    }
    return;
  case 2:
    if (g_windowMgr.Confirm(LookupMessage(MSI_NOTREGISTED_USER), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
      return;
    }
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    return;
  case 3:
    if (g_windowMgr.Confirm(LookupMessage(MSI_CHANGE_NAME_FAILURE), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8 && g_windowMgr.changeNameWnd) {
      g_windowMgr.CloseWindow(WID_CHANGENAMEWND);
    }
    return;
  case 4:
    if (g_windowMgr.Confirm(LookupMessage(MSI_DUPLICATED_CHAR_NAME), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
      return;
    }
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME, 0, 0, 0);
    }
    return;
  case 5:
    if (g_windowMgr.Confirm(LookupMessage(MSI_FAILED_RENAME_BELONGS_TO_GUILD), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8 && g_windowMgr.changeNameWnd) {
      g_windowMgr.CloseWindow(WID_CHANGENAMEWND);
    }
    return;
  case 6:
    if (g_windowMgr.Confirm(LookupMessage(MSI_FAILED_RENAME_BELONGS_TO_PARTY), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8 && g_windowMgr.changeNameWnd) {
      g_windowMgr.CloseWindow(WID_CHANGENAMEWND);
    }
    return;
  case 7:
    if (g_windowMgr.Confirm(LookupMessage(MSI_FAILED_RENAME_OVER_LENGTH), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
      return;
    }
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME, 0, 0, 0);
    }
    return;
  case 8:
    if (g_windowMgr.Confirm(LookupMessage(MSI_FAILED_RENAME_PREVENTNAM), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
      return;
    }
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME, 0, 0, 0);
    }
    return;
  case 9:
    if (g_windowMgr.Confirm(LookupMessage(MSI_FAILED_RENAME_FORBIDDEN), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
      return;
    }
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME, 0, 0, 0);
    }
    return;
  case 10:
    if (g_windowMgr.Confirm(LookupMessage(MSI_FAILED_RENAME_UNKOWN), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8 && g_windowMgr.changeNameWnd) {
      g_windowMgr.CloseWindow(WID_CHANGENAMEWND);
    }
    return;
  default:
    return;
  }
}

// OFFSET: 0x00D8E1D0
// FUNCTION: fn_22270
int GetClientType() {
  switch (g_serviceMode) {
  case 1:
    switch (g_serviceType) {
    case 1:
      return 14;
    case ServiceJapan:
      return 9;
    case ServiceChina:
      return 4;
    case ServiceTaiwan:
      return 5;
    case ServiceThai:
      return 7;
    case 6:
      return 13;
    case 7:
      return 18;
    case 8:
      return 16;
    case 9:
      return 17;
    case 10:
      return 20;
    case 11:
      return 21;
    case 12:
      return 22;
    case 13:
      return 23;
    case ServiceVietnam:
      return 33;
    case 19:
      return 35;
    case ServiceRussia:
      return 25;
    case 20:
      return 36;
    default:
      return 2;
    }

  case 5:
    switch (g_serviceType) {
    case 7:
      return 27;
    case ServiceJapan:
      return 28;
    case ServiceThai:
      return 29;
    case ServiceVietnam:
      return 32;
    case 6:
      return 34;
    default:
      return 0;
    }

  default:
    switch (g_serviceType) {
    case 1:
      return 1;
    case ServiceJapan:
      return 3;
    case ServiceChina:
      return 4;
    case ServiceTaiwan:
      return 5;
    case ServiceThai:
      return 7;
    case 6:
      return 12;
    case 7:
      return 15;
    case 8:
      return 16;
    case 9:
      return 17;
    case 10:
      return 20;
    case 11:
      return 21;
    case 12:
      return 22;
    case 13:
      return 23;
    case ServiceRussia:
      return 25;
    case ServiceVietnam:
      return 26;
    case ServiceChile:
      return 30;
    case 18:
      return 31;
    case 19:
      return 35;
    case 20:
      return 36;
    default:
      return 0;
    }
  }
}

// OFFSET: 0x00D8E3A0
// FUNCTION: fn_22271
std::string GetMacAddress() {
  ULONG size = 0;
  if (GetAdaptersInfo(nullptr, &size) == ERROR_BUFFER_OVERFLOW) {
    IP_ADAPTER_INFO* adapter = reinterpret_cast<IP_ADAPTER_INFO*>(new (std::nothrow) char[size]);
    memset(adapter, 0, size);
    GetAdaptersInfo(adapter, &size);
    if (adapter != nullptr) {
      char text[100];
      memset(text, 0, sizeof(text));
      constexpr std::array<char, 30> source = CRYPT_NAME("%02X-%02X-%02X-%02X-%02X-%02X", 0x0F41EBBDA32D1B17ULL);
      _snprintf(text, sizeof(text), DecryptName<0x0F41EBBDA32D1B17ULL>(source), adapter->Address[0], adapter->Address[1], adapter->Address[2],
        adapter->Address[3], adapter->Address[4], adapter->Address[5]);
      delete[] reinterpret_cast<char*>(adapter);
      return text;
    }
  }

  constexpr std::array<char, 18> source = CRYPT_NAME("11-22-33-44-55-66", 0xFBD599E117C547C1ULL);
  return DecryptName<0xFBD599E117C547C1ULL>(source);
}

// OFFSET: 0x00D8E700
// FUNCTION: fn_22272
int CLoginMode::GetMaxCharacterSlot() {
  if (this == nullptr) {
    return 0;
  }

  int maxSlot = -1;
  for (long long i = 0; i < 68; ++i) {
    if (this->characters[i].gid == 0) {
      break;
    }

    const int slot = this->characters[i].slot;
    maxSlot = maxSlot < slot ? slot : maxSlot;
  }

  return maxSlot;
}

// OFFSET: 0x00D8E770
// FUNCTION: fn_22273
const char* GetUserCountText(unsigned short userCount) {
  if (userCount <= 1000) {
    return LookupMessage(MSI_LITTLEUSER);
  }

  if (userCount <= 3000) {
    return LookupMessage(MSI_ALITTLEUSER);
  }

  if (userCount <= 6000) {
    return LookupMessage(MSI_NORMLEUSER);
  }

  if (userCount <= 10000) {
    return LookupMessage(MSI_MANYUSER);
  }

  return LookupMessage(MSI_VERYMANYUSER);
}

// OFFSET: 0x00D8E990
// FUNCTION: fn_22274
void CLoginMode::ReceiveCharacterSlots(const CharacterSlotsPacket* packet) {
  g_session.m_normalSlotCount = packet->normalSlots;
  g_session.m_premiumSlotCount = packet->premiumSlots;
  g_session.m_billingSlotCount = packet->billingSlots;
  g_session.m_producibleSlotCount = packet->validSlots;
  g_session.m_validSlotCount = packet->producibleSlots;

  SecureZeroMemory(this->characters, sizeof(this->characters));
  g_session.characterSlotState = 0;
  this->receivedCharacterCount = 0;
  this->padding_01DC = 0;

  this->slotExtension = packet->extension;
  this->slotExtension.code = htonl(this->slotExtension.code);
  this->slotExtension.time1 = htonl(this->slotExtension.time1);
  this->slotExtension.time2 = htonl(this->slotExtension.time2);
}

// OFFSET: 0x00D8EAC0
// FUNCTION: fn_22275
void CLoginMode::ReceiveAcceptEnter(const AcceptEnterPacket* packet) {
  this->slotExtension = packet->extension;
  this->slotExtension.code = htonl(this->slotExtension.code);
  this->slotExtension.time1 = htonl(this->slotExtension.time1);
  this->slotExtension.time2 = htonl(this->slotExtension.time2);

  this->characterCount = (packet->length - 27) / sizeof(CharacterInfo);
  const int remainder = (packet->length - 27) % sizeof(CharacterInfo);
  memcpy_s(this->characters, sizeof(this->characters), packet->characters, this->characterCount * sizeof(CharacterInfo));
  if (remainder > 0) {
    char text[256];
    memset(text, 0, sizeof(text));
    const int count = this->characterCount;
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), CharacterInfo2SizeErrorFormat{}().Text(), count, sizeof(CharacterInfo));
    g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
  }

  g_session.characterSlotState = this->characterCount;
  for (int i = 0; i < this->characterCount; ++i) {
    char* mapName = this->characters[i].mapName;
    strncpy(mapName, g_session.GatName2RswName(mapName), strlen(mapName));
  }
}

// OFFSET: 0x00D8EDC0
// FUNCTION: fn_22276
void CLoginMode::SubD8EDC0(int) {
}

// OFFSET: 0x00D8EDD0
// FUNCTION: fn_22277
void CLoginMode::ReceiveCharacterResult(const CharacterResultPacket* packet) {
  UIWindow* window = g_windowMgr.newSelectCharWnd;
  switch (packet->result) {
  case 0:
    g_windowMgr.Confirm(LookupMessage(MSI_CHANGE_NAME_SUCCESS), 0, true, false, 0, "Message", 280, 120, 0);
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME_SUCCESS, 0, 0, 0);
    }
    break;
  case 1:
    if (g_windowMgr.Confirm(LookupMessage(1218), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    break;
  case 2:
    if (g_windowMgr.Confirm(LookupMessage(1219), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    break;
  case 3:
    if (g_windowMgr.Confirm(LookupMessage(MSI_CHANGE_NAME_FAILURE), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME, 0, 0, 0);
    }
    break;
  case 4:
    if (g_windowMgr.Confirm(LookupMessage(MSI_DUPLICATED_CHAR_NAME), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME, 0, 0, 0);
    }
    break;
  case 5:
    if (g_windowMgr.Confirm(LookupMessage(2022), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    break;
  case 6:
    if (g_windowMgr.Confirm(LookupMessage(2023), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    break;
  case 10:
    if (g_windowMgr.Confirm(LookupMessage(2024), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    break;
  }
}

// OFFSET: 0x00D8F040
// FUNCTION: fn_22278
void CLoginMode::ReceiveCharacterPage(const CharacterPagePacket* packet) {
  const unsigned int count = (packet->length - 4) / sizeof(CharacterInfo);
  if ((packet->length - 4) % sizeof(CharacterInfo) != 0) {
    char text[256];
    memset(text, 0, sizeof(text));
    constexpr std::array<char, 55> source = CRYPT_NAME("CHARACTER_INFO2 size error!! CHARACTER_INFO2 size : %d", 0x0399459DAF454F0FULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x0399459DAF454F0FULL>(source), sizeof(CharacterInfo));
    g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
    return;
  }

  const unsigned int total = this->receivedCharacterCount + count;
  if (total > 68) {
    char text[256];
    memset(text, 0, sizeof(text));
    constexpr std::array<char, 33> source = CRYPT_NAME("CHARACTER_INFO2 Cnt Over %d / %d", 0xD5A78DB38D8DAFABULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0xD5A78DB38D8DAFABULL>(source), total, 68);
    g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
    return;
  }

  memcpy_s(&this->characters[this->receivedCharacterCount], sizeof(this->characters) - this->receivedCharacterCount * sizeof(CharacterInfo),
    packet->characters, count * sizeof(CharacterInfo));
  __time32_t now;
  _time32(&now);
  for (int i = 0; i < static_cast<int>(count); ++i) {
    if (this->characters[this->receivedCharacterCount + i].sex == 99) {
      this->characters[this->receivedCharacterCount + i].sex = static_cast<unsigned char>(g_session.m_sex);
    }
    if (this->characters[this->receivedCharacterCount + i].deleteDate != 0) {
      this->characters[this->receivedCharacterCount + i].deleteDate += now;
    }
    CharacterInfo& character = this->characters[this->receivedCharacterCount + i];
    this->characters[this->receivedCharacterCount + i].bodyStyle =
      static_cast<std::int16_t>(g_session.GetBodyStyle(character.job, character.bodyStyle, character.bodyCostume));
  }

  this->receivedCharacterCount += count;
  if (count != 3) {
    this->characterCount = this->receivedCharacterCount;
    g_session.characterSlotState = this->receivedCharacterCount;
    this->receivedCharacterCount = 0;
    this->characterDeleted = false;
    for (long long i = 0; i < this->characterCount; ++i) {
      char* mapName = this->characters[i].mapName;
      StringFormat(mapName, sizeof(this->characters[i].mapName), static_cast<std::size_t>(-1), "%s", g_session.GatName2RswName(mapName));
    }
    this->pendingCursorType = 7;
  }
}

// OFFSET: 0x00D8F560
// FUNCTION: fn_22279
void CLoginMode::SubD8F560(int) {
}

// OFFSET: 0x00D8F8E0
// FUNCTION: fn_22280
__declspec(noinline) void CLoginMode::ReceiveCharacterList(const CharacterListPacket* packet) {
  this->characterCount = (packet->length - 4) / sizeof(CharacterInfo);
  g_session.characterSlotState = this->characterCount;
  memcpy_s(this->characters, sizeof(this->characters), packet->characters, this->characterCount * sizeof(CharacterInfo));
  this->pendingCursorType = 7;
}

// OFFSET: 0x00D8FA00
// FUNCTION: fn_22281
void CLoginMode::ReceiveDeleteCharacter(const CharacterDeletePacket* packet) {
  std::string message;
  switch (packet->result) {
  case 0:
    message = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
    break;
  case 1:
    this->characterDeleted = true;
    break;
  case 2:
    message = LookupMessage(MSI_RESULT_FAIL_CONFIGURATION);
    break;
  case 3:
    message = LookupMessage(MSI_RESULT_FAIL_DATABASE);
    break;
  case 4:
    message = LookupMessage(MSI_RESULT_FAIL_DATE);
    break;
  case 5:
    message = LookupMessage(MSI_RESULT_FAIL_BIRTH);
    break;
  case 6:
    message = LookupMessage(MSI_SPRITE_ALTER);
    break;
  case 7:
    message = LookupMessage(MSI_CANNOT_DELETE_CHARACTER_EMAIL);
    break;
  case 8:
    message = LookupMessage(MSI_ZENY_LOTTO_BUY_1);
    break;
  default:
    message = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
    break;
  }

  if (!message.empty()) {
    g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
  }
  if (this->characterDeleted != true) {
    this->pendingCursorType = 7;
  }
}

// OFFSET: 0x00D8FC80
// FUNCTION: fn_22282
void CLoginMode::ReceiveDeleteCancel(const CharacterDeletePacket* packet) {
  std::string message;
  switch (packet->result) {
  case 0:
    message = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
    break;
  case 1:
    for (int i = 0; i < this->characterCount; ++i) {
      if (this->characters[i].gid == packet->gid) {
        this->characters[i].deleteDate = 0;
        break;
      }
    }
    this->pendingCursorType = 7;
    break;
  case 2:
    message = LookupMessage(MSI_RESULT_FAIL_DATABASE);
    break;
  default:
    message = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
    break;
  }

  if (!message.empty()) {
    g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

// OFFSET: 0x00D8FE40
// FUNCTION: fn_22283
void CLoginMode::ReceiveDeleteReservation(const CharacterDeletePacket* packet) {
  std::string message;
  switch (packet->result) {
  case 0:
    message = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
    break;
  case 1:
    for (int i = 0; i < this->characterCount; ++i) {
      if (this->characters[i].gid == packet->gid) {
        __time32_t now;
        _time32(&now);
        this->characters[i].deleteDate = packet->remaining + now;
        break;
      }
    }
    this->pendingCursorType = 7;
    break;
  case 2:
    break;
  case 3:
    message = LookupMessage(MSI_RESULT_FAIL_DATABASE);
    break;
  case 4:
    message = LookupMessage(MSI_RESULT_FAIL_GUILD);
    break;
  case 5:
    message = LookupMessage(MSI_RESULT_FAIL_PARTY);
    break;
  case 6:
    message = LookupMessage(MSI_RESULT_FAILED_ASSISTANT_POS_CHARACTER);
    break;
  case 7:
    message = LookupMessage(MSI_ZENY_LOTTO_RODEX_WINNER_TITLE);
    break;
  default:
    message = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
    break;
  }

  if (!message.empty()) {
    g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

// OFFSET: 0x00D904B0
// FUNCTION: fn_22284
void CLoginMode::ReceiveMapStates(const MapStatePacket* packet) {
  const unsigned int count = (packet->length - 4) / sizeof(MapStateInfo);
  if ((packet->length - 4) % sizeof(MapStateInfo) != 0) {
    constexpr std::array<char, 34> source = CRYPT_NAME("mismatch MapState size(size = %d)", 0x792FFD1325418FB5ULL);
    char text[256];
    sprintf(text, DecryptName<0x792FFD1325418FB5ULL>(source), sizeof(MapStateInfo));
    g_windowMgr.Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
    this->pendingCursorType = 7;
    return;
  }

  this->mapStates.clear();
  MapStateEntry entry;
  entry.index = -1;
  for (int i = 0; i < static_cast<int>(count); ++i) {
    if (packet->states[i].state == 0) {
      entry.index = i;
      entry.data = packet->states[i].data;
      this->mapStates.push_back(entry);
    }
  }

  if (this->mapStates.size() == 0) {
    g_windowMgr.Confirm(LookupMessage(MSI_NOT_EXIST_ENTRYMAP), 0, true, false, 0, "Message", 280, 120, 0);
    this->pendingCursorType = 7;
    return;
  }

  this->pendingCursorType = 30;
}

// OFFSET: 0x00D91780
// FUNCTION: fn_22288
void CLoginMode::SubD91780(int) {
}

// OFFSET: 0x00D97180
// FUNCTION: fn_22298
void CLoginMode::End() {
  g_soundMgr->EndSamples();
  ResourceMgr::Instance().ClearUnused();
  g_renderer->atlas.Clear();
  this->keycryptErrorInfo.clear();
}

// OFFSET: 0x00D975C0
// FUNCTION: fn_22301
void CLoginMode::OnUpdateEx(int value) {
  if (value == 0) {
    if (this->running == 0 || g_modalImmediate != 0) {
      return;
    }
  } else {
    GetRagConnection().UpdateStatus();
  }

  PumpApplicationMessages();
  this->PollNetworkStatus();
  this->SendKeepalive();
  g_mouse.Update();
  g_windowMgr.ProcessMouse();
  if (this->running != 0) {
    this->SubD97780(this->cursorType);
    this->ExpireTooltip();
  }

  if (g_restoreSurfaces != 0) {
    g_restoreSurfaces = 0;
    GetRenderSystemHolder()->TestDevice();
    g_windowMgr.InvalidateAll();
    CBitmapRes* bitmap = reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str())));
    g_windowMgr.SetBackgroundSurface(bitmap);
  }

  if (g_isAppActive == 0 && GetRenderSystemHolder()->IsWindowed()) {
    return;
  }

  if (ShouldRenderFrame()) {
    g_renderer->ClearDepth(1);
    g_windowMgr.UpdateFrameWindows();
    if (g_serviceType == 0 || g_serviceType == 4 || g_serviceType == 8 || g_serviceType == 9 || g_serviceType == 3 || g_serviceType == 10 ||
        g_serviceType == 1 || g_serviceType == 11 || g_serviceType == 12 || g_serviceType == 5 || g_serviceType == 7 || g_serviceType == 14 ||
        g_serviceType == 20) {
      g_windowMgr.DrawLoginOverlay();
      if (this->cursorType == 0) {
        g_windowMgr.DrawCenteredBackground();
      }
    } else {
      g_windowMgr.DrawCenteredBackground();
    }
    g_windowMgr.DrawWindows();
    this->DrawCursor();
    this->DrawFadeIn();
    if (g_renderer->Draw()) {
      g_renderer->Present(1);
    }
  }

  this->TakeScreenshot();
}

// OFFSET: 0x00D97780
// FUNCTION: fn_22302
void CLoginMode::SubD97780(int value) {
  int timeout = 0x3A98;
  if (g_serviceType == 5 || g_serviceType == 7) {
    timeout = 0x1D4C0;
  }

  switch (value) {
  case 4:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + timeout) {
      this->SendMsg(LMM_DISCONNECT_ACSVR, 0, 0, 0, 0);
    }
    if (g_serviceType == 5 || g_serviceType == 7) {
      if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x3A98) {
        if (this->waitWnd == nullptr) {
          this->waitWnd = g_windowMgr.EnsureWindow(WID_WAITWND);
          if (this->waitWnd == nullptr) {
            break;
          }
        }
        this->waitWnd->SendMsg(nullptr, UM_ENABLE_CANCEL, 0, 0, 0, 0);
        this->waitWnd->Invalidate();
      }
    }
    break;
  case 5:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x13880) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
        this->otpStarted = timeGetTime();
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
    }
    break;
  case 12:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x3A98) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
        this->otpStarted = timeGetTime();
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
    }
    break;
  case 6:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x3A98) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
        g_modeMgr.Quit();
      }
    }
    break;
  case 38:
    if (this->otpPending == true) {
      if (static_cast<int>(timeGetTime()) >= this->otpStarted + 0xEA60) {
        GetRagConnection().Close();
        if (this->loginResponseReceived == true && this->otpPending == true) {
          this->otpTimedOut = true;
          if (g_windowMgr.HasModalWindow() != true) {
            if (g_windowMgr.Confirm(LookupMessage(MSI_PRIVATE_AIRPLANE_FAILED_SAME_MAP), 0, true, false, 0, "Message", 280, 120, 0) == 0xB8) {
              this->otpPending = false;
              this->otpTimedOut = false;
              this->loginResponseReceived = false;
              this->otpStarted = timeGetTime();
              this->pendingCursorType = 3;
            }
          }
        }
      }
    } else {
      if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x13880) {
        GetRagConnection().Close();
        if (this->loginResponseReceived == false) {
          if (g_windowMgr.Confirm(LookupMessage(MSI_NO_PARTY_FOUND), 0, true, false, 0, "Message", 280, 120, 0) == 0xB8) {
            this->loginResponseReceived = false;
            this->otpStarted = timeGetTime();
            this->pendingCursorType = 3;
          }
        }
      }
    }
    break;
  default:
    break;
  }
}

// OFFSET: 0x00D97B20
// FUNCTION: fn_22303
void CLoginMode::SubD97B20(int) {
}

// OFFSET: 0x00D97D30
// FUNCTION: fn_22305
void CLoginMode::Update() {
  PumpApplicationMessages();
  this->PollNetworkStatus();
  this->SendKeepalive();
  g_mouse.Update();
  g_windowMgr.ProcessMouse();
  if (this->running != 0) {
    this->SubD97780(this->cursorType);
    this->ExpireTooltip();
  }

  g_timeScheduler->Process();
  g_asyncWorkMgr->Update(1000);
  g_ggHashMgr->VerifyPendingFiles();
  if (g_launchPassCommandLine) {
    Base_RunCallback();
  }

  if (g_restoreSurfaces != 0) {
    g_restoreSurfaces = 0;
    GetRenderSystemHolder()->TestDevice();
    g_windowMgr.InvalidateAll();
    CBitmapRes* bitmap = reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str())));
    g_windowMgr.SetBackgroundSurface(bitmap);
  }

  if (g_isAppActive == 0 && GetRenderSystemHolder()->IsWindowed()) {
    return;
  }

  if (ShouldRenderFrame()) {
    g_renderer->ClearDepth(1);
    g_windowMgr.UpdateFrameWindows();
    if (g_serviceType == 0 || g_serviceType == 4 || g_serviceType == 8 || g_serviceType == 9 || g_serviceType == 3 || g_serviceType == 10 ||
        g_serviceType == 1 || g_serviceType == 11 || g_serviceType == 12 || g_serviceType == 5 || g_serviceType == 7 || g_serviceType == 14 ||
        g_serviceType == 20 || g_serviceType == 15) {
      g_windowMgr.DrawLoginOverlay();
      if (this->cursorType == 0) {
        g_windowMgr.DrawCenteredBackground();
      }
    } else {
      g_windowMgr.DrawCenteredBackground();
    }
    g_windowMgr.DrawWindows();
    this->DrawCursor();
    this->DrawFadeIn();
    g_renderer->Draw();
    g_renderer->Present(1);
  }

  this->TakeScreenshot();
}

// OFFSET: 0x00D97EF0
// FUNCTION: fn_22306
void CLoginMode::SubD97EF0(int) {
}

// OFFSET: 0x00D97F00
// FUNCTION: fn_22307
void CLoginMode::SubD97F00(int) {
}

// OFFSET: 0x00D97F10
// FUNCTION: fn_22308
void CLoginMode::OpenAccountRegistration() {
  switch (g_serviceType) {
  case ServiceKorea:
    if (*reinterpret_cast<const int*>(&g_session.mouseAndChannelOptions[0x198]) == 1) {
      const char* url = LookupMessage(MSI_SETTLE_WEB_URL_HANGAME);
      constexpr std::array<char, 5> source = CRYPT_NAME("open", 0xCFF3899D95AF355FULL);
      ShellExecuteA(hWnd, DecryptName<0xCFF3899D95AF355FULL>(source), url, nullptr, nullptr, SW_SHOWNORMAL);
    } else {
      const char* url = LookupMessage(MSI_SETTLE_WEB_URL);
      constexpr std::array<char, 5> source = CRYPT_NAME("open", 0xE52DF30DCBAF7125ULL);
      ShellExecuteA(hWnd, DecryptName<0xE52DF30DCBAF7125ULL>(source), url, nullptr, nullptr, SW_SHOWNORMAL);
    }
    break;
  case 1: {
    constexpr std::array<char, 61> urlSource = CRYPT_NAME("https://iro.ragnarokonline.com/member/memberjoin1.asp?mNum=1", 0x13E33FA1A591AB67ULL);
    const char* file = DecryptName<0x13E33FA1A591AB67ULL>(urlSource);
    constexpr std::array<char, 5> source = CRYPT_NAME("open", 0x17A9E5BF39AD4737ULL);
    ShellExecuteA(hWnd, DecryptName<0x17A9E5BF39AD4737ULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
    break;
  }
  case ServiceJapan:
    break;
  case ServiceChina: {
    constexpr std::array<char, 54> urlSource = CRYPT_NAME("http://t.kunlun.com/?act=voucher.main&cid=1000&pid=61", 0x9949BFD527532943ULL);
    const char* file = DecryptName<0x9949BFD527532943ULL>(urlSource);
    constexpr std::array<char, 5> source = CRYPT_NAME("open", 0xCD8F077123FB7D6FULL);
    ShellExecuteA(hWnd, DecryptName<0xCD8F077123FB7D6FULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
    break;
  }
  default: {
    const char* url = LookupMessage(MSI_SETTLE_WEB_URL);
    constexpr std::array<char, 5> source = CRYPT_NAME("open", 0xC517FB932D95BB6DULL);
    ShellExecuteA(hWnd, DecryptName<0xC517FB932D95BB6DULL>(source), url, nullptr, nullptr, SW_SHOWNORMAL);
    break;
  }
  }
}

// OFFSET: 0x00D92B70
// FUNCTION: fn_22292
bool sub_D92B70() {
  return false;
}

// OFFSET: 0x00D92D90
// FUNCTION: fn_22294
void __stdcall nullsub_486(int) {
}

// OFFSET: 0x00D92DA0
// FUNCTION: fn_22295
void CLoginMode::ReceiveRefuseLogin(const RefuseLoginPacket* packet) {
  char text[260];
  memset(text, 0, sizeof(text));
  {
    const char* message = LookupMessage(MSI_NAVIGATION_ROADLISTVIEW);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0x891B232D09451965ULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x891B232D09451965ULL>(source), message);
  }

  switch (packet->result) {
  case 1: {
    const char* message = LookupMessage(MSI_TAREN_LOGINREFUSE_FAIL_PERSONALNUMBER);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0x758F1D0B4F7705B9ULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x758F1D0B4F7705B9ULL>(source), message);
    break;
  }
  case 2: {
    const char* message = LookupMessage(MSI_TAREN_LOGINREFUSE_FAIL_ACCOUNT_BLOCK);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0x35E5656B5DEDCD75ULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x35E5656B5DEDCD75ULL>(source), message);
    break;
  }
  case 3: {
    const char* message = LookupMessage(MSI_TAREN_LOGINREFUSE_FAIL_SYSTEM);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0x4941FD9F21BBB9FDULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x4941FD9F21BBB9FDULL>(source), message);
    break;
  }
  case 4: {
    const char* message = LookupMessage(MSI_TAREN_LOGINREFUSE_FAIL_UNKNOWN);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0xE7978FB5A94DD7FDULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0xE7978FB5A94DD7FDULL>(source), message);
    break;
  }
  case 5: {
    const char* message = LookupMessage(MSI_NAVIGATION_ICONTITEL);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0xB7FD8F670FC7B96BULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0xB7FD8F670FC7B96BULL>(source), message);
    break;
  }
  case 6: {
    const char* message = LookupMessage(MSI_NAVIGATION_RUITELISTTITEL);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0x39FF7FB50DEB998FULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x39FF7FB50DEB998FULL>(source), message);
    break;
  }
  case 7: {
    const char* message = LookupMessage(MSI_NAVIGATION_V4_HELP02);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0x2FE993D1DDE1F197ULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x2FE993D1DDE1F197ULL>(source), message);
    break;
  }
  case 8:
  case 100:
  case 102: {
    const char* message = LookupMessage(MSI_NAVIGATION_ROADLISTVIEW);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0xAD075B8DB599F1CDULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0xAD075B8DB599F1CDULL>(source), message);
    break;
  }
  case 9: {
    const char* message = LookupMessage(MSI_ZEROQUEST_SIMPLEUI_ON);
    constexpr std::array<char, 3> source = CRYPT_NAME("%s", 0x2F8B0F37118D157FULL);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x2F8B0F37118D157FULL>(source), message);
    break;
  }
  case 101: {
    char blockDate[21] = {0};
    memcpy_s(blockDate, 20, packet->blockDate, 20);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_LOGIN_REFUSE_BLOCKED_UNTIL), blockDate);
    break;
  }
  default:
    break;
  }

  GetRagConnection().Close();
  const int result = packet->result;
  constexpr std::array<char, 7> source = CRYPT_NAME("%s(%d)", 0x050505679903579BULL);
  g_windowMgr.Confirm(FormatString(DecryptName<0x050505679903579BULL>(source), text, result), 0, true, false, 0, "Message", 280, 120, 0);
  g_charServerAccountIdPending = 0;
  this->pendingCursorType = 3;
}

// OFFSET: 0x00D93B10
// FUNCTION: fn_22296
void __stdcall nullsub_487(int) {
}

// OFFSET: 0x00D92B60
// FUNCTION: fn_22291
bool sub_D92B60(int) {
  return false;
}

// OFFSET: 0x00D9A230
// FUNCTION: fn_22310
void PreloadLoginSprites() {
  PreloadCommonSprites();
}

// OFFSET: 0x00D9CFD0
// FUNCTION: fn_22315
void CLoginMode::ReceiveLoginErrorDetail(const LoginErrorDetailPacket* packet) {
  this->loginResponseReceived = true;
  std::string message;
  switch (packet->error) {
  case 0:
    SecureZeroMemory(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText));
    memcpy_s(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText), packet->text,
      std::min<std::size_t>(sizeof(g_session.m_loginErrorText), packet->length - 28));
    GetRagConnection().Close();
    g_modeMgr.gameMode->SendMsg(LMM_CONNECT_TO_ACSVR, 0, 0, 0, 0);
    break;
  case 400:
    if (g_windowMgr.LookupWindow(WID_LOGINOTPWND)) {
      g_windowMgr.CloseWindow(WID_LOGINOTPWND);
    }
    g_windowMgr.EnsureWindow(WID_LOGINOTPWND);
    this->otpPending = true;
    this->otpStarted = timeGetTime();
    break;
  case 500:
    message = LookupMessage(MSI_OTP_FAILED);
    break;
  case 600:
    message = LookupMessage(MSI_INCORRECT_PASSWORD);
    break;
  case 700:
    message = LookupMessage(MSI_AUTHORIZATION_CODE);
    break;
  case 900: {
    ShowErrorDialog(LookupMessage(MSI_CHECK_ATTENDANCE_REWARD), 0);
    constexpr std::array<char, 21> parametersSource = CRYPT_NAME("http://ro.gnjoy.com/", 0x8783C33941E15117ULL);
    const char* parametersText = DecryptName<0x8783C33941E15117ULL>(parametersSource);
    constexpr std::array<char, 21> fileSource = CRYPT_NAME("http://ro.gnjoy.com/", 0x1767DDE18D779D8BULL);
    const char* fileText = DecryptName<0x1767DDE18D779D8BULL>(fileSource);
    constexpr std::array<char, 5> operationSource = CRYPT_NAME("OPEN", 0xC1BB8111EDD183E1ULL);
    ShellExecuteA(nullptr, DecryptName<0xC1BB8111EDD183E1ULL>(operationSource), fileText, parametersText, nullptr, SW_SHOW);
    g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
    return;
  }
  case 1200:
    message = LookupMessage(4355);
    break;
  default:
    message = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
    break;
  }

  if (!message.empty()) {
    const int error = packet->error;
    constexpr std::array<char, 5> codeSource = CRYPT_NAME("(%d)", 0x271B45771F83DD9FULL);
    message += FormatOwnedString(DecryptName<0x271B45771F83DD9FULL>(codeSource), error);
    constexpr std::array<char, 6> detailSource = CRYPT_NAME(" [%s]", 0xC70319B5EB61598FULL);
    message += FormatOwnedString(DecryptName<0xC70319B5EB61598FULL>(detailSource), packet->detail);
    g_windowMgr.Confirm(message.c_str(), 5, true, true, 0, "Message", 280, 120, 0);
    if (this->otpTimedOut != true) {
      if (error == 500) {
        this->otpStarted = timeGetTime();
        if (g_windowMgr.LookupWindow(WID_LOGINOTPWND)) {
          g_windowMgr.CloseWindow(WID_LOGINOTPWND);
        }
        g_windowMgr.EnsureWindow(WID_LOGINOTPWND);
      } else {
        if (g_windowMgr.LookupWindow(WID_LOGINOTPWND)) {
          g_windowMgr.CloseWindow(WID_LOGINOTPWND);
        }
        this->loginResponseReceived = false;
        this->otpPending = false;
        GetRagConnection().Close();
        this->otpStarted = timeGetTime();
        this->pendingCursorType = 3;
      }
    }
  }
}

// OFFSET: 0x00D9D730
// FUNCTION: fn_22316
void CLoginMode::ReceiveLoginError(const LoginErrorPacket* packet) {
  this->loginResponseReceived = true;
  std::string message;
  switch (packet->error) {
  case 0:
    SecureZeroMemory(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText));
    memcpy_s(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText), packet->text,
      std::min<std::size_t>(sizeof(g_session.m_loginErrorText), packet->length - 8));
    GetRagConnection().Close();
    g_modeMgr.gameMode->SendMsg(LMM_CONNECT_TO_ACSVR, 0, 0, 0, 0);
    break;
  case 400:
    if (g_windowMgr.LookupWindow(WID_LOGINOTPWND)) {
      g_windowMgr.CloseWindow(WID_LOGINOTPWND);
    }
    g_windowMgr.EnsureWindow(WID_LOGINOTPWND);
    this->otpPending = true;
    this->otpStarted = timeGetTime();
    break;
  case 500:
    message = LookupMessage(MSI_OTP_FAILED);
    break;
  case 600:
    message = LookupMessage(MSI_INCORRECT_PASSWORD);
    break;
  case 700:
    message = LookupMessage(MSI_AUTHORIZATION_CODE);
    break;
  case 800:
    message = LookupMessage(MSI_NO_PARTY_FOUND);
    break;
  default:
    message = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
    break;
  }

  if (!message.empty()) {
    const int error = packet->error;
    const char* caption;
    bool protect;
    if (error == 800) {
      constexpr std::array<char, 7> captionSource = CRYPT_NAME("메세지", 0x75ADC57D87EFBD8FULL);
      caption = DecryptName<0x75ADC57D87EFBD8FULL>(captionSource);
      protect = true;
    } else {
      constexpr std::array<char, 5> codeSource = CRYPT_NAME("(%d)", 0xD1413DBB01CD8B51ULL);
      message += FormatOwnedString(DecryptName<0xD1413DBB01CD8B51ULL>(codeSource), error);
      caption = "Message";
      protect = false;
    }
    g_windowMgr.Confirm(message.c_str(), 5, true, true, 0, caption, 280, 120, protect);
    if (this->otpTimedOut != true) {
      if (error == 500) {
        this->otpStarted = timeGetTime();
        if (g_windowMgr.LookupWindow(WID_LOGINOTPWND)) {
          g_windowMgr.CloseWindow(WID_LOGINOTPWND);
        }
        g_windowMgr.EnsureWindow(WID_LOGINOTPWND);
      } else {
        if (g_windowMgr.LookupWindow(WID_LOGINOTPWND)) {
          g_windowMgr.CloseWindow(WID_LOGINOTPWND);
        }
        this->loginResponseReceived = false;
        this->otpPending = false;
        GetRagConnection().Close();
        this->otpStarted = timeGetTime();
        this->pendingCursorType = 3;
      }
    }
  }
}

// OFFSET: 0x00D9E170
// FUNCTION: fn_22319
void CLoginMode::SubD9E170(int) {
}

// GLOBAL: gl_24602
int g_modalImmediate;

// OFFSET: 0x00D97B30
// FUNCTION: fn_22304
int CLoginMode::Run() {
  while (this->running) {
    if (g_modalImmediate) {
      break;
    }

    if (g_windowMgr.modalWindows.empty() || g_windowMgr.modalWindows.back() == nullptr) {
      if (this->pendingCursorType != -1) {
        this->cursorType = this->pendingCursorType;
        this->cursorFrame = 0;
        this->pendingCursorType = -1;
        this->OnChangeState(this->cursorType);
      }
    }

    if (g_nProtectGameGuardMgr->HasError()) {
      GetRagConnection().Close();
      g_modeMgr.Quit();
      std::string message = g_nProtectGameGuardMgr->ErrorMessage();
      if (!message.empty()) {
        constexpr std::array<char, 19> source = CRYPT_NAME("nProtect GameGuard", 0x2D51E3B7236175EDULL);
        MessageBoxA(hWnd, message.c_str(), DecryptName<0x2D51E3B7236175EDULL>(source), 0);
      }
    }

    this->Update();
    ++this->cursorFrame;
  }

  FadeOutMode(*g_renderer, *GetRenderSystemHolder()->system, ResourceMgr::Instance(), 0, false, 0.0f, 0.0f);
  return 1;
}

#pragma pack(push, 1)
struct NotifyZoneServerPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t gid;
  /* 0006 */ unsigned char mapName[16];
  /* 0016 */ std::uint32_t ip;
  /* 001A */ std::int16_t port;
};

struct NotifyZoneServer2Packet {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t gid;
  /* 0006 */ unsigned char mapName[16];
  /* 0016 */ std::uint32_t ip;
  /* 001A */ std::int16_t port;
  /* 001C */ char host[128];
};
#pragma pack(pop)

// GLOBAL: gl_25028
ConnectionEndpoint g_zoneServerEndpoint;

// OFFSET: 0x00D929A0
// FUNCTION: fn_22290
void CLoginMode::ConnectZoneServer(std::uint32_t gid, std::string mapName, std::uint32_t ip, int port, std::string host) {
  g_session.m_gid = gid;
  g_modeMgr.gameMode->SendMsg(LMM_SELECT_CHARACTER, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0);
  if (g_windowMgr.newSelectCharWnd) {
    g_windowMgr.Schedule(g_windowMgr.newSelectCharWnd);
  }

  std::string name = mapName;
  name.resize(name.size() - 4);
  g_session.SetNavigationPlayerMap(name.c_str());

  in_addr address;
  address.s_addr = ip;
  strcpy(g_zoneServerEndpoint.address, inet_ntoa(address));
  g_zoneServerEndpoint.port = port;
  memcpy_s(g_zoneServerEndpoint.overrideAddress, sizeof(g_zoneServerEndpoint.overrideAddress), host.c_str(), std::min<std::size_t>(sizeof(g_zoneServerEndpoint.overrideAddress), host.size() + 1));

  GetRagConnection().Close();
  this->pendingCursorType = 12;
}

// OFFSET: 0x00D90740
// FUNCTION: fn_22285
__declspec(noinline) void CLoginMode::ReceiveNotifyZoneServer(const NotifyZoneServerPacket* packet) {
  this->ConnectZoneServer(packet->gid, std::string(std::begin(packet->mapName), std::end(packet->mapName)).c_str(), packet->ip, packet->port, std::string());
}

// OFFSET: 0x00D971E0
// FUNCTION: fn_22299
void CLoginMode::ReceiveNotifyZoneServer2(const NotifyZoneServer2Packet* packet) {
  std::string host(std::begin(packet->host), std::end(packet->host));
  this->ConnectZoneServer(packet->gid, std::string(std::begin(packet->mapName), std::end(packet->mapName)).c_str(), packet->ip, packet->port, host.c_str());
}

#pragma pack(push, 1)
struct PasswordModePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t accountId;
  /* 0006 */ std::int32_t value;
  /* 000A */ std::uint16_t mode;
  /* 000C */ unsigned char count;
};
#pragma pack(pop)

// OFFSET: 0x00D90C90
// FUNCTION: fn_22286
void CLoginMode::SubD90C90(const PasswordModePacket* packet) {
  g_session.m_loginAccountId = packet->accountId;
  switch (packet->mode) {
  case PINCODE_OK:
    g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
    g_session.m_passwordWndMode = 0;
    this->pendingCursorType = 7;
    break;
  case PINCODE_PASSED:
    g_session.m_passwordWndMode = 1;
    this->pendingCursorType = 7;
    break;
  case PINCODE_WRONG:
    if (packet->count <= 0) {
      g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
      g_windowMgr.Confirm(LookupMessage(MSI_ACCOUNTLINKEDUSERDATA_LOAD_FAIL), 0, true, false, 0, "Message", 280, 120, 0);
      this->SendMsg(MM_QUIT, 0, 0, 0, 0);
    } else {
      g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
      this->pendingCursorType = 7;
      g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_SECOND_PASSWORD_LOGIN_FAILED), packet->count).c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    }
    break;
  case PINCODE_ILLEGAL:
    g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
    this->pendingCursorType = 7;
    g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_RESTRICT_PW), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case PINCODE_KSSN:
    g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
    this->pendingCursorType = 7;
    g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_PERSONALNUM_PW), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case PINCODE_NOTSET:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 4;
    this->pendingCursorType = 7;
    break;
  case PINCODE_EXPIRED:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 3;
    this->pendingCursorType = 7;
    break;
  case PINCODE_NEW:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 5;
    this->pendingCursorType = 7;
    break;
  case PINCODE_ASK:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 2;
    this->passwordRetryCount = 0;
    this->pendingCursorType = 7;
    break;
  case 9: {
    const char* text = LookupMessage(MSI_NAVIGATION_RUITELISTTITEL);
    constexpr std::array<char, 7> source = CRYPT_NAME("%s(%d)", 0x0B8745519B1F95D7ULL);
    g_windowMgr.Confirm(FormatString(DecryptName<0x0B8745519B1F95D7ULL>(source), text, 9), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 10: {
    const char* text = LookupMessage(MSI_NAVIGATION_RUITELISTTITEL);
    constexpr std::array<char, 7> source = CRYPT_NAME("%s(%d)", 0xA597A7E7492B39D5ULL);
    g_windowMgr.Confirm(FormatString(DecryptName<0xA597A7E7492B39D5ULL>(source), text, 10), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 11: {
    const char* text = LookupMessage(MSI_NAVIGATION_RUITELISTTITEL);
    constexpr std::array<char, 7> source = CRYPT_NAME("%s(%d)", 0xFFABE3D5D1BBEF73ULL);
    g_windowMgr.Confirm(FormatString(DecryptName<0xFFABE3D5D1BBEF73ULL>(source), text, 11), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  }
}

// OFFSET: 0x00D911C0
// FUNCTION: fn_22287
void CLoginMode::SubD911C0(const PasswordModePacket* packet) {
  g_session.m_loginAccountId = packet->accountId;
  switch (packet->mode) {
  case PINCODE_OK:
    g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
    g_session.m_passwordWndMode = 0;
    this->pendingCursorType = 7;
    break;
  case PINCODE_PASSED:
    g_session.m_passwordWndMode = 1;
    this->pendingCursorType = 7;
    break;
  case PINCODE_WRONG: {
    g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
    ++this->passwordRetryCount;
    this->pendingCursorType = 7;
    if (3 - this->passwordRetryCount <= 0) {
      g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_INFO_3), 0, true, false, 0, "Message", 280, 120, 0);
      this->SendMsg(MM_QUIT, 0, 0, 0, 0);
    } else {
      g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_SECOND_PASSWORD_LOGIN_FAILED), 3 - this->passwordRetryCount).c_str(), 0, true, false, 0,
        "Message", 280, 120, 0);
    }
    break;
  }
  case PINCODE_ILLEGAL:
    g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
    this->pendingCursorType = 7;
    g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_RESTRICT_PW), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case PINCODE_KSSN:
    g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
    this->pendingCursorType = 7;
    g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_PERSONALNUM_PW), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case PINCODE_NOTSET:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 4;
    this->pendingCursorType = 7;
    break;
  case PINCODE_EXPIRED:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 3;
    this->pendingCursorType = 7;
    break;
  case PINCODE_NEW:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 5;
    this->pendingCursorType = 7;
    break;
  case PINCODE_ASK:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 2;
    this->passwordRetryCount = 0;
    this->pendingCursorType = 7;
    break;
  case 9: {
    const char* text = LookupMessage(MSI_NAVIGATION_RUITELISTTITEL);
    constexpr std::array<char, 7> source = CRYPT_NAME("%s(%d)", 0x49D33529F1556333ULL);
    g_windowMgr.Confirm(FormatString(DecryptName<0x49D33529F1556333ULL>(source), text, 9), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 10: {
    const char* text = LookupMessage(MSI_NAVIGATION_RUITELISTTITEL);
    constexpr std::array<char, 7> source = CRYPT_NAME("%s(%d)", 0x01CB4F1DF9D3F931ULL);
    g_windowMgr.Confirm(FormatString(DecryptName<0x01CB4F1DF9D3F931ULL>(source), text, 10), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 11: {
    const char* text = LookupMessage(MSI_NAVIGATION_RUITELISTTITEL);
    constexpr std::array<char, 7> source = CRYPT_NAME("%s(%d)", 0xC5B1BBCD9BC9C997ULL);
    g_windowMgr.Confirm(FormatString(DecryptName<0xC5B1BBCD9BC9C997ULL>(source), text, 11), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  }
}

// OFFSET: 0x00D97410
// FUNCTION: fn_22300
void CLoginMode::Initialize(const char* name) {
  g_soundMgr->StopStreams();
  this->charParams[0] = 5;
  this->charParams[1] = 5;
  this->charParams[2] = 5;
  this->charParams[3] = 5;
  this->charParams[4] = 5;
  this->charParams[5] = 5;
  this->characterParameter7 = 0;
  this->characterParameter8 = 1;
  this->makingCharNameFirstByte = false;
  this->wallpaperName = g_loginWallpaperName;
  this->running = 1;
  this->pendingCursorType = -1;
  std::memset(this->billingInfo, 0, sizeof(this->billingInfo));
  this->cursorType = g_loginStartMode;
  this->cursorFrame = 0;
  this->pendingCursorType = -1;
  this->OnChangeState(g_loginStartMode);
  g_loginStartMode = 3;
  g_isStopByLogin = 1;
  g_session.ClearItemContainers();
  g_guildInfo.Init();
  PreloadCommonSprites();
  if (g_serviceOptions.Get(7)) {
    g_soundMgr->PlayStream("bgm\\01.mp3", 0, false);
  }
  this->slotExtension = CharacterSlotExtension();
  g_session.ClearAllInventoryCategories();
  g_entryQueueMgr->ready = 0;
  g_entryQueueMgr->current = nullptr;
  g_session.ClearShortcutSlots();
  g_session.ClearSecondaryShortcutSlots();
  g_session.playerAppearanceState = 0;
  this->serverCount = 0;
  this->characterCount = 0;
  this->loginResponseReceived = false;
  this->otpPending = false;
  this->otpTimedOut = false;
  this->remainingLoginSeconds = 0;
  g_session.madogearType = 0;
  g_session.summonHp.clear();
  g_session.summonMaxHp.clear();
}

#pragma pack(push, 1)
struct AcceptEnterPositionPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t startTime;
  /* 0006 */ unsigned char posDir[3];
  /* 0009 */ unsigned char xSize;
  /* 000A */ unsigned char ySize;
  /* 000B */ unsigned char font;
};
#pragma pack(pop)

// GLOBAL: gl_25025
bool g_pingLogEnabled;

// GLOBAL: gl_25026
std::uint32_t g_pingLogFirstTick;

// GLOBAL: gl_25027
std::list<std::uint32_t> g_pingLogTicks;

// OFFSET: 0x00D9DC50
// FUNCTION: fn_22317
void CLoginMode::Zc_Accept_Enter3(const AcceptEnterPositionPacket* packet) {
  g_session.m_startTimeTicks = std::chrono::system_clock::now().time_since_epoch().count();
  g_session.m_loginTimeTicks = std::chrono::system_clock::now().time_since_epoch().count();
  g_session.SetServerTime(packet->startTime);
  g_session.SetOption(11, 1, 0);
  const int x = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  const int y = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);
  const int dir = packet->posDir[2] & 0xF;
  g_session.SetPlayerPosition(x, y, dir);
  g_session.chatFont[0] = packet->font;

  char mapName[36];
  sprintf(mapName, "%s.rsw", g_session.m_navigationPlayerMap);
  g_modeMgr.Switch(1, mapName);
  static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_PLEASE_BE_PATIENT), 17, 1);
  GetRagConnection().SetBlock(0);
  g_session.SetOption(17, 0, 0);

  if (g_pingLogEnabled) {
    FILE* file = fopen("PingLog.txt", "at");
    if (file) {
      __time32_t now;
      _time32(&now);
      fprintf(file, "\n");
      fprintf(file, "server:%s\n", g_session.m_accountName.c_str());
      fprintf(file, "ip:%s\n", g_zoneServerEndpoint.address);
      fprintf(file, "map:%s.gat\n", g_session.m_navigationPlayerMap);
      fprintf(file, "time:%s\n\n", _ctime32(&now));
      fclose(file);
    }
    g_pingLogTicks.clear();
    g_pingLogFirstTick = 0;
  }

  g_session.SetReplayPacketMode(0);
  g_session.characterNames.clear();
  for (unsigned int i = 0; i < 68; ++i) {
    std::string name = this->characters[i].name;
    g_session.characterNames.push_back(name);
  }

  SecureZeroMemory(this->characters, sizeof(this->characters));
  g_renewQuestMgr->m_newQuests.clear();
  g_renewQuestMgr->m_statusRequests.clear();
}

// OFFSET: 0x00D9DFC0
// FUNCTION: fn_22318
void CLoginMode::Zc_Accept_Enter2(const AcceptEnterPositionPacket* packet) {
  g_session.SetServerTime(packet->startTime);
  g_session.SetPlayerPosition((packet->posDir[0] << 2) | (packet->posDir[1] >> 6), ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4),
    packet->posDir[2] & 0xF);

  char mapName[36];
  sprintf(mapName, "%s.rsw", g_session.m_navigationPlayerMap);
  g_modeMgr.Switch(1, mapName);
  static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_PLEASE_BE_PATIENT), 17, 1);
  GetRagConnection().SetBlock(0);

  if (g_pingLogEnabled) {
    FILE* file = fopen("PingLog.txt", "at");
    if (file) {
      __time32_t now;
      _time32(&now);
      fprintf(file, "\n");
      fprintf(file, "server:%s\n", g_session.m_accountName.c_str());
      fprintf(file, "ip:%s\n", g_zoneServerEndpoint.address);
      fprintf(file, "map:%s.gat\n", g_session.m_navigationPlayerMap);
      fprintf(file, "time:%s\n\n", _ctime32(&now));
      fclose(file);
    }
    g_pingLogTicks.clear();
    g_pingLogFirstTick = 0;
  }
}

// GLOBAL: gl_25155
char g_loginFailCount;

// OFFSET: 0x00D91790
// FUNCTION: fn_22289
void CLoginMode::ShowLoginError(int code, int argument) {
  std::string message("");
  if (code != 0x12) {
    GetRagConnection().Close();
  }

  switch (code) {
  case 0x1393:
    message = LookupMessage(MSI_REFUSE_SSO_AUTH_INVALID_TOKEN);
    break;
  case 0x1394: {
    message = FormatOwnedString(LookupMessage(MSI_BLOCKED), argument);
    break;
  }
  case 0x1395:
  case 0x13BE:
    message = LookupMessage(MSI_WITHDREW_USERS);
    break;
  case 0x1397:
    if (g_launchPassCommandLine) {
      std::string url;
      if (g_stoveMgr->m_environment == 1) {
        url = "https://member.gate8.com/auth/login/by_game?redirect_url=https%3A%2F%2Fro.game.gate8.com%2Faccount%2Fingamepopup&access_token=";
      } else {
        url = "https://member.onstove.com/auth/login/by_game?redirect_url=https://ro.game.onstove.com/account/ingamepopup/&access_token=";
      }
      url += g_stoveMgr->GetAccessToken();
      constexpr std::array<char, 5> source = CRYPT_NAME("OPEN", 0xD98F5345098D630DULL);
      ShellExecuteA(nullptr, DecryptName<0xD98F5345098D630DULL>(source), url.c_str(), url.c_str(), nullptr, SW_SHOWNORMAL);
      message = "";
      g_modeMgr.Quit();
      return;
    }
    message = LookupMessage(MSI_REFUSE_SSO_AUTH_INVALID_USER);
    break;
  case 0x1399:
    message = LookupMessage(MSI_REFUSE_SSO_AUTH_INVALID_AGE);
    break;
  case 0x13C7:
    message = LookupMessage(MSI_ACCOUNT_BLOCK);
    break;
  case 0x13C8:
    message = LookupMessage(MSI_BUG_BLOCK);
    break;
  case 0x1396:
  case 0x1398:
  case 0x139B:
  case 0x139D:
  case 0x13BC:
  case 0x13BD:
  case 0x13BF:
  case 0x13C0:
  case 0x13C1:
  case 0x13C2:
  case 0x13C3:
  case 0x13C4:
  case 0x13C5:
    message = LookupMessage(MSI_SSO_ERROR_MSG);
    break;
  case 0x13BA:
    message = LookupMessage(MSI_REFUSE_SSO_NOT_PAY_USER);
    break;
  case 0x13BB:
    message = LookupMessage(MSI_DOUBLE_LOGIN_PROHIBITED);
    break;
  case 0x14B4:
    message = LookupMessage(MSI_RUSSIA_PERSONALINFO_DEATH);
    break;
  case 0x14B5:
    message = LookupMessage(MSI_RODEX_BUTTON_DELETE);
    break;
  case 33:
    message = LookupMessage(MSI_TITLE_EQUIPMENT);
    break;
  case REFUSE_INVALID_ID:
    message = LookupMessage(MSI_INCORRECT_USERID);
    break;
  case REFUSE_INVALID_PASSWD:
    if (g_serviceType == 0 && ++g_loginFailCount >= 3) {
      message = std::string(PasswordRetryExceededMessage{}().Text());
      if (g_windowMgr.Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
        if (g_session.m_channelCopId == 1) {
          constexpr std::array<char, 23> urlSource = CRYPT_NAME("http://www.hangame.com", 0xED796785D1DF137FULL);
          const char* file = DecryptName<0xED796785D1DF137FULL>(urlSource);
          constexpr std::array<char, 5> source = CRYPT_NAME("open", 0x83EFBFBB37EDBDBDULL);
          ShellExecuteA(hWnd, DecryptName<0x83EFBFBB37EDBDBDULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
        } else {
          constexpr std::array<char, 22> urlSource = CRYPT_NAME("http://ragnarok.co.kr", 0xA5FD29D7AB6DC59FULL);
          const char* file = DecryptName<0xA5FD29D7AB6DC59FULL>(urlSource);
          constexpr std::array<char, 5> source = CRYPT_NAME("open", 0xD97D5DBDBB2F2709ULL);
          ShellExecuteA(hWnd, DecryptName<0xD97D5DBDBB2F2709ULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
        }
        g_modeMgr.Quit();
      }
      message.clear();
    } else {
      message = LookupMessage(MSI_INCORRECT_LOGIN_PASSWORD);
    }
    break;
  case REFUSE_ID_EXPIRED:
    message = LookupMessage(MSI_ID_EXPIRED);
    break;
  case REFUSE_NOT_CONFIRMED:
  case 0x13C6:
    message = LookupMessage(MSI_ID_EMAIL_CONFIRM_NEEDED);
    break;
  case REFUSE_INVALID_VERSION:
    message = LookupMessage(MSI_INVALID_VERSION);
    break;
  case REFUSE_DELETED_ACCOUNT:
    message = LookupMessage(MSI_DELETED_ACCOUNT);
    break;
  case REFUSE_BLOCK_TEMPORARY: {
    message = FormatOwnedString(LookupMessage(MSI_LOGIN_REFUSE_BLOCKED_UNTIL), argument);
    break;
  }
  case REFUSE_TEMP_BAN_HACKING_INVESTIGATION: {
    message = FormatOwnedString(LookupMessage(MSI_HACKING_INVESTIGATION), argument);
    break;
  }
  case REFUSE_TEMP_BAN_BUG_INVESTIGATION:
    message = LookupMessage(MSI_BUG_INVESTIGATION);
    break;
  case REFUSE_TEMP_BAN_DELETING_CHAR:
    message = LookupMessage(MSI_DELETING_CHAR);
    break;
  case REFUSE_TEMP_BAN_DELETING_SPOUSE_CHAR:
    message = LookupMessage(MSI_DELETING_SPOUSE_CHAR);
    break;
  case 7:
    message = LookupMessage(MSI_ACCOUNT_BUSY);
    break;
  case REFUSE_NONSAKRAY_ID_BLOCKED:
    message = LookupMessage(MSI_REFUSE_NONSAKRAY_ID_BLOCKED);
    break;
  case REFUSE_BAN_BY_DBA:
    message = LookupMessage(MSI_REFUSE_BAN_BY_DBA);
    break;
  case REFUSE_EMAIL_NOT_CONFIRMED:
    message = LookupMessage(MSI_REFUSE_EMAIL_NOT_CONFIRMED);
    break;
  case REFUSE_BAN_BY_GM:
    message = LookupMessage(MSI_REFUSE_BAN_BY_GM);
    break;
  case REFUSE_TEMP_BAN_FOR_DBWORK:
    message = LookupMessage(MSI_REFUSE_TEMP_BAN_FOR_DBWORK);
    break;
  case REFUSE_SELF_LOCK:
    message = LookupMessage(MSI_REFUSE_SELF_LOCK);
    break;
  case REFUSE_NOT_PERMITTED_GROUP:
    message = LookupMessage(MSI_REFUSE_NOT_PERMITTED_GROUP);
    break;
  case REFUSE_WAIT_FOR_SAKRAY_ACTIVE:
    message = LookupMessage(MSI_REFUSE_WAIT_FOR_SAKRAY_ACTIVE);
    break;
  case REFUSE_ALREADY_CONNECT: {
    message = FormatOwnedString(LookupMessage(MSI_ALREADY_CONNECT), argument);
    break;
  }
  case 16:
    if (g_serviceType == 0) {
      constexpr std::array<char, 48> textSource = CRYPT_NAME("비밀 번호 변경 후, 다시 접속 해주시기 바랍니다.", 0xF14F79170939C7F1ULL);
      message = std::string(DecryptName<0xF14F79170939C7F1ULL>(textSource));
      if (g_windowMgr.Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
        constexpr std::array<char, 65> urlSource = CRYPT_NAME("http://www.ragnarok.co.kr/setNewPW/password_change_agreement.asp", 0x7F2DFFC1ABE91F2BULL);
        const char* file = DecryptName<0x7F2DFFC1ABE91F2BULL>(urlSource);
        constexpr std::array<char, 5> source = CRYPT_NAME("open", 0xFD69A9B3F311818DULL);
        ShellExecuteA(hWnd, DecryptName<0xFD69A9B3F311818DULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
        g_modeMgr.Quit();
      }
      message.clear();
    }
    break;
  case ACCEPT_LOGIN_USER_PHONE_BLOCK:
    message = LookupMessage(MSI_PHONE_CONFIRM);
    break;
  case REFUSE_USER_PHONE_BLOCK:
    message = LookupMessage(MSI_PHONE_BLOCK);
    break;
  case 17: {
    message = FormatOwnedString(LookupMessage(MSI_RE17), argument);
    if (g_windowMgr.Confirm(message.c_str(), 2, true, true, 0, "Message", 280, 120, 0) == 0xBB) {
      g_modeMgr.Quit();
    }
    message.clear();
    break;
  }
  case 18:
    message = LookupMessage(MSI_RE18);
    break;
  case 19:
    message = LookupMessage(MSI_DB_ERROR);
    break;
  case 20:
    message = LookupMessage(MSI_OTP_FAILED);
    break;
  case 108:
    message = LookupMessage(MSI_MAIN_USER_CANONT_LOGIN_FREE_SERVER);
    break;
  case 109:
    message = LookupMessage(MSI_INVALID_ONETIMELIMIT);
    break;
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 27:
  case 30:
    message = LookupMessage(MSI_NO_PARTY_FOUND);
    break;
  case 28:
    message = LookupMessage(MSI_FAILED_MAIL_CAUSE_TO_USE_ITEM);
    break;
  case 29:
    message = LookupMessage(MSI_FAILIED_ONECLICK_ITEMIDENTIFY_NO_MAGNIFIER);
    break;
  case 21:
  case 0x139A:
    message = LookupMessage(MSI_SSO_FAILED);
    break;
  case 110:
    if (g_windowMgr.Confirm(LookupMessage(MSI_SHOUTDOWN_TIME), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
      const char* file = LookupMessage(MSI_PARTIAL_SHOUTDOWN_TIME);
      constexpr std::array<char, 5> source = CRYPT_NAME("open", 0x9B0F234DEFDF7593ULL);
      ShellExecuteA(hWnd, DecryptName<0x9B0F234DEFDF7593ULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
      g_modeMgr.Quit();
    }
    message = "";
    break;
  case 112:
    message = LookupMessage(MSI_NAVIGATION_HELP_3);
    break;
  case 242:
    message = LookupMessage(MSI_THE_PARTY_MASTER_CANNOT_BE_REQUESTED);
    break;
  case 36: {
    const std::div_t remaining = std::div(this->remainingLoginSeconds, 3600);
    message = FormatOwnedString(LookupMessage(MSI_CHECK_ATTENDANCE_RODEX_CONTENTS), remaining.quot, static_cast<int>(std::ceil(static_cast<float>(remaining.rem) / 60.0f)));
    break;
  }
  case 243:
    message = LookupMessage(MSI_UPGRADESKILLERROR_MORE_THIRDJOBSKILL);
    break;
  case 244:
    message = LookupMessage(MSI_TIPTAG_FOCUS_WARNING);
    break;
  default: {
    message = LookupMessage(MSI_ACCESS_DENIED);
    constexpr std::array<char, 5> source = CRYPT_NAME("(%d)", 0xC9712D676B5B3943ULL);
    message += FormatOwnedString(DecryptName<0xC9712D676B5B3943ULL>(source), code);
    break;
  }

  }

  if (!message.empty()) {
    g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    if (code == 5 || code == 99) {
      this->SendMsg(MM_QUIT, 0, 0, 0, 0);
    }
  }

  if (g_session.m_loginModeFlag == true) {
    this->pendingCursorType = 3;
  } else {
    g_modeMgr.Quit();
  }
}

// OFFSET: 0x00D8C6A0
// FUNCTION: fn_22258
CLoginMode::CLoginMode() : wallpaperName(), noticeCommands(), padding_6F2C(), accountInfo(), keycryptErrorInfo(), mapStates() {
  this->fadeInRequested = 1;
  this->nextKeepaliveTime = timeGetTime() + 12000;
  this->savedInlineCharset = g_inlineCharset;
  g_inlineCharset = 0;
  this->selectedAccount = -1;
  this->waitWnd = nullptr;
  this->passwordRetryCount = 0;
  SecureZeroMemory(this->characters, sizeof(this->characters));
  this->receivedCharacterCount = 0;
  this->padding_01DC = 0;
  this->characterDeleted = false;
}

// OFFSET: 0x00D8C990
// FUNCTION: fn_45557
CLoginMode::~CLoginMode() {
  g_inlineCharset = this->savedInlineCharset;
}

// OFFSET: 0x00D92B80
// FUNCTION: fn_22293
bool CLoginMode::ShowNotice(int notice, bool append) {
  UIWindow* window;
  if (append) {
    window = g_windowMgr.menuWnd;
  } else {
    this->noticeCommands.erase(this->noticeCommands.begin(), this->noticeCommands.end());
    if (g_windowMgr.menuWnd) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }
    window = g_windowMgr.EnsureWindow(WID_MENUWND);
  }

  if (!window) {
    return false;
  }

  switch (notice) {
  case 45:
    window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MSG_MAKECHARCTER)), 0, 0, 0);
    window->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
    this->noticeCommands.push_back(45);
    break;
  case 46:
    window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UAE_URL)), 0, 0, 0);
    window->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
    this->noticeCommands.push_back(46);
    break;
  case 47:
    window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UAE_SERVERSTR)), 0, 0, 0);
    window->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
    this->noticeCommands.push_back(47);
    break;
  }

  if (this->noticeCommands.empty()) {
    return false;
  }

  if (UIWindow* notices = g_windowMgr.menuWnd) {
    int x = g_mouse.x + 1;
    int y = g_mouse.y + 1;
    if (x < 1) {
      x = 1;
    }
    int maximumX = g_renderer->viewport.width - notices->geometry.width;
    if (x <= maximumX) {
      maximumX = x;
    }
    if (y < 1) {
      y = 1;
    }
    int maximumY = g_renderer->viewport.height - notices->geometry.height;
    if (y <= maximumY) {
      maximumY = y;
    }
    notices->Move(maximumX, maximumY);
  }

  return true;
}

static void ComputeLoginDigest(unsigned char* digest, const void* data, int size) {
}

// OFFSET: 0x00D8CD60
// FUNCTION: fn_22264
void CLoginMode::ReceiveAcceptLogin(const AcceptLoginPacket* packet, int version) {
  g_loginAcceptTime = timeGetTime();
  *reinterpret_cast<std::string*>(&g_session.autoReplyState[4]) = LookupMessage(MSI_AUTO_MSG);

  SessionAccount& account = *reinterpret_cast<SessionAccount*>(&g_session.m_sex);
  account.authCode = packet->authCode;
  account.userLevel = packet->userLevel;
  g_session.m_aid = packet->aid;

  unsigned char digest[16] = {};
  ComputeLoginDigest(digest, packet, offsetof(AcceptLoginPacket, body));
  g_pakRegistry.ApplyAuxiliary(digest, sizeof(digest));
  SecureZeroMemory(digest, sizeof(digest));

  if (packet->sex >= 10) {
    g_session.SetFlag(packet->sex - 10);
    g_session.m_sex = packet->sex - 10;
  } else {
    if (g_loginExitFlag == 1) {
      exit(0);
    }
    g_session.SetFlag(packet->sex);
    g_session.m_sex = packet->sex;
  }

  switch (g_serviceType) {
  case 6:
  case 10:
  case 12:
    g_serviceOptions.Set(0x6C, 0);
    break;
  }
  const unsigned char flags = g_serviceOptions.Get(0x6C);
  g_session.SetServiceFlags(flags, flags);

  account.lastLoginIp = packet->lastLoginIp;
  memcpy_s(account.lastLoginTime, sizeof(packet->lastLoginTime), packet->lastLoginTime, sizeof(packet->lastLoginTime));
  account.lastLoginTime[26] = '\0';

  unsigned int count;
  switch (version) {
  case 2:
    count = (packet->length - (offsetof(AcceptLoginPacket, body) + 4)) / sizeof(ServerAddress);
    this->serverCount = count;
    memcpy(this->serverList, packet->body + 4, count * sizeof(ServerAddress));
    break;
  case 3:
    count = (packet->length - (offsetof(AcceptLoginPacket, body) + 17)) / sizeof(ServerAddress);
    this->serverCount = count;
    memcpy(this->serverList, packet->body + 17, count * sizeof(ServerAddress));
    memcpy(&g_session.webServiceLoginData[8], packet->body, 16);
    break;
  case 4:
    count = (packet->length - offsetof(AcceptLoginPacket, body)) / sizeof(ServerAddress2);
    this->serverCount = count;
    memcpy(this->serverList, packet->body, count * sizeof(ServerAddress2));
    break;
  case 5:
    count = (packet->length - (offsetof(AcceptLoginPacket, body) + 17)) / sizeof(ServerAddress2);
    this->serverCount = count;
    memcpy(this->serverList, packet->body + 17, count * sizeof(ServerAddress2));
    memcpy(&g_session.webServiceLoginData[8], packet->body, 16);
    break;
  case 6:
  case 7:
  case 8:
  case 9:
    break;
  default:
    count = (packet->length - offsetof(AcceptLoginPacket, body)) / sizeof(ServerAddress);
    this->serverCount = count;
    memcpy(this->serverList, packet->body, count * sizeof(ServerAddress));
    break;
  }

  CNProtectGameGuardMgr* gameGuard = g_nProtectGameGuardMgr;
  const std::uint32_t aid = packet->aid;
  constexpr std::array<std::uint8_t, 3> source = CRYPT_NAME("%d", 0x637B959D55771F69ULL);
  thread_local EncryptedName<sizeof(source), 0x637B959D55771F69ULL> format(source);
  gameGuard->Send(FormatString(format.Text(), aid));
  GetRagConnection().Close();
  g_loginFailCount = 0;
  this->pendingCursorType = 6;
}

static void IgnoreRemovedPacket(const unsigned char* packet) {
}

// OFFSET: 0x00D98470
// FUNCTION: fn_22309
void CLoginMode::PollNetworkStatus() {
  if (g_replayMode) {
    return;
  }

  memset(g_gameModePacketBuffer, 0, sizeof(g_gameModePacketBuffer));
  if (!GetRagConnection().Update()) {
    g_modeMgr.gameMode->SendMsg(MM_SOCKETERROR, 0, 0, 0, 0);
  }

  if (g_charServerAccountIdPending == 1) {
    int accountId;
    if (GetRagConnection().Read(&accountId, 4, 1)) {
      g_charServerAccountIdPending = 0;
    }
    return;
  }

  while (GetRagConnection().ReadPacket(g_gameModePacketBuffer, nullptr, g_packetLengths)) {
    switch (GetRagConnection().GetPacketType(reinterpret_cast<const char*>(g_gameModePacketBuffer))) {
    case ID_AC_ACCEPT_LOGIN:
    case ID_HC_ACCEPT_MAKECHAR:
    case ID_PING:
    case ID_ZC_AID:
    case ID_HC_UPDATE_CHARINFO:
    case ID_AC_SHUTDOWN_INFO:
    case ID_HC_ACK_CHARINFO_PER_PAGE:
    case ID_HC_SECRETSCAN_DATA:
    case ID_AC_ACCEPT_LOGIN3:
    case ID_AC_GOOGLE_OTP:
    case ID_AC_ACCEPT_LOGIN4:
    case ID_ZC_SEND_EAC_MESSAGE:
    case ID_AC_REFUSE_LOGIN_USA:
    case 0xC20:
      break;
    case ID_AC_REFUSE_LOGIN: {
      RefuseLoginPacket* packet = reinterpret_cast<RefuseLoginPacket*>(g_gameModePacketBuffer);
      packet->blockDate[19] = '\0';
      this->ShowLoginError(packet->result, reinterpret_cast<int>(packet->blockDate));
      break;
    }
    case ID_HC_ACCEPT_ENTER:
      this->ReceiveAcceptEnter(reinterpret_cast<const AcceptEnterPacket*>(g_gameModePacketBuffer));
      break;
    case ID_HC_REFUSE_ENTER:
      GetRagConnection().Close();
      switch (reinterpret_cast<const ResultPacket*>(g_gameModePacketBuffer)->result) {
      case 1:
        g_windowMgr.Confirm(LookupMessage(MSI_ID_MISMATCH), 0, true, true, 0, "Message", 280, 120, 0);
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
        break;
      case ID_HC_ACCEPT_DELETECHAR:
        g_windowMgr.Confirm(LookupMessage(MSI_OVER_CONNECT_USER), 0, true, false, 0, "Message", 280, 120, 0);
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
        break;
      case ID_ZC_NOTIFY_UPDATECHAR:
        g_windowMgr.Confirm(LookupMessage(MSI_GRADE_ENCHANT_REFINING_LEVEL_LACK), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 6;
        break;
      case ID_ZC_NOTIFY_UPDATEPLAYER:
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_ASSISTANT_TRADE_DATE), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 6;
        break;
      default:
        g_windowMgr.Confirm(LookupMessage(MSI_ACCESS_DENIED), 0, true, false, 0, "Message", 280, 120, 0);
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
        break;
      }
      break;
    case ID_HC_REFUSE_MAKECHAR: {
      int message;
      switch (reinterpret_cast<const ResultPacket*>(g_gameModePacketBuffer)->result) {
      case 0:
        message = 10;
        break;
      case 1:
        message = 0x12A;
        break;
      case 2:
        message = 0x4F8;
        break;
      case 3:
        message = 0x54B;
        break;
      case 11:
        message = 0x553;
        break;
      case 12:
        message = 0x7E2;
        break;
      default:
        message = 11;
        break;
      }
      g_windowMgr.Confirm(LookupMessage(message), 0, true, false, 0, "Message", 280, 120, 0);
      this->pendingCursorType = 7;
      break;
    }
    case ID_HC_ACCEPT_DELETECHAR: {
      CharacterInfo* remaining = new CharacterInfo[this->characterCount];
      long long kept = 0;
      for (long long i = 0; i < this->characterCount; ++i) {
        if (this->characters[i].slot != static_cast<unsigned char>(g_selectedSlotIndex)) {
          memcpy_s(&remaining[kept], sizeof(CharacterInfo), &this->characters[i], sizeof(CharacterInfo));
          ++kept;
        }
      }
      --this->characterCount;
      --g_session.characterSlotState;
      memcpy_s(this->characters, sizeof(this->characters), remaining, this->characterCount * sizeof(CharacterInfo));
      delete[] remaining;
      this->pendingCursorType = 7;
      break;
    }
    case ID_HC_REFUSE_DELETECHAR: {
      int message;
      switch (reinterpret_cast<const ResultPacket*>(g_gameModePacketBuffer)->result) {
      case 0:
        message = g_serviceType == 0 ? 0x12F : 0x12D;
        break;
      case 1:
        message = 0x54C;
        break;
      case 2:
        message = 0x788;
        break;
      default:
        message = 12;
        break;
      }
      g_windowMgr.Confirm(LookupMessage(message), 0, true, false, 0, "Message", 280, 120, 0);
      this->pendingCursorType = 7;
      break;
    }
    case ID_HC_NOTIFY_ZONESVR:
      this->ReceiveNotifyZoneServer(reinterpret_cast<const NotifyZoneServerPacket*>(g_gameModePacketBuffer));
      break;
    case ID_ZC_ACCEPT_ENTER:
      this->Zc_Accept_Enter2(reinterpret_cast<const AcceptEnterPositionPacket*>(g_gameModePacketBuffer));
      return;
    case ID_ZC_REFUSE_ENTER:
      g_windowMgr.Confirm(LookupMessage(MSI_ACCESS_DENIED), 0, true, false, 0, "Message", 280, 120, 0);
      GetRagConnection().Close();
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
      break;
    case ID_SC_NOTIFY_BAN:
      this->ReceiveNotifyBan(g_gameModePacketBuffer);
      break;
    case ID_SC_BILLING_INFO:
      this->ReceiveBillingInfo(g_gameModePacketBuffer);
      break;
    case ID_AC_ASK_PNGAMEROOM:
      if (g_windowMgr.Confirm(LookupMessage(MSI_ASK_PNGAMEROOM), 2, true, false, 0, "Message", 280, 120, 0) == 0xBB) {
        GameRoomReplyPacket reply;
        reply.id = ID_CA_REPLY_PNGAMEROOM;
        reply.permission = 1;
        GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(reply.id), &reply);
      } else {
        GetRagConnection().Close();
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
      break;
    case ID_SC_ACK_ENCRYPTION: {
      constexpr std::array<std::uint8_t, 25> source = CRYPT_NAME("No Packet Encryption !!!", 0xB195637DAFCB0D59ULL);
      thread_local EncryptedName<sizeof(source), 0xB195637DAFCB0D59ULL> text(source);
      ShowErrorDialog(GetRenderSystemHolder()->system, text.Text(), 0);
      break;
    }
    case ID_AC_NOTIFY_ERROR: {
      const LoginMessagePacket* packet = reinterpret_cast<const LoginMessagePacket*>(g_gameModePacketBuffer);
      const std::string message(packet->text, reinterpret_cast<const char*>(packet) + packet->length);
      g_windowMgr.Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0);
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
      break;
    }
    case ID_HC_BLOCK_CHARACTER: {
      const BlockedCharacterPacket* packet = reinterpret_cast<const BlockedCharacterPacket*>(g_gameModePacketBuffer);
      BlockedCharacter* blocked = reinterpret_cast<BlockedCharacter*>(&g_session.characterSelectionData[0x40]);
      g_session.blockedCharacterCount = (packet->length - offsetof(BlockedCharacterPacket, characters)) / sizeof(BlockedCharacter);
      memset(blocked, 0, g_session.blockedCharacterCount * sizeof(BlockedCharacter));
      memcpy(blocked, packet->characters, g_session.blockedCharacterCount * sizeof(BlockedCharacter));
      break;
    }
    case ID_AC_EVENT_RESULT:
      g_windowMgr.Confirm(LookupMessage(MSI_EVENT_RESULT), 0, true, false, 0, "Message", 280, 120, 0);
      break;
    case ID_HC_CHARNOTBEENSELECTED: {
      const AcceptEnterNeoUnionPacket* packet = reinterpret_cast<const AcceptEnterNeoUnionPacket*>(g_gameModePacketBuffer);
      this->slotExtension = packet->extension;
      this->slotExtension.code = htonl(this->slotExtension.code);
      this->slotExtension.time1 = htonl(this->slotExtension.time1);
      this->slotExtension.time2 = htonl(this->slotExtension.time2);

      const unsigned int dataLength = packet->length - offsetof(AcceptEnterNeoUnionPacket, characters);
      this->characterCount = dataLength / sizeof(CharacterInfo);
      g_session.characterCount = this->characterCount;
      const unsigned char* characters = reinterpret_cast<const unsigned char*>(packet->characters) + dataLength % sizeof(CharacterInfo);
      memcpy_s(this->characters, sizeof(this->characters), characters, this->characterCount * sizeof(CharacterInfo));
      this->pendingCursorType = 19;
      break;
    }
    case ID_HC_ACK_IS_VALID_CHARNAME: {
      const ShortResultPacket* packet = reinterpret_cast<const ShortResultPacket*>(g_gameModePacketBuffer);
      if (packet->result != 0) {
        char text[300];
        sprintf(text, LookupMessage(MSI_CONFIRM_TO_CHANGE_NAME), reinterpret_cast<const char*>(&g_session.characterRenameData[5]));
        if (g_windowMgr.Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
          ChangeCharacterNamePacket request;
          request.id = ID_CH_REQ_CHANGE_CHARNAME;
          request.gid = *reinterpret_cast<const std::uint32_t*>(&g_session.characterRenameData[1]);
          GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(request.id), &request);
          break;
        }
      } else if (g_windowMgr.Confirm(LookupMessage(MSI_CHARACTER_NAME_ALREADY_EXISTS), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
        break;
      }

      if (UIWindow* window = g_windowMgr.newSelectCharWnd) {
        window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME, 0, 0, 0);
      }
      break;
    }
    case ID_HC_ACK_CHANGE_CHARNAME:
      this->ReceiveCharacterResult(reinterpret_cast<const CharacterResultPacket*>(g_gameModePacketBuffer));
      break;
    case ID_HC_REFUSE_SELECTCHAR:
      switch (reinterpret_cast<const ResultPacket*>(g_gameModePacketBuffer)->result) {
      case 0:
        g_windowMgr.Confirm(LookupMessage(MSI_FR_INVALID_SLOT), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 1:
        g_windowMgr.Confirm(LookupMessage(MSI_FR_BASELVL), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 2:
        g_windowMgr.Confirm(LookupMessage(MSI_FR_INVALID_JOBLV), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 3:
        g_windowMgr.Confirm(LookupMessage(MSI_FR_JOB), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 4:
        g_windowMgr.Confirm(LookupMessage(MSI_FR_MAP), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 5:
        g_windowMgr.Confirm(LookupMessage(MSI_FR_INVALID_MONEY), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      this->pendingCursorType = GetRagConnection().NativeSocket() != INVALID_SOCKET ? 7 : 3;
      break;
    case ID_ZC_ACCEPT_ENTER2:
      this->Zc_Accept_Enter3(reinterpret_cast<const AcceptEnterPositionPacket*>(g_gameModePacketBuffer));
      return;
    case ID_HC_CHARACTER_LIST:
      this->ReceiveCharacterList(reinterpret_cast<const CharacterListPacket*>(g_gameModePacketBuffer));
      break;
    case ID_HC_DELETE_CHAR3_RESERVED:
      this->ReceiveDeleteReservation(reinterpret_cast<const CharacterDeletePacket*>(g_gameModePacketBuffer));
      break;
    case ID_HC_DELETE_CHAR3:
      this->ReceiveDeleteCharacter(reinterpret_cast<const CharacterDeletePacket*>(g_gameModePacketBuffer));
      break;
    case ID_HC_DELETE_CHAR3_CANCEL:
      this->ReceiveDeleteCancel(reinterpret_cast<const CharacterDeletePacket*>(g_gameModePacketBuffer));
      break;
    case ID_HC_ACCEPT_ENTER2:
      this->ReceiveCharacterSlots(reinterpret_cast<const CharacterSlotsPacket*>(g_gameModePacketBuffer));
      break;
    case ID_AC_REFUSE_LOGIN_SSO: {
      LoginErrorCodePacket* packet = reinterpret_cast<LoginErrorCodePacket*>(g_gameModePacketBuffer);
      packet->text[19] = '\0';
      this->ShowLoginError(packet->code, reinterpret_cast<int>(packet->text));
      break;
    }
    case ID_HC_NOTIFY_ACCESSIBLE_MAPNAME:
      this->ReceiveMapStates(reinterpret_cast<const MapStatePacket*>(g_gameModePacketBuffer));
      break;
    case ID_ZC_GPK_DYNCODE:
      IgnoreRemovedPacket(g_gameModePacketBuffer);
      break;
    case ID_HC_SECOND_PASSWD_LOGIN:
      this->SubD911C0(reinterpret_cast<const PasswordModePacket*>(g_gameModePacketBuffer));
      break;
    case ID_HC_MAKE_SECOND_PASSWD: {
      const SecondPasswordResultPacket* packet = reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer);
      g_session.m_loginAccountId = packet->accountId;
      switch (packet->result) {
      case 0:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDEDITWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_MAKE_SUCCESS), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 1:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDEDITWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_MAKE_DUPLICATED_PW), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 2:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDEDITWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_RESTRICT_PW), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 3:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDEDITWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_PERSONALNUM_PW), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 4:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDEDITWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_MAKE_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    }
    case ID_HC_DELETE_SECOND_PASSWD: {
      const SecondPasswordResultPacket* packet = reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer);
      g_session.m_loginAccountId = packet->accountId;
      switch (packet->result) {
      case 0:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDEDITWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_DEL_SUCCESS), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 1:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDEDITWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_DEL_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    }
    case ID_HC_EDIT_SECOND_PASSWD: {
      const SecondPasswordResultPacket* packet = reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer);
      g_session.m_loginAccountId = packet->accountId;
      switch (packet->result) {
      case 0:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_SUCCESS), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 1:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 2:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_RESTRICT_PW), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 3:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWORD_EDIT_PERSONALNUM_PW), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    }
    case ID_HC_NOT_AVAILABLE_SECOND_PASSWD: {
      const SecondPasswordResultPacket* packet = reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer);
      g_session.m_loginAccountId = packet->accountId;
      switch (packet->result) {
      case 0:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWDRD_NOT_AVAILABLE_SUCCESS), 0, true, false, 0, "Message", 280, 120, 0);
        static_cast<UINewSelectCharWnd*>(g_windowMgr.newSelectCharWnd)->EnableButtonById(0x1A2);
        g_session.m_passwordWndMode = 1;
        break;
      case 1:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWDRD_NOT_AVAILABLE_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    }
    case ID_HC_AVAILABLE_SECOND_PASSWD:
      switch (reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer)->result) {
      case 0:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWDRD_AVAILABLE_SUCCESS), 0, true, false, 0, "Message", 280, 120, 0);
        static_cast<UINewSelectCharWnd*>(g_windowMgr.newSelectCharWnd)->DisableButtonById(0x1A2);
        break;
      case 1:
        g_windowMgr.CloseWindow(WID_SECONDPASSWORDINPUTWND);
        g_windowMgr.Confirm(LookupMessage(MSI_SECOND_PASSWDRD_AVAILABLE_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    case ID_HC_ACK_CHANGE_CHARACTER_SLOT:
    case ID_HC_ACK_CHANGE_CHARACTER_SLOT2:
      if (reinterpret_cast<const SecondPasswordStatePacket*>(g_gameModePacketBuffer)->result == 1) {
        g_windowMgr.Confirm(LookupMessage(MSI_CHANGE_CHARACTER_SLOT_FAILED), 0, true, false, 0, "Message", 280, 120, 0);
      }
      break;
    case ID_CH_ACK_CHANGE_CHARACTERNAME:
      this->SubD8D500(reinterpret_cast<const char*>(g_gameModePacketBuffer));
      break;
    case ID_AC_SHUTDOWN_NOTIFY: {
      const ShutdownNoticePacket* packet = reinterpret_cast<const ShutdownNoticePacket*>(g_gameModePacketBuffer);
      __time32_t endTime = packet->endTime;
      const __time32_t startTime = packet->startTime;
      const tm when = *_localtime32(&endTime);
      char text[0x400];
      memset(text, 0, sizeof(text));
      _snprintf(text, sizeof(text), LookupMessage(MSI_MENU_MACROWND), when.tm_year + 1900, when.tm_mon + 1, when.tm_mday, when.tm_hour, when.tm_min, when.tm_sec);
      g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
      const std::uint64_t timestamp = static_cast<std::int64_t>(startTime) * 10000000;
      const std::int64_t interval = 3600000;
      CPartialShutDownSchedule* schedule = new CPartialShutDownSchedule(timestamp, interval, 1);
      schedule->SetPeriod(endTime, startTime);
      g_timeScheduler->AddSchedule(schedule);
      break;
    }
    case ID_AC_REFUSE_LOGIN3: {
      const LoginErrorTimePacket* packet = reinterpret_cast<const LoginErrorTimePacket*>(g_gameModePacketBuffer);
      if (packet->code != 0x12) {
        GetRagConnection().Close();
      }
      this->remainingLoginSeconds = packet->remaining;
      this->ShowLoginError(packet->code, reinterpret_cast<int>(""));
      break;
    }
    case ID_AC_REQ_MOBILE_OTP:
      this->authChallengeId = reinterpret_cast<const IntValuePacket*>(g_gameModePacketBuffer)->value;
      this->pendingCursorType = 24;
      break;
    case ID_HC_CHARLIST_NOTIFY:
      for (int i = 1; i <= reinterpret_cast<const IntValuePacket*>(g_gameModePacketBuffer)->value; ++i) {
        std::int16_t request = 0x9A1;
        GetRagConnection().Send(sizeof(request), &request);
      }
      break;
    case ID_TC_RESULT:
      this->ReceiveLoginError(reinterpret_cast<const LoginErrorPacket*>(g_gameModePacketBuffer));
      break;
    case ID_AC_LOGIN_TAREN_ACCEPT:
      this->ReceiveAcceptLogin(reinterpret_cast<const AcceptLoginPacket*>(g_gameModePacketBuffer), 4);
      break;
    case ID_AC_LOGIN_TAREN_REFUSE2:
      this->ReceiveRefuseLogin(reinterpret_cast<const RefuseLoginPacket*>(g_gameModePacketBuffer));
      break;
    case ID_AC_LOGIN_TAREN_REFUSE: {
      const ResultPacket* packet = reinterpret_cast<const ResultPacket*>(g_gameModePacketBuffer);
      int message = 0xC82;
      switch (packet->result) {
      case 1:
        message = 0xC7C;
        break;
      case 2:
        message = 0xC7D;
        break;
      case 3:
        message = 0xC7E;
        break;
      case 4:
        message = 0xC7F;
        break;
      case 5:
        message = 0xC80;
        break;
      case 6:
        message = 0xC81;
        break;
      case 7:
        message = 0xC8E;
        break;
      case 9:
        message = 0xF47;
        break;
      }
      GetRagConnection().Close();
      const int code = packet->result;
      const char* text = LookupMessage(message);
      const std::array<std::uint8_t, 7> source = CRYPT_BYTES(7, "%s(%d)", 0xDD3913D5D7F97BE1ULL);
      thread_local EncryptedName<sizeof(source), 0xDD3913D5D7F97BE1ULL> format(source);
      g_windowMgr.Confirm(FormatString(format.Text(), text, code), 0, true, false, 0, "Message", 280, 120, 0);
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
      break;
    }
    case ID_AC_REFUSE_LOGIN_EX: {
      const LoginErrorCodePacket* packet = reinterpret_cast<const LoginErrorCodePacket*>(g_gameModePacketBuffer);
      const std::string text(packet->text, sizeof(packet->text));
      this->ShowLoginError(packet->code, reinterpret_cast<int>(text.c_str()));
      break;
    }
    case ID_HC_NOTIFY_ZONESVR_DOMAIN:
      this->ReceiveNotifyZoneServer2(reinterpret_cast<const NotifyZoneServer2Packet*>(g_gameModePacketBuffer));
      break;
    case ID_AC_ACCEPT_LOGIN_DOMAIN:
      this->ReceiveAcceptLogin(reinterpret_cast<const AcceptLoginPacket*>(g_gameModePacketBuffer), 5);
      break;
    case ID_AC_ACCEPT_LOGIN_VIETNAM:
      this->ReceiveAcceptLogin(reinterpret_cast<const AcceptLoginPacket*>(g_gameModePacketBuffer), 6);
      break;
    case ID_ZC_RECOVER_PENALTY_OVERWEIGHT: {
      const int value = reinterpret_cast<const IntValuePacket*>(g_gameModePacketBuffer)->value;
      if (value > 0) {
        g_overweightThresholdPercent = value;
      }
      break;
    }
    case ID_TC_RESULT2:
      this->ReceiveLoginErrorDetail(reinterpret_cast<const LoginErrorDetailPacket*>(g_gameModePacketBuffer));
      break;
    case ID_HC_SECOND_PASSWD_LOGIN2:
      this->SubD90C90(reinterpret_cast<const PasswordModePacket*>(g_gameModePacketBuffer));
      break;
    case ID_ZC_EXTEND_BODYITEM_SIZE: {
      const std::int16_t slots = reinterpret_cast<const ShortResultPacket*>(g_gameModePacketBuffer)->result;
      if (slots <= 100) {
        g_session.extraInventorySlots = slots;
      }
      break;
    }
    case ID_HC_ACCEPT_MAKECHAR2: {
      const CharacterInfoPacket* packet = reinterpret_cast<const CharacterInfoPacket*>(g_gameModePacketBuffer);
      g_charInfo = packet->info;
      g_selectedSlotIndex = static_cast<unsigned char>(g_charInfo.slot);
      memcpy_s(&this->characters[this->characterCount], sizeof(CharacterInfo), &packet->info, sizeof(CharacterInfo));
      char* mapName = this->characters[this->characterCount].mapName;
      StringFormat(mapName, sizeof(this->characters[this->characterCount].mapName), static_cast<std::size_t>(-1), "%s", g_session.GatName2RswName(mapName));
      ++this->characterCount;
      ++g_session.characterSlotState;
      this->pendingCursorType = 7;
      break;
    }
    case 0xC05: {
      const CharacterSlotCountsPacket* packet = reinterpret_cast<const CharacterSlotCountsPacket*>(g_gameModePacketBuffer);
      g_session.restrictedCharacterSlots = packet->counts[1];
      CharacterSlotsPacket slots;
      slots.id = ID_HC_ACCEPT_ENTER2;
      slots.length = sizeof(CharacterSlotsPacket);
      slots.normalSlots = packet->counts[0] + packet->counts[1];
      slots.premiumSlots = packet->counts[2];
      slots.billingSlots = packet->counts[3];
      slots.producibleSlots = packet->counts[4];
      slots.validSlots = packet->counts[5];
      slots.extension = packet->extension;
      this->ReceiveCharacterSlots(&slots);
      break;
    }
    case ID_HC_UPDATE_CHARINFO2: {
      const CharacterInfoPacket* packet = reinterpret_cast<const CharacterInfoPacket*>(g_gameModePacketBuffer);
      CharacterInfo* character = reinterpret_cast<CharacterInfo*>(g_modeMgr.gameMode->SendMsg(MM_QUERYCHARICTORINFO, packet->info.slot, 0, 0, 0));
      if (character && character->gid == *reinterpret_cast<const std::uint32_t*>(&g_session.characterRenameData[1])) {
        *character = packet->info;
        StringFormat(character->mapName, sizeof(character->mapName), static_cast<std::size_t>(-1), "%s", g_session.GatName2RswName(character->mapName));
        this->pendingCursorType = 7;
      }
      break;
    }
    case ID_HC_ACK_CHARINFO_PER_PAGE2:
      this->ReceiveCharacterPage(reinterpret_cast<const CharacterPagePacket*>(g_gameModePacketBuffer));
      break;
    default:
      GetRagConnection().ResetQueue();
      break;
    }
  }
}

// OFFSET: 0x00D9B990
// FUNCTION: fn_22313
int CLoginMode::SendMsg(int event, int argument1, int argument2, int argument3, int argument4) {
  (void)argument4;

  switch (event) {
  case MM_SOCKETERROR:
    GetRagConnection().Close();
    if (g_windowMgr.Confirm(LookupMessage(MSI_UNABLE_TO_CONNECT_SERVER), 0, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
    }
    return 0;
  case LMM_RESET_TO_LOGIN:
    g_charServerAccountIdPending = 0;
    this->pendingCursorType = 3;
    return 0;
  case LMM_SEND_CHARACTER_AUTH_CODE: {
    unsigned char* buffer = static_cast<unsigned char*>(
      _MallocaComputeSize(0x14) != 0
        ? (_MallocaComputeSize(0x14) <= _ALLOCA_S_THRESHOLD
            ? _MarkAllocaS(_alloca(_MallocaComputeSize(0x14)), _ALLOCA_S_STACK_MARKER)
            : _MarkAllocaS(std::malloc(_MallocaComputeSize(0x14)), _ALLOCA_S_HEAP_MARKER))
        : nullptr
    );
    if (buffer) {
      *reinterpret_cast<std::int16_t*>(buffer) = 0x9A3;
      *reinterpret_cast<int*>(buffer + 4) = this->authChallengeId;
      StringFormat(reinterpret_cast<char*>(buffer + 8), 10, static_cast<std::size_t>(-1), reinterpret_cast<const char*>(&this->loginCredentials[0x80]));
      *reinterpret_cast<std::int16_t*>(buffer + 2) = 14;
      GetRagConnection().Send(*reinterpret_cast<std::int16_t*>(buffer + 2), buffer);
      this->waitWnd = g_windowMgr.EnsureWindow(WID_WAITWND);
      static_cast<UIWaitWnd*>(this->waitWnd)->SetMessage(LookupMessage(MSI_WAITING_RESPONSE_FROM_SERVER), 16, 1);
      _freea(buffer);
    }
    return 0;
  }
  case MM_QUERYCHARICTORINFO:
    for (int i = 0; i < this->characterCount; ++i) {
      if (this->characters[i].slot == argument1) {
        return reinterpret_cast<int>(&this->characters[i]);
      }
    }
    return 0;
  case MM_COMMAND:
    switch (argument1) {
    case LMM_SELECTCHARACTER:
      this->pendingCursorType = 7;
      return 0;
    case LMM_DELETE_CHARACTER:
      this->pendingCursorType = 11;
      return 0;
    case LMM_MAKECHARACTER:
      this->pendingCursorType = 8;
      return 0;
    case LMM_TRANSFERCHARACTER:
      this->pendingCursorType = 9;
      return 0;
    case LMM_CANCEL:
      if (this->cursorType == 7) {
        g_windowMgr.Shutdown();
        GetRagConnection().Close();
        this->pendingCursorType = 6;
      }
      return 0;
    default:
      return 0;
    }
  case LMM_DISPATCH_CHARACTER_SLOT_ACTION:
    switch (this->noticeCommands[argument1]) {
    case 45:
      if (UIWindow* window = g_windowMgr.LookupWindow(WID_NEWSELECTCHARWND)) {
        window->SendMsg(nullptr, UM_COMMAND, 0x1B0, 0, 0, 0);
      }
      return 0;
    case 46:
      if (UIWindow* window = g_windowMgr.LookupWindow(WID_NEWSELECTCHARWND)) {
        window->SendMsg(nullptr, UM_COMMAND, 0x1B1, 0, 0, 0);
      }
      return 0;
    case 47:
      if (UIWindow* window = g_windowMgr.LookupWindow(WID_NEWSELECTCHARWND)) {
        window->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_NAME, 0, 0, 0);
      }
      return 0;
    default:
      return 0;
    }
  case LMM_QUERY_LAST_CHARACTER_INFO:
    if (this->characterCount == 0) {
      return 0;
    }
    return reinterpret_cast<int>(&this->characters[this->characterCount - 1]);
  case LMM_SEND_LOGIN_AUTH_CODE: {
    struct PacketAD0 {
      /* 0000 */ std::int16_t id;
      /* 0002 */ char code[9];
    };
    PacketAD0 packet;
    packet.id = ID_CT_MOTP;
    memcpy_s(packet.code, sizeof(packet.code), &this->loginCredentials[0x80], sizeof(packet.code));
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    this->waitWnd = g_windowMgr.EnsureWindow(WID_WAITWND);
    static_cast<UIWaitWnd*>(this->waitWnd)->SetMessage(LookupMessage(MSI_WAITING_RESPONSE_FROM_SERVER), 16, 1);
    return 0;
  }
  case LMM_CONNECT_TO_ACSVR:
    this->pendingCursorType = 4;
    return 0;
  case LMM_SET_STATE_GET_EMAIL_ADDRESS:
    this->pendingCursorType = argument1;
  case LMM_GET_EMAIL_ADDRESS:
    return reinterpret_cast<int>(this->billingInfo);
  case LMM_SET_EMAIL_ADDRESS:
    g_windowMgr.InvalidateAll();
    strcpy(reinterpret_cast<char*>(this->billingInfo), reinterpret_cast<const char*>(argument1));
    return 0;
  case LMM_GET_MAKING_CHAR_NAME:
    return reinterpret_cast<int>(&this->makingCharNameFirstByte);
  case LMM_SET_MAKING_CHAR_NAME:
    strcpy(reinterpret_cast<char*>(&this->makingCharNameFirstByte), reinterpret_cast<const char*>(argument1));
    return 0;
  case LMM_GET_CHAR_PARAM:
    return reinterpret_cast<int>(this->charParams);
  case LMM_SET_CHAR_PARAM: {
    const int* values = reinterpret_cast<const int*>(argument1);
    this->charParams[0] = values[0];
    this->charParams[1] = values[1];
    this->charParams[2] = values[2];
    this->charParams[3] = values[3];
    this->charParams[4] = values[4];
    this->charParams[5] = values[5];
    this->characterParameter7 = values[6];
    this->characterParameter8 = values[7];
    return 0;
  }
  case LMM_PASSWORD:
    strcpy(reinterpret_cast<char*>(this->loginCredentials), reinterpret_cast<const char*>(argument1));
    return 0;
  case LMM_ID:
    strcpy(reinterpret_cast<char*>(&this->loginCredentials[0x40]), reinterpret_cast<const char*>(argument1));
    strcpy(reinterpret_cast<char*>(g_session.characterSelectionData), reinterpret_cast<const char*>(argument1));
    return 0;
  case LMM_SET_AUTH_CODE:
    strcpy(reinterpret_cast<char*>(&this->loginCredentials[0x80]), reinterpret_cast<const char*>(argument1));
    return 0;
  case LMM_SELECT_CHARACTER: {
    g_session.selectedCharacterSlot = static_cast<unsigned char>(g_selectedSlotIndex);
    g_windowMgr.LoadShortcutItems();
    for (int i = 0; i < this->characterCount; ++i) {
      if (this->characters[i].slot == static_cast<unsigned char>(g_selectedSlotIndex)) {
        *reinterpret_cast<int*>(&g_session.characterLoginData[0x00]) = this->characters[i].slot;
        g_session.SetJob(this->characters[i].job);
        g_session.m_gid = this->characters[i].gid;
        *reinterpret_cast<int*>(g_session.healthPoints) = this->characters[i].speed;
        g_session.InvalidateWindowFrames();
        g_session.m_level = this->characters[i].level;
        g_session.InvalidateWindowFrames();
        g_session.m_skillPoint = this->characters[i].skillPoint;
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<int*>(g_session.statusPoints) = this->characters[i].jobPoint;
        g_session.InvalidateWindowFrames();
        g_session.m_standardStats[0] = this->characters[i].stats[0];
        g_session.InvalidateWindowFrames();
        g_session.m_standardStats[1] = this->characters[i].stats[1];
        g_session.InvalidateWindowFrames();
        g_session.m_standardStats[2] = this->characters[i].stats[2];
        g_session.InvalidateWindowFrames();
        g_session.m_standardStats[3] = this->characters[i].stats[3];
        g_session.InvalidateWindowFrames();
        g_session.m_standardStats[4] = this->characters[i].stats[4];
        g_session.InvalidateWindowFrames();
        g_session.m_standardStats[5] = this->characters[i].stats[5];
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x04]) = this->characters[i].exp;
        g_session.InvalidateWindowFrames();
        const int hp = static_cast<int>(this->characters[i].hp);
        if (g_session.hp != hp) {
          g_session.hp = hp;
          g_session.RefreshHpSpWindows();
        }
        const int maxHp = static_cast<int>(this->characters[i].maxHp);
        if (g_session.maxHp != maxHp) {
          g_session.maxHp = maxHp;
          g_session.RefreshHpSpWindows();
        }
        const int sp = static_cast<int>(this->characters[i].sp);
        if (*reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0x00]) != sp) {
          *reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0x00]) = sp;
          g_session.RefreshHpSpWindows();
        }
        const int maxSp = static_cast<int>(this->characters[i].maxSp);
        if (*reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0x04]) != maxSp) {
          *reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0x04]) = maxSp;
          g_session.RefreshHpSpWindows();
        }
        g_session.m_zeny = *reinterpret_cast<const int*>(&this->characters[i].progressionData[0x00]);
        g_session.RefreshHpSpWindows();
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x1C]) = *reinterpret_cast<const std::int64_t*>(&this->characters[i].progressionData[0x04]);
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<int*>(g_session.optionFlags) = *reinterpret_cast<const int*>(&this->characters[i].progressionData[0x1C]);
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<int*>(&g_session.healthPoints[0x04]) = *reinterpret_cast<const int*>(&this->characters[i].progressionData[0x20]);
        g_session.InvalidateWindowFrames();
        g_session.m_jobLevel = *reinterpret_cast<const int*>(&this->characters[i].progressionData[0x0C]);
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<int*>(&g_session.characterLoginData[0x30]) = this->characters[i].characterFlags[0];
        g_session.m_head = this->characters[i].head;
        *reinterpret_cast<int*>(&g_session.weaponAppearance[0x04]) = 0;
        *reinterpret_cast<int*>(&g_session.weaponAppearance[0x08]) = 0;
        *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x00]) = this->characters[i].accessory;
        *reinterpret_cast<int*>(&g_session.weaponAppearance[0x0C]) = this->characters[i].shield;
        g_session.UpdateBodyStyle(this->characters[i].job, this->characters[i].bodyStyle, this->characters[i].bodyCostume);
        g_session.m_headPalette = this->characters[i].headPalette;
        *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x04]) = this->characters[i].accessory2;
        *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x08]) = this->characters[i].accessory3;
        g_session.m_robe = this->characters[i].robe;
        g_session.m_bodyCostume = this->characters[i].bodyCostume;
        *reinterpret_cast<int*>(&g_session.statusLevelEffects[0x00]) = *reinterpret_cast<const int*>(&this->characters[i].progressionData[0x10]);
        *reinterpret_cast<int*>(&g_session.statusLevelEffects[0x04]) = *reinterpret_cast<const int*>(&this->characters[i].progressionData[0x14]);
        g_session.m_jobFlags = *reinterpret_cast<const std::uint32_t*>(&this->characters[i].progressionData[0x18]);
        return 0;
      }
    }
    return 0;
  }
  case LMM_GOTOSELECTACCOUNT:
    if (argument2 == 201) {
      g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
      return 0;
    }
    this->pendingCursorType = 2;
    return 0;
  case LMM_SELECTACCOUNT:
    this->selectedAccount = argument1;
    *reinterpret_cast<int*>(&g_session.loginOptions[4]) = argument1;
    TraceSelection(argument1);
    if (g_serviceType == 19) {
      std::string suffix;
      if (argument1 == 1) {
        g_tipOfTheDayEnglish = true;
        g_serverLanguage->alternateLanguage = static_cast<std::uint16_t>(argument1);
        constexpr std::array<std::uint8_t, 5> prefixSource = CRYPT_NAME("uae\\", 0x23BBD58F8B5FF5C9ULL);
        thread_local EncryptedName<sizeof(prefixSource), 0x23BBD58F8B5FF5C9ULL> prefix(prefixSource);
        g_serverLanguage->SetServicePrefix(prefix.Text());
        const std::array<std::uint8_t, 5> suffixSource = CRYPT_BYTES(5, "uae\\", 0x0B7109397F6B1387ULL);
        thread_local EncryptedName<sizeof(suffixSource), 0x0B7109397F6B1387ULL> suffixName(suffixSource);
        suffix = suffixName.Text();
      } else {
        g_tipOfTheDayEnglish = false;
        g_serverLanguage->alternateLanguage = 9;
        const std::array<std::uint8_t, 9> prefixSource = CRYPT_BYTES(9, "english\\", 0x03772763F96D9DA5ULL);
        thread_local EncryptedName<sizeof(prefixSource), 0x03772763F96D9DA5ULL> prefix(prefixSource);
        g_serverLanguage->SetServicePrefix(prefix.Text());
        const std::array<std::uint8_t, 9> suffixSource = CRYPT_BYTES(9, "english\\", 0xEDC74F6D13DF71EFULL);
        thread_local EncryptedName<sizeof(suffixSource), 0xEDC74F6D13DF71EFULL> suffixName(suffixSource);
        suffix = suffixName.Text();
      }
      g_session.ReloadLanguageTables();
      g_session.IgnoreText(suffix);
    }
    this->pendingCursorType = (g_serviceType == 5 || g_serviceType == 7) ? 13 : 3;
    return 0;
  case LMM_SELECTACCOUNT2:
    this->selectedSecondaryAccount = argument1;
    this->accountInfo.clear();
    TraceSelectionRange(this->selectedAccount, this->selectedSecondaryAccount);
    this->pendingCursorType = 3;
    return 0;
  case LMM_GOTO_LOGIN:
    GetRagConnection().Close();
    g_charServerAccountIdPending = 0;
    this->pendingCursorType = 3;
    return 0;
  case LMM_SET_LOGIN_EXTRA:
    strcpy(reinterpret_cast<char*>(this->loginExtra), reinterpret_cast<const char*>(argument1));
    return 0;
  case LMM_DISCONNECT_ACSVR:
    GetRagConnection().Close();
    if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
      this->otpStarted = timeGetTime();
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
    }
    return 0;
  case LMM_SELECTSVR: {
    *reinterpret_cast<int*>(this->selectedServerStorage) = argument1;
    if (argument1 == -1) {
      return 0;
    }

    ServerAddress2* servers = reinterpret_cast<ServerAddress2*>(this->serverList);
    if (servers[argument1].address.state == 3) {
      g_windowMgr.Confirm(LookupMessage(MSI_CANT_CONNECT_TO_PAY_SERVER), 0, true, false, 0, "Message", 280, 120, 0);
      this->pendingCursorType = 6;
      return 0;
    }

    g_session.m_accountName = servers[argument1].address.name;
    const int selected = *reinterpret_cast<int*>(this->selectedServerStorage);
    const std::uint32_t ip = servers[selected].address.ip;
    const std::array<std::uint8_t, 12> ipFormatSource = CRYPT_BYTES(12, "%d.%d.%d.%d", 0xF161095D0DE18F95ULL);
    thread_local EncryptedName<sizeof(ipFormatSource), 0xF161095D0DE18F95ULL> ipFormat(ipFormatSource);
    CharServerEndpoint endpoint;
    wsprintfA(endpoint.ip, ipFormat.Text(), ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, ip >> 24);
    const int selected2 = *reinterpret_cast<int*>(this->selectedServerStorage);
    endpoint.port = servers[selected2].address.port;
    const std::array<std::uint8_t, 3> hostFormatSource = CRYPT_BYTES(3, "%s", 0x2BAD156DE3E1F9D9ULL);
    thread_local EncryptedName<sizeof(hostFormatSource), 0x2BAD156DE3E1F9D9ULL> hostFormat(hostFormatSource);
    StringFormat(endpoint.host, sizeof(endpoint.host), static_cast<std::size_t>(-1), hostFormat.Text(), servers[selected2].domain);
    g_charServerEndpoint = endpoint;
    g_charServerIp = servers[*reinterpret_cast<int*>(this->selectedServerStorage)].address.ip;
    g_session.m_serverName = servers[*reinterpret_cast<int*>(this->selectedServerStorage)].address.name;
    g_checkAttendanceMgr->LoadLocal(g_session.m_serverName);
    this->pendingCursorType = 5;
    return 0;
  }
  case LMM_SENDCHARINFO:
    this->pendingCursorType = 10;
    return 0;
  case LMM_CANCEL:
    if (this->cursorType == 6 || this->cursorType == 27) {
      GetRagConnection().Close();
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
      return 0;
    }
    if (this->cursorType == 30) {
      this->pendingCursorType = 7;
    }
    return 0;
  case LMM_GET_BILLING_INFO: {
    char* output = reinterpret_cast<char*>(argument1);
    switch (this->slotExtension.code) {
    case 100:
      std::sprintf(output, LookupMessage(MSI_BILLING_100), this->slotExtension.time1 / 60);
      return 0;
    case 200: {
      const std::uint32_t value = this->slotExtension.time1;
      std::sprintf(output, LookupMessage(MSI_BILLING_200), value / 3600, (value / 60) % 60, value % 60);
      return 0;
    }
    case 300:
      std::sprintf(output, LookupMessage(MSI_BILLING_300));
      return 0;
    case 400: {
      const std::uint32_t value = this->slotExtension.time2 - this->slotExtension.time1;
      std::sprintf(output, LookupMessage(MSI_BILLING_400), value / 3600, (value / 60) % 60, value % 60);
      return 0;
    }
    case 500: {
      const std::uint32_t value = this->slotExtension.time1 != 0 ? this->slotExtension.time1 : this->slotExtension.time2;
      const int messageId = this->slotExtension.time1 != 0 ? 0x219 : 0x21A;
      std::sprintf(output, LookupMessage(messageId), value / 3600, (value / 60) % 60, value % 60);
      return 0;
    }
    case 600:
      std::sprintf(output, LookupMessage(MSI_BILLING_600));
      return 0;
    case 700:
      std::sprintf(output, LookupMessage(MSI_BILLING_700));
      return 0;
    case 800:
      std::sprintf(output, LookupMessage(MSI_BILLING_800));
      return 0;
    default:
      std::sprintf(output, LookupMessage(MSI_CHARACTER_INFO));
      return 0;
    }
  }
  case LMM_SELECT_REPLAY_DATA:
    reinterpret_cast<unsigned char*>(&GetRagConnection())[0x6C] = 1;
    this->pendingCursorType = 27;
    return 0;
  case LMM_START_REPLAY:
    g_session.SetFlag(static_cast<char>(Replay::Instance().header.sessionSyncValue));
    this->pendingCursorType = 28;
    return 0;
  case LMM_REPLAY_SELECT_SERVER:
    reinterpret_cast<unsigned char*>(&GetRagConnection())[0x6C] = 0;
    g_session.m_aid = ReplayFileList::Instance().selectionState;
    g_session.SetFlag(static_cast<char>(ReplayFileList::Instance().dialogState));
    this->pendingCursorType = 6;
    return 0;
  case LMM_SELECT_MAP:
    if (argument1 != -1) {
      MapStateEntry entry;
      entry.index = this->mapStates[argument1].index;
      this->mapStates.clear();
      this->mapStates.push_back(entry);
      this->pendingCursorType = 31;
    }
    return 0;
  default:
    return CMode::SendMsg(event, argument1, argument2, argument3, 0);
  }
}

#pragma pack(push, 1)
struct ZoneLoginPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char code[4];
  /* 0006 */ char name[25];
  /* 001F */ char password[32];
  /* 003F */ char extra[5];
};

struct AccountLoginPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t version;
  /* 0006 */ char name[24];
  /* 001E */ char password[24];
  /* 0036 */ unsigned char clientType;
};

struct CharServerEnterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ int authCode;
  /* 000A */ int userLevel;
  /* 000E */ std::uint16_t clientType;
  /* 0010 */ unsigned char sex;
};

struct SelectCharacterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char slot;
};

struct MakeCharacterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
  /* 001A */ unsigned char slot;
  /* 001B */ std::int16_t headPalette;
  /* 001D */ std::int16_t head;
  /* 001F */ int job;
  /* 0023 */ unsigned char sex;
};

struct DeleteCharacterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t gid;
  /* 0006 */ char code[6];
};

struct DeleteCharacterEmailPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t gid;
  /* 0006 */ char code[50];
};

struct ZoneEnterPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint32_t gid;
  /* 000A */ int authCode;
  /* 000E */ unsigned char padding_000E[4];
  /* 0012 */ std::uint32_t clientTime;
  /* 0016 */ unsigned char sex;
};

struct SelectMapPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char slot;
  /* 0003 */ unsigned char map;
};
#pragma pack(pop)

static_assert(sizeof(ZoneLoginPacket) == 0x44);
static_assert(sizeof(AccountLoginPacket) == 0x37);
static_assert(sizeof(CharServerEnterPacket) == 0x11);
static_assert(sizeof(MakeCharacterPacket) == 0x24);
static_assert(sizeof(DeleteCharacterEmailPacket) == 0x38);
static_assert(sizeof(ZoneEnterPacket) == 0x17);

// OFFSET: 0x00D8C020
// FUNCTION: fn_22254
std::string nullsub_481(int, unsigned int) {
  return {};
}
void __stdcall LoadCustomServerList(CLoginMode* mode);

// GLOBAL: gl_25523
unsigned char g_customServerListLoaded;

// GLOBAL: gl_25475
unsigned char g_loginCancelRequested;

// GLOBAL: gl_25527
std::uint32_t g_patchedClientVersion = 0x80000000;

// INLINED
static char (&CustomServerBalloons())[8][128] {
  static char balloons[8][128];
  return balloons;
}

// OFFSET: 0x03DE91E0
// FUNCTION: fn_45403
__declspec(noinline) static const char* XmlText(const std::string& text, std::size_t& length) {
  length = text.size();
  return text.c_str();
}

// OFFSET: 0x03DE91EC
// FUNCTION: fn_45404
__declspec(noinline) static void CopyXmlText(const std::string* text, char* output, std::size_t capacity) {
  if (text == nullptr) {
    *output = '\0';
    return;
  }
  std::size_t length;
  const char* source = XmlText(*text, length);
  if (source == nullptr || capacity == 0) {
    *output = '\0';
    return;
  }
  if (length > capacity - 1) {
    length = capacity - 1;
  }
  std::memcpy(output, source, length);
  output[length] = '\0';
}

// OFFSET: 0x03DE9224
// FUNCTION: fn_45405
__declspec(noinline) static void CopyXmlTextWithoutColors(const std::string* text, char* output, std::size_t capacity) {
  if (text == nullptr) {
    *output = '\0';
    return;
  }
  std::size_t length;
  const char* source = XmlText(*text, length);
  if (source == nullptr || capacity == 0) {
    *output = '\0';
    return;
  }
  std::size_t remaining = capacity - 1;
  while (length != 0 && remaining > 0) {
    const char c = *source++;
    --length;
    if (c == '^' && length >= 6) {
      source += 6;
      length -= 6;
      continue;
    }
    *output++ = c;
    --remaining;
  }
  *output = '\0';
}

// OFFSET: 0x03DE927C
// FUNCTION: fn_45406
__declspec(noinline) static unsigned int ParseXmlNumber(const std::string* text) {
  if (text == nullptr) {
    return 0;
  }
  std::size_t length;
  const char* source = XmlText(*text, length);
  unsigned int value = 0;
  while (length != 0) {
    const unsigned int digit = static_cast<unsigned char>(*source++) - '0';
    --length;
    if (digit > 9) {
      break;
    }
    value = value * 10 + digit;
  }
  return value;
}

// OFFSET: 0x03DE92AD
// FUNCTION: fn_45407
__declspec(noinline) static std::uint32_t ParseXmlAddress(const std::string* text) {
  if (text == nullptr) {
    return 0;
  }
  std::size_t length;
  const char* source = XmlText(*text, length);
  if (source == nullptr || length == 0) {
    return 0;
  }
  std::uint32_t address = 0;
  unsigned int octet = 0;
  unsigned int dots = 0;
  while (length != 0) {
    const unsigned int c = static_cast<unsigned char>(*source++);
    --length;
    if (c == '.') {
      if (dots >= 3) {
        return 0;
      }
      address = (address << 8) | octet;
      octet = 0;
      ++dots;
      continue;
    }
    if (c - '0' > 9) {
      return 0;
    }
    octet = octet * 10 + (c - '0');
    if (octet > 0xFF) {
      return 0;
    }
  }
  if (dots != 3) {
    return 0;
  }
  return (address << 8) | octet;
}

// OFFSET: 0x03DE9318
// FUNCTION: fn_45408
void __stdcall LoadCustomServerList(CLoginMode* mode) {
  ServerAddress2* servers = reinterpret_cast<ServerAddress2*>(mode->serverList);
  mode->serverCount = 0;
  *reinterpret_cast<int*>(mode->selectedServerStorage) = -1;

  XmlNode* connection = g_clientInfoRoot->FindChild("connection");
  int count = 0;
  while (connection != nullptr && count < 8) {
    ServerAddress2& server = servers[count];
    std::memset(&server, 0, sizeof(server));
    std::memset(CustomServerBalloons()[count], 0, sizeof(CustomServerBalloons()[count]));

    const std::string* address = nullptr;
    const std::string* port = nullptr;
    const std::string* display = nullptr;
    const std::string* balloon = nullptr;

    if (XmlNode* node = connection->FindChild("address")) {
      address = &node->Text();
      if (address != nullptr) {
        server.address.ip = _byteswap_ulong(ParseXmlAddress(address));
      }
    }
    if (XmlNode* node = connection->FindChild("port")) {
      port = &node->Text();
      if (port != nullptr) {
        server.address.port = static_cast<std::int16_t>(ParseXmlNumber(port));
      }
    }
    if (XmlNode* node = connection->FindChild("display")) {
      display = &node->Text();
    }

    server.address.userCount = 4;
    server.address.state = 0;
    server.address.property = 0;
    CopyXmlTextWithoutColors(display != nullptr ? display : address, server.address.name, sizeof(server.address.name));
    CopyXmlText(display != nullptr ? display : address, server.domain, 0x78);

    if (XmlNode* node = connection->FindChild("balloon")) {
      balloon = &node->Text();
      if (balloon != nullptr) {
        CopyXmlText(balloon, CustomServerBalloons()[count], sizeof(CustomServerBalloons()[count]));
      }
    }

    ++count;
    connection = connection->FindSibling("connection");
  }

  mode->serverCount = count;
  if (count != 0) {
    *reinterpret_cast<int*>(mode->selectedServerStorage) = 0;
  }
}

// OFFSET: 0x00D93B20
// FUNCTION: fn_22297
void CLoginMode::OnChangeState(int value) {
  this->otpStarted = timeGetTime();
  g_windowMgr.Shutdown();
  this->tooltip = nullptr;
  g_session.quitBlocked = 0;
  this->connectionReady = 1;

  ServerAddress2* servers = reinterpret_cast<ServerAddress2*>(this->serverList);
  SessionAccount& account = *reinterpret_cast<SessionAccount*>(&g_session.m_sex);

  switch (value) {
  case 0:
  case 1:
  case 13:
    this->SendMsg(LMM_RESET_TO_LOGIN, 0, 0, 0, 0);
    break;
  case 38: {
    CharServerEndpoint endpoint;
    std::memset(&endpoint, 0, sizeof(endpoint));
    if (!std::strstr(g_session.clientVersion, "Dev")) {
      const int port = rand() % 3 + 6951;
      const std::array<std::uint8_t, 26> hostSource = CRYPT_BYTES(26, "kro-agency.ragnarok.co.kr", 0x7941D76335CFA14FULL);
      thread_local EncryptedName<sizeof(hostSource), 0x7941D76335CFA14FULL> host(hostSource);
      const char* hostText = host.Text();
      const std::array<std::uint8_t, 6> formatSource = CRYPT_BYTES(6, "%s:%d", 0x7321C3814F8799E1ULL);
      thread_local EncryptedName<sizeof(formatSource), 0x7321C3814F8799E1ULL> format(formatSource);
      StringFormat(endpoint.host, sizeof(endpoint.host), static_cast<std::size_t>(-1), format.Text(), hostText, port);
    } else {
      const std::array<std::uint8_t, 34> hostSource = CRYPT_BYTES(34, "kro-qm-agency.ragnarok.co.kr:6951", 0x7115D9F973F913C9ULL);
      thread_local EncryptedName<sizeof(hostSource), 0x7115D9F973F913C9ULL> host(hostSource);
      const char* hostText = host.Text();
      const std::array<std::uint8_t, 3> formatSource = CRYPT_BYTES(3, "%s", 0xCB1D253B7FE1F563ULL);
      thread_local EncryptedName<sizeof(formatSource), 0xCB1D253B7FE1F563ULL> format(formatSource);
      StringFormat(endpoint.host, sizeof(endpoint.host), static_cast<std::size_t>(-1), format.Text(), hostText);
    }

    this->connectionReady = GetRagConnection().Connect(*reinterpret_cast<const ConnectionEndpoint*>(&endpoint));
    if (!this->connectionReady) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 1, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        this->pendingCursorType = 3;
      }
      break;
    }

    ZoneLoginPacket packet;
    std::memset(&packet, 0, sizeof(packet));
    packet.id = ID_CT_AUTH;
    const std::array<std::uint8_t, 5> codeSource = CRYPT_BYTES(5, "0011", 0x793F370985ED674FULL);
    thread_local EncryptedName<sizeof(codeSource), 0x793F370985ED674FULL> code(codeSource);
    std::memcpy(packet.code, code.Text(), sizeof(packet.code));
    const std::array<std::uint8_t, 3> nameFormatSource = CRYPT_BYTES(3, "%s", 0x234977DDB54BC71DULL);
    thread_local EncryptedName<sizeof(nameFormatSource), 0x234977DDB54BC71DULL> nameFormat(nameFormatSource);
    StringFormat(packet.name, sizeof(packet.name), static_cast<std::size_t>(-1), nameFormat.Text(), reinterpret_cast<const char*>(&this->loginCredentials[0x40]));
    char password[33] = {};
    const std::array<std::uint8_t, 3> passwordFormatSource = CRYPT_BYTES(3, "%s", 0xEF6D1D893F2B2DC5ULL);
    thread_local EncryptedName<sizeof(passwordFormatSource), 0xEF6D1D893F2B2DC5ULL> passwordFormat(passwordFormatSource);
    StringFormat(password, sizeof(password), static_cast<std::size_t>(-1), passwordFormat.Text(), reinterpret_cast<const char*>(this->loginCredentials));
    DecryptRijndael256(password, packet.password);
    const std::array<std::uint8_t, 3> extraFormatSource = CRYPT_BYTES(3, "%s", 0x39A39F87DD5F535BULL);
    thread_local EncryptedName<sizeof(extraFormatSource), 0x39A39F87DD5F535BULL> extraFormat(extraFormatSource);
    StringFormat(packet.extra, sizeof(packet.extra), static_cast<std::size_t>(-1), extraFormat.Text(), reinterpret_cast<const char*>(this->loginExtra));
    GetRagConnection().Send(sizeof(packet), &packet);
    this->otpStarted = timeGetTime();
    this->waitWnd = g_windowMgr.EnsureWindow(WID_WAITWND);
    static_cast<UIWaitWnd*>(this->waitWnd)->SetMessage(LookupMessage(MSI_WAITING_RESPONSE_FROM_SERVER), 16, 1);
    break;
  }
  case 2:
  case 3:
    g_accountLinkedUserDataMgr->Reset(*g_session.m_hotkeyScript, g_serviceHost, g_servicePort, g_serviceOverrideKey);
    g_characterLinkedUserDataMgr->Reset(*g_session.m_hotkeyScript, g_serviceHost, g_servicePort, g_serviceOverrideKey);
    g_session.m_storeInfoBackup.Reset(*g_session.m_hotkeyScript, *g_session.m_hotkeyScript, g_serviceHost, g_servicePort, g_serviceOverrideKey);
    g_guildMgr->m_guilds.clear();
    g_emblemDataMgr->RefreshUrls(*g_session.m_hotkeyScript, *g_session.m_hotkeyScript, g_serviceHost, g_servicePort, g_serviceOverrideKey);
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    if (!g_session.characterRenameEnabled) {
      if (g_session.m_loginModeFlag == true) {
        this->otpStarted = timeGetTime();
        g_charServerAccountIdPending = 0;
        this->loginResponseReceived = false;
        this->otpPending = false;
        SecureZeroMemory(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText));
      }

      if (std::strcmp(g_session.m_loginErrorText, "") == 0) {
        g_session.m_loginModeFlag = true;
        if (!g_windowMgr.LookupWindow(WID_LOGINWND)) {
          g_windowMgr.EnsureWindow(WID_LOGINWND);
        }
        break;
      }

      if (g_session.m_launchAccountEnabled != true) {
        g_modeMgr.Quit();
        break;
      }
      g_session.m_launchAccountEnabled = false;
    }
    this->pendingCursorType = 4;
    break;
  case 6: {
    Replay::Instance().SetState(1);
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    UISelectServerWnd* window = static_cast<UISelectServerWnd*>(g_windowMgr.EnsureWindow(WID_SELECTSERVERWND));
    const std::array<std::uint8_t, 10> countNameSource = CRYPT_BYTES(10, "NUMSERVER", 0x6FE153231D879777ULL);
    thread_local EncryptedName<sizeof(countNameSource), 0x6FE153231D879777ULL> countName(countNameSource);
    window->SendMsg(nullptr, UM_SET_TARGET, 10003, reinterpret_cast<std::intptr_t>(countName.Text()), 0, 0);

    if (g_loginCancelRequested == 1) {
      g_loginCancelRequested = 0;
      LoadCustomServerList(this);
    } else if (this->serverCount < 1 || this->serverCount > 8) {
      LoadCustomServerList(this);
    }
    if (this->serverCount != 0) {
      g_customServerListLoaded = 1;
    }

    char(*displayNames)[128] = CustomServerBalloons();
    for (int i = 0; i < this->serverCount; ++i) {
      const ServerAddress2& server = servers[i];
      char text[64];
      std::memset(text, 0, sizeof(text));
      if (server.address.property == 1) {
        std::sprintf(text, LookupMessage(MSI_SERVER_PROPERTY_NEW));
      }
      const bool custom = server.address.userCount == 4 && server.address.state == 0;
      std::sprintf(text + std::strlen(text), custom ? server.domain : server.address.name);

      std::string status;
      std::string green = "^00B509";
      std::string orange = "^FF7800";
      std::string red = "^FF0000";
      std::string purple = "^BA00FF";
      switch (server.address.userCount) {
      case 0:
        status = static_cast<const std::string&>(FormatOwnedString("%s%s  ^000000%s", green.c_str(), LookupMessage(MSI_SERVER_USERCOUNT_STATE_VERY_BUSY), text));
        break;
      case 1:
        status = static_cast<const std::string&>(FormatOwnedString("%s%s  ^000000%s", orange.c_str(), LookupMessage(MSI_MENUICON_MAP), text));
        break;
      case 2:
        status = static_cast<const std::string&>(FormatOwnedString("%s%s  ^000000%s", red.c_str(), LookupMessage(MSI_WORLDSTORE_ONLY_INVENTORY), text));
        break;
      case 3:
        status = static_cast<const std::string&>(FormatOwnedString("%s%s  ^000000%s", purple.c_str(), LookupMessage(MSI_WORLDSTORE_PUT_BLOCK_PETEGG), text));
        break;
      }

      switch (server.address.state) {
      case 2:
        status.append(LookupMessage(MSI_SERVER_USERCOUNT_STATE_NICE));
        break;
      case 3:
        status.append(LookupMessage(MSI_SERVER_USERCOUNT_STATE_NORMAL));
        break;
      case 4:
        status.append(LookupMessage(MSI_SERVER_USERCOUNT_STATE_BUSY));
        break;
      }

      if (!status.empty()) {
        strcpy_s(text, sizeof(text), status.c_str());
      }
      window->m_serverList->AddLine(text);

      if (custom && static_cast<unsigned int>(i) < 8 && displayNames[i][0] != '\0') {
        std::string balloon;
        balloon.assign(displayNames[i], std::strlen(displayNames[i]));
        UIServerListBox* list = static_cast<UIServerListBox*>(window->m_serverList);
        if (list->balloonTexts.size() != list->balloonTexts.capacity()) {
          list->balloonTexts.push_back(balloon);
        } else if (list->balloonTexts.capacity() == 0) {
          list->balloonTexts.reserve(8);
          list->balloonTexts.push_back(balloon);
        }
      }
    }

    window->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
    break;
  }
  case 7:
    g_screenShakeDuration = 0;
    g_statusRefreshSuppressed = 0;
    g_skillTooltipActive = 0;
    g_session.cartCount = 0;
    g_session.cartWeight = 0;
    g_session.petJob = -1;
    g_session.SetEquippedArrowIndex(0);
    g_session.guildLeader = 0;
    g_renewQuestMgr->m_ongoingQuests.clear();
    g_session.ClearAchievements();
    g_nProtectGameGuardMgr->IncrementCounter();
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_rodexMailStore->Clear();
    if (UINewSelectCharWnd* window = dynamic_cast<UINewSelectCharWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(WID_NEWSELECTCHARWND)))) {
      window->m_gameStartPending = false;
    }
    g_session.ClearStoreSearchResults();
    g_session.ClearPairValues(1, 1, 1);
    g_windowMgr.CloseWindow(WID_REPLAYRECCONTROLWND);
    g_windowMgr.CloseWindow(WID_ROULLETTEWND);
    g_session.RestoreSelectionItems();
    g_session.ClearItemQuantities();
    g_session.ClearInventory();
    g_session.ResetEquipment();
    g_session.ResetCostumeItems();
    g_session.ClearMerchantShopItems();
    g_session.ResetSwapCostumeItems();
    g_session.ResetSwapEquippedItems();
    switch (g_session.m_passwordWndMode) {
    case 2:
      g_session.m_passwordInputAlternate = false;
      g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
      g_windowMgr.EnsureWindow(WID_SECONDPASSWORDINPUTWND);
      break;
    case 3:
      g_session.m_passwordInputAlternate = false;
      g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
      g_windowMgr.EnsureWindow(WID_SECONDPASSWORDINPUTWND);
      g_windowMgr.secondPasswordInputWnd->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_STORE_PASSWORD, 0, 0, 0);
      break;
    case 4:
      g_session.m_passwordInputAlternate = true;
      g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
      g_windowMgr.EnsureWindow(WID_SECONDPASSWORDEDITWND);
      g_windowMgr.secondPasswordEditWnd->SendMsg(nullptr, UM_COMMAND, ID_CHANGE_STORE_PASSWORD, 0, 0, 0);
      g_windowMgr.secondPasswordEditWnd->SendMsg(nullptr, UM_COMMAND, ID_OK, 1, 0, 0);
      break;
    case 5:
      g_session.m_passwordInputAlternate = true;
      g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
      g_windowMgr.EnsureWindow(WID_SECONDPASSWORDEDITWND);
      break;
    }
    break;
  case 8:
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_windowMgr.EnsureWindow(WID_NEWMAKECHARWND);
    break;
  case 4: {
    g_playerNameDictionary->Clear();
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    if (!std::strstr(g_session.clientVersion, "Dev")) {
      const std::array<std::uint8_t, 29> hostSource = CRYPT_BYTES(29, "kro-acc1.ragnarok.co.kr:6900", 0x2FEB8739E5975BD1ULL);
      thread_local EncryptedName<sizeof(hostSource), 0x2FEB8739E5975BD1ULL> host(hostSource);
      host.Text();
      const std::array<std::uint8_t, 3> formatSource = CRYPT_BYTES(3, "%s", 0x33133B977BCD03CDULL);
      thread_local EncryptedName<sizeof(formatSource), 0x33133B977BCD03CDULL> format(formatSource);
      format.Text();
    } else {
      const std::array<std::uint8_t, 30> hostSource = CRYPT_BYTES(30, "kro-qm-1a.ragnarok.co.kr:6900", 0xA12DAB392545D72FULL);
      thread_local EncryptedName<sizeof(hostSource), 0xA12DAB392545D72FULL> host(hostSource);
      host.Text();
      const std::array<std::uint8_t, 3> formatSource = CRYPT_BYTES(3, "%s", 0x6BA1C1132F033B75ULL);
      thread_local EncryptedName<sizeof(formatSource), 0x6BA1C1132F033B75ULL> format(formatSource);
      format.Text();
    }

    CharServerEndpoint endpoint;
    endpoint.ip[0] = '\0';
    endpoint.port = 0;
    endpoint.host[0] = '\0';
    const char* address = g_clientInfoAddress;
    const char* port = g_clientInfoPort;
    const char* domain = g_clientInfoDomain;
    if (address) {
      std::strcpy(endpoint.ip, address);
    }
    if (domain || address) {
      std::strcpy(endpoint.host, domain ? domain : address);
    }
    if (port) {
      endpoint.port = atoi(port);
    }

    this->connectionReady = GetRagConnection().Connect(*reinterpret_cast<const ConnectionEndpoint*>(&endpoint));
    if (!this->connectionReady) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 1, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
      break;
    }

    g_packetReassembler->Notify(2);
    SendExecutableDigest(g_serviceType, g_serviceMode, g_instance, GetRagConnection(), *reinterpret_cast<const PacketLengths*>(&GetRagConnection()));
    AccountLoginPacket packet;
    packet.id = ID_CA_LOGIN;
    packet.version = g_patchedClientVersion;
    std::memcpy(packet.name, &this->loginCredentials[0x40], sizeof(packet.name));
    if (g_serviceType == 7) {
      char password[24];
      DecryptAES(reinterpret_cast<const char*>(this->loginCredentials), password, sizeof(password));
      std::memcpy(packet.password, password, sizeof(packet.password));
    } else if (g_serviceType == 4) {
      DecryptRijndael192(reinterpret_cast<const char*>(this->loginCredentials), packet.password);
    } else {
      std::memcpy(packet.password, this->loginCredentials, sizeof(packet.password));
    }
    packet.clientType = static_cast<unsigned char>(GetClientType());
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_WAITING_RESPONSE_FROM_SERVER), 16, 1);
    break;
  }
  case 24:
    g_windowMgr.EnsureWindow(WID_LOGINOTPWND);
    break;
  case 5: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_session.m_partyBooking.Reset();
    GetRagConnection().Close();
    this->connectionReady = GetRagConnection().Connect(*reinterpret_cast<const ConnectionEndpoint*>(&g_charServerEndpoint));
    if (!this->connectionReady) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 1, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
      break;
    }

    g_packetReassembler->Notify(2);
    g_charServerAccountIdPending = 1;
    CharServerEnterPacket packet;
    packet.id = ID_CH_ENTER;
    packet.clientType = 0;
    packet.aid = g_session.m_aid;
    packet.authCode = account.authCode;
    packet.userLevel = account.userLevel;
    packet.sex = static_cast<unsigned char>(account.sex);
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_WAITING_RESPONSE_FROM_SERVER), 16, 1);
    g_accountLinkedUserDataMgr->Load(g_session.m_aid, g_session.m_accountName);
    break;
  }
  case 10: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    MakeCharacterPacket packet;
    packet.id = ID_CH_MAKE_CHAR_TRIBE;
    std::memcpy(packet.name, g_charInfo.name, sizeof(packet.name));
    packet.slot = static_cast<unsigned char>(g_selectedSlotIndex);
    packet.headPalette = g_charInfo.headPalette;
    packet.head = g_charInfo.head;
    packet.job = g_charInfo.job;
    packet.sex = g_charInfo.sex;
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    if (UIWaitWnd* wait = static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))) {
      wait->SetMessage(LookupMessage(MSI_MAKING_CHARACTER), 17, 1);
    }
    break;
  }
  case 11: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    DeleteCharacterPacket packet;
    packet.id = ID_CH_DELETE_CHAR3;
    std::strncpy(packet.code, reinterpret_cast<const char*>(this->billingInfo), sizeof(packet.code));
    const CharacterInfo* character = reinterpret_cast<const CharacterInfo*>(this->SendMsg(MM_QUERYCHARICTORINFO, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
    packet.gid = character->gid;
    if (g_serviceType == 0) {
      if (this->billingInfo[5] == '\0') {
        packet.code[0] = '\0';
      }
      GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    } else if (g_serviceType == 10 || g_serviceType == 12 || g_serviceType == 1) {
      DeleteCharacterEmailPacket email;
      email.gid = character->gid;
      email.id = ID_CH_DELETE_CHAR2;
      std::strncpy(email.code, reinterpret_cast<const char*>(this->billingInfo), sizeof(email.code));
      email.code[49] = '\0';
      GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(email.id), &email);
    } else {
      GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    }
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_DELETING_CHARACTER), 17, 1);
    break;
  }
  case 9: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    SelectCharacterPacket packet;
    packet.id = ID_CH_SELECT_CHAR;
    packet.slot = static_cast<unsigned char>(g_selectedSlotIndex);
    g_cashEmotionMgr->ClearOwnedPacks();
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_WAITING_RESPONSE_FROM_SERVER), 16, 1);
    break;
  }
  case 12: {
    g_characterLinkedUserDataMgr->Load(g_session.m_aid, g_session.m_gid, g_session.m_accountName);
    g_runeSystemMgr->Reset();
    g_session.memorialWindowOpen = 0;
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    this->connectionReady = GetRagConnection().Connect(g_zoneServerEndpoint);
    if (!this->connectionReady) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(MSI_SERVER_CONNECTION_FAILED), 1, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
      break;
    }

    g_nProtectGameGuardMgr->IncrementCounter();
    g_packetReassembler->Notify(1);
    char keyName[MAX_PATH];
    std::strcpy(keyName, g_subKey);
    const std::array<std::uint8_t, 9> cashKeySource = CRYPT_BYTES(9, "\\NeoCash", 0xB39FA7917BCFBB8DULL);
    thread_local EncryptedName<sizeof(cashKeySource), 0xB39FA7917BCFBB8DULL> cashKey(cashKeySource);
    std::strcat(keyName, cashKey.Text());
    DWORD category = 0;
    DWORD disposition = 0;
    HKEY key;
    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, keyName, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
      const std::array<std::uint8_t, 14> valueNameSource = CRYPT_BYTES(14, "CASH_CATEGORY", 0x8D1D0FF1AF2B13F7ULL);
      thread_local EncryptedName<sizeof(valueNameSource), 0x8D1D0FF1AF2B13F7ULL> valueName(valueNameSource);
      RegSetValueExA(key, valueName.Text(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&category), sizeof(category));
      RegCloseKey(key);
    }

    ZoneEnterPacket packet;
    packet.packetType = ID_CZ_ENTER2;
    packet.clientTime = timeGetTime();
    packet.aid = g_session.m_aid;
    packet.gid = g_session.m_gid;
    packet.authCode = account.authCode;
    packet.sex = static_cast<unsigned char>(g_session.GetCurrentSex());
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.packetType), &packet);
    g_session.SetOption(17, 1, 0);
    GetRagConnection().SetBlock(1);
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_WAITING_RESPONSE_FROM_SERVER), 16, 1);
    break;
  }
  case 19:
    g_screenShakeDuration = 0;
    g_statusRefreshSuppressed = 0;
    g_skillTooltipActive = 0;
    g_session.petJob = -1;
    g_session.SetEquippedArrowIndex(0);
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_windowMgr.EnsureWindow(WID_SELCHARFORUSERVERWND);
    break;
  case 29: {
    if (g_serviceType == 6 || g_serviceType == 10 || g_serviceType == 12) {
      g_serviceOptions.Set(0x6C, 0);
    }
    const unsigned char flags = g_serviceOptions.Get(0x6C);
    g_session.SetServiceFlags(flags, flags);
    reinterpret_cast<Replay&>(ReplayFileList::Instance()).SetBaseFrame(reinterpret_cast<std::uint32_t>(&Replay::Instance()));
    const std::array<std::uint8_t, 44> wallpaperSource = CRYPT_BYTES(44, "유저인터페이스\\login_interface\\warning2.bmp", 0x4F99DF03C3EF31B7ULL);
    thread_local EncryptedName<sizeof(wallpaperSource), 0x4F99DF03C3EF31B7ULL> wallpaper(wallpaperSource);
    this->wallpaperName = std::string(wallpaper.Text());
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_windowMgr.EnsureWindow(WID_NOTICECONFIRMWND)->SendMsg(nullptr, UM_SET_TARGET, 10026, 0, 0, 0);
    break;
  }
  case 25:
    if (ReplayFileList::Instance().Open(32, 0, 0) == 1) {
      this->pendingCursorType = 28;
    } else {
      g_windowMgr.Confirm(LookupMessage(MSI_STORE_TABNAME_9), 4, true, true, 0, "Message", 280, 120, 0);
      this->pendingCursorType = 26;
    }
    break;
  case 26:
    this->serverCount = ReplayFileList::Instance().fileState;
    if (this->serverCount > 100) {
      this->serverCount = 0;
      ReplayFileList::Instance().fileState = 0;
    }
    for (int i = 0; i < this->serverCount; ++i) {
      std::memcpy(&servers[i], &reinterpret_cast<const ServerAddress2*>(ReplayFileList::Instance().buffer)[i], sizeof(ServerAddress2));
    }
    this->SendMsg(LMM_SELECT_REPLAY_DATA, 0, 0, 0, 0);
  case 27: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetBackgroundSurface(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    UISelectReplayDataWnd* window = static_cast<UISelectReplayDataWnd*>(g_windowMgr.EnsureWindow(WID_SELECTREPLAYDATAWND));
    ReplayFileList::Instance().Refresh();
    const int count = static_cast<int>(ReplayFileList::Instance().files.size());
    for (int i = 0; i < count; ++i) {
      window->m_list->AddLine(ReplayFileList::Instance().FileName(i));
    }
    window->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
    ReplayFileList::Instance().fileState = this->serverCount;
    if (this->serverCount > 100) {
      this->serverCount = 0;
      ReplayFileList::Instance().fileState = 0;
    }
    for (int i = 0; i < this->serverCount; ++i) {
      const ServerAddress2 server = servers[i];
      reinterpret_cast<ServerAddress2*>(ReplayFileList::Instance().buffer)[i] = server;
    }
    break;
  }
  case 28: {
    std::string key = nullsub_481(47, std::_Random_device());
    unsigned char digest[16] = {};
    ComputeLoginDigest(digest, key.data(), static_cast<int>(key.size()));
    g_pakRegistry.ApplyAuxiliary(digest, sizeof(digest));
    SecureZeroMemory(digest, sizeof(digest));
    g_windowMgr.CloseWindow(WID_ROULLETTEWND);
    g_session.RestoreSelectionItems();
    g_session.ClearItemQuantities();
    g_session.ClearInventory();
    g_session.ResetEquipment();
    g_session.ResetCostumeItems();
    g_session.ClearMerchantShopItems();
    g_session.ResetSwapCostumeItems();
    g_session.ResetSwapEquippedItems();
    Replay::Instance().SetState(6);
    ReplayHeader header = Replay::Instance().header;
    g_session.SetPlayerNameKey(reinterpret_cast<const unsigned char*>(header.characterName));
    g_session.SetNavigationPlayerMap(header.mapName);
    g_session.mapPropertyId = header.accountId;
    g_session.m_playerX = header.cellX;
    g_session.m_playerY = header.cellY;
    g_session.m_playerDir = header.playerDirection;
    g_windowMgr.ApplyReplayOptions(header.options);
    g_session.ResetCharacterSlotState();
    g_session.ReadReplayCharacter(Replay::Instance().ReadSection(3));
    char mapName[56];
    std::sprintf(mapName, "%s.rsw", g_session.m_navigationPlayerMap);
    g_modeMgr.Switch(1, mapName);
    break;
  }
  case 30: {
    UISelectServerWnd* window = static_cast<UISelectServerWnd*>(g_windowMgr.EnsureWindow(WID_SELECTSERVERWND));
    const std::array<std::uint8_t, 8> countNameSource = CRYPT_BYTES(8, "NUMZONE", 0x9171997D6963C9F7ULL);
    thread_local EncryptedName<sizeof(countNameSource), 0x9171997D6963C9F7ULL> countName(countNameSource);
    window->SendMsg(nullptr, UM_SET_TARGET, 10028, reinterpret_cast<std::intptr_t>(countName.Text()), 0, 0);
    for (unsigned int i = 0; i < this->mapStates.size(); ++i) {
      char mapName[16];
      std::strncpy(mapName, g_session.GatName2RswName(reinterpret_cast<const char*>(&this->mapStates[i].data)), sizeof(mapName));
      window->m_serverList->AddLine(g_session.GetMapDisplayName(mapName));
    }
    window->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
    break;
  }
  case 31: {
    SelectMapPacket packet;
    packet.id = ID_CH_SELECT_ACCESSIBLE_MAPNAME;
    packet.slot = static_cast<unsigned char>(g_selectedSlotIndex);
    packet.map = static_cast<unsigned char>(this->mapStates.front().index);
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    break;
  }
  default:
    break;
  }
}
