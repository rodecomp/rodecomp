#include "Client/Mode/CLoginMode.h"
#include "Utility/EncryptedName.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Application/ApplicationMessages.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/ClientInfo.h"
#include "Client/Application/Launcher.h"
#include "Archive/PakRegistry.h"
#include "Client/Input/Mouse.h"
#include "Client/Stove/StoveMgr.h"
#include "Client/ModeFadeOut.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Client/Replay.h"
#include "Client/Timer.h"
#include "Client/Input/InputCharset.h"
#include "Client/Stove/StovePCSDK.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EntryQueueMgr.h"
#include "Contents/CheckAttendanceMgr.h"
#include "Contents/TipOfTheDay.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/PartialShutDownSchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/ClientKeepalive.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/XmlNode.h"
#include "Resource/ResourcePreload.h"
#include "Resource/TextureNames.h"
#include "Security/NProtectGameGuardMgr.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/UIBartermarketWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Dialogs/UIPasswordWnd.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UISelCharForUServerWnd.h"
#include "Utility/Async/NAsyncWorkMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/GGHashMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextTokens.h"
#include "Client/Mode/CMode.h"
#include "Client/Registry.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Items/RuneSystem.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RodexMailStore.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Network/ExecutableDigestPacket.h"
#include "Security/ReassemblyPacketMgr.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIReplayListBox.h"
#include "UI/Window/UISelectReplayDataWnd.h"
#include "UI/Window/UISelectServerWnd.h"
#include "Utility/Cipher/AES.h"
#include "Utility/Cipher/Rijndael.h"

#include <algorithm>
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
  /* 0018 */ unsigned char unknown_0018[3];
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
  /* 0004 */ int unknown_0004;
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
const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

struct CharacterInfo2SizeErrorFormat {
  __declspec(noinline) EncryptedName<75, 0xED15938D333D2DB9ULL>& operator()() const;
};

// OFFSET: 0x00D8CA50
// FUNCTION: fn_22261
EncryptedName<75, 0xED15938D333D2DB9ULL>& CharacterInfo2SizeErrorFormat::operator()() const {
  const char source[] = {'\xFA', '\x65', '\x7C', '\x61', '\xCC', '\xD0', '\x41', '\xA8', '\xEB', '\x72', '\x74', '\x7D', '\xCB', '\xDC', '\x27', '\xCD',
                         '\xCA', '\x44', '\x47', '\x56', '\xAD', '\xF6', '\x67', '\x9F', '\xD6', '\x5F', '\x1C', '\x12', '\xAD', '\xF0', '\x7D', '\x8C',
                         '\xCB', '\x4C', '\x5E', '\x47', '\xE8', '\xE1', '\x35', '\x83', '\xCC', '\x40', '\x1D', '\x09', '\xAD', '\xB6', '\x71', '\xCD',
                         '\x99', '\x6E', '\x75', '\x72', '\xDF', '\xD2', '\x56', '\xB9', '\xFC', '\x7F', '\x62', '\x7A', '\xC3', '\xD5', '\x5A', '\xDF',
                         '\x99', '\x5E', '\x54', '\x49', '\xE8', '\xB3', '\x2F', '\xCD', '\x9C', '\x49', '\x3D'};
  thread_local EncryptedName<sizeof(source), 0xED15938D333D2DB9ULL> name(source);
  return name;
}

struct PasswordRetryExceededMessage {
  __declspec(noinline) EncryptedName<103, 0xBF2B4B31397FF5A7ULL>& operator()() const;
};

// OFFSET: 0x00D8CB50
// FUNCTION: fn_22262
EncryptedName<103, 0xBF2B4B31397FF5A7ULL>& PasswordRetryExceededMessage::operator()() const {
  const char source[] = {'\x1D', '\x04', '\xC6', '\xE9', '\x88', '\xB3', '\xE3', '\x1C', '\x1F', '\x53', '\x5F', '\x0A', '\xF9', '\xF3', '\x0B', '\x7F',
                         '\x78', '\x4D', '\x87', '\xF9', '\xE5', '\xFC', '\xE9', '\x78', '\x68', '\x4A', '\xCB', '\x84', '\xF1', '\xFF', '\xE4', '\x0B',
                         '\x7E', '\xDB', '\x5F', '\x83', '\xC0', '\xF2', '\xFB', '\x06', '\x5F', '\x3D', '\xDC', '\x81', '\x97', '\x6B', '\xE3', '\x11',
                         '\x67', '\x3B', '\xB8', '\xE1', '\xF0', '\x9D', '\x97', '\x03', '\x18', '\x11', '\x51', '\x19', '\x11', '\x6B', '\x0B', '\x9F',
                         '\x87', '\xD5', '\x5F', '\x19', '\x11', '\x6B', '\x0B', '\x9F', '\x87', '\x30', '\xD3', '\x8F', '\xC2', '\x8B', '\xE7', '\x01',
                         '\x57', '\x33', '\xD1', '\x19', '\x8C', '\x8C', '\xEC', '\x5F', '\x67', '\x4E', '\x5F', '\xF8', '\x8F', '\xFC', '\xCA', '\x78',
                         '\x72', '\x41', '\xB0', '\x8D', '\xE8', '\x65', '\x2B'};
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
  UIWindow* window = g_windowMgr.window_03D4;
  int message;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 0:
    g_windowMgr.Confirm(LookupMessage(0x4BE), 0, true, false, 0, "Message", 280, 120, 0);
    if (window) {
      window->SendMsg(nullptr, 6, 0x160, 0, 0, 0);
    }
    return;
  case 1:
    message = 0x4C2;
    break;
  case 2:
    if (g_windowMgr.Confirm(LookupMessage(0x4C3), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
      return;
    }
    if (window) {
      window->SendMsg(nullptr, 6, 0xB9, 0, 0, 0);
    }
    return;
  case 3:
    message = 0x4BF;
    break;
  case 4:
    message = 0x4C4;
    goto retry;
  case 5:
    message = 0x7E6;
    break;
  case 6:
    message = 0x7E7;
    break;
  case 7:
    message = 0x7EB;
    goto retry;
  case 8:
    message = 0x7EC;
    goto retry;
  case 9:
    message = 0x7ED;
  retry:
    if (g_windowMgr.Confirm(LookupMessage(message), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
      return;
    }
    if (window) {
      window->SendMsg(nullptr, 6, 0x15F, 0, 0, 0);
    }
    return;
  case 10:
    message = 0x7E8;
    break;
  default:
    return;
  }

  if (g_windowMgr.Confirm(LookupMessage(message), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8 && g_windowMgr.window_03DC) {
    g_windowMgr.CloseWindow(0x80);
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
    case 2:
      return 9;
    case 3:
      return 4;
    case 4:
      return 5;
    case 5:
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
    case 15:
      return 33;
    case 19:
      return 35;
    case 14:
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
    case 2:
      return 28;
    case 5:
      return 29;
    case 15:
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
    case 2:
      return 3;
    case 3:
      return 4;
    case 4:
      return 5;
    case 5:
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
    case 14:
      return 25;
    case 15:
      return 26;
    case 17:
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
      const char source[] = {'\x32', '\x2B', '\x1F', '\xFB', '\x90', '\xCE', '\x71', '\x3D', '\x4F', '\x36', '\x08', '\x93', '\x8F', '\xB3', '\x6C',
                             '\x2A', '\x27', '\x29', '\x75', '\x8E', '\x98', '\xDB', '\x73', '\x57', '\x3A', '\x3E', '\x1D', '\x91', '\xE5', '\xEB'};
      _snprintf(text, sizeof(text), DecryptName<0x0F41EBBDA32D1B17ULL>(source), adapter->Address[0], adapter->Address[1], adapter->Address[2],
        adapter->Address[3], adapter->Address[4], adapter->Address[5]);
      delete[] reinterpret_cast<char*>(adapter);
      return text;
    }
  }

  const char source[] = {'\xF0', '\x76', '\xE8', '\x25', '\xD3', '\xB4', '\xE6', '\xC8', '\xEC', '\x73', '\xF1', '\x3A', '\xD4', '\xAC', '\xF8', '\xCD',
                         '\xF7', '\x47'};
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

    maxSlot = std::max(static_cast<int>(this->characters[i].slot), maxSlot);
  }

  return maxSlot;
}

// OFFSET: 0x00D8E770
// FUNCTION: fn_22273
const char* GetUserCountText(unsigned short userCount) {
  if (userCount <= 1000) {
    return LookupMessage(825);
  }

  if (userCount <= 3000) {
    return LookupMessage(826);
  }

  if (userCount <= 6000) {
    return LookupMessage(827);
  }

  if (userCount <= 10000) {
    return LookupMessage(828);
  }

  return LookupMessage(829);
}

// OFFSET: 0x00D8E990
// FUNCTION: fn_22274
void CLoginMode::ReceiveCharacterSlots(const CharacterSlotsPacket* packet) {
  g_session.unknown_59C0 = packet->normalSlots;
  g_session.unknown_59C4 = packet->premiumSlots;
  g_session.unknown_59C8 = packet->billingSlots;
  g_session.unknown_59CC = packet->validSlots;
  g_session.unknown_59D4 = packet->producibleSlots;

  SecureZeroMemory(this->characters, sizeof(this->characters));
  g_session.unknown_59BC = 0;
  this->unknown_01D8 = 0;
  this->unknown_01DC = 0;

  this->slotExtension = packet->extension;
  this->slotExtension.unknown_0001 = htonl(this->slotExtension.unknown_0001);
  this->slotExtension.unknown_0005 = htonl(this->slotExtension.unknown_0005);
  this->slotExtension.unknown_0009 = htonl(this->slotExtension.unknown_0009);
}

// OFFSET: 0x00D8EAC0
// FUNCTION: fn_22275
void CLoginMode::ReceiveAcceptEnter(const AcceptEnterPacket* packet) {
  this->slotExtension = packet->extension;
  this->slotExtension.unknown_0001 = htonl(this->slotExtension.unknown_0001);
  this->slotExtension.unknown_0005 = htonl(this->slotExtension.unknown_0005);
  this->slotExtension.unknown_0009 = htonl(this->slotExtension.unknown_0009);

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

  g_session.unknown_59BC = this->characterCount;
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
  UIWindow* window = g_windowMgr.window_03D4;
  int message;
  switch (packet->result) {
  case 0:
    g_windowMgr.Confirm(LookupMessage(1214), 0, true, false, 0, "Message", 280, 120, 0);
    if (window) {
      window->SendMsg(nullptr, 6, 352, 0, 0, 0);
    }
    return;
  case 1:
    message = 1218;
    break;
  case 2:
    message = 1219;
    break;
  case 3:
    if (g_windowMgr.Confirm(LookupMessage(1215), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, 6, 351, 0, 0, 0);
    }
    return;
  case 4:
    if (g_windowMgr.Confirm(LookupMessage(1220), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, 6, 351, 0, 0, 0);
    }
    return;
  case 5:
    message = 2022;
    break;
  case 6:
    message = 2023;
    break;
  case 10:
    message = 2024;
    break;
  default:
    return;
  }

  if (g_windowMgr.Confirm(LookupMessage(message), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
    window->SendMsg(nullptr, 6, 185, 0, 0, 0);
  }
}

// OFFSET: 0x00D8F040
// FUNCTION: fn_22278
void CLoginMode::ReceiveCharacterPage(const CharacterPagePacket* packet) {
  const unsigned int count = (packet->length - 4) / sizeof(CharacterInfo);
  if ((packet->length - 4) % sizeof(CharacterInfo) != 0) {
    char text[256];
    memset(text, 0, sizeof(text));
    const char source[] = {'\x4C', '\x07', '\x04', '\xFD', '\xDC', '\x06', '\xCD', '\x46', '\x5D', '\x10', '\x0C', '\xE1', '\xDB', '\x0A', '\xAB', '\x23',
                           '\x7C', '\x26', '\x3F', '\xCA', '\xBD', '\x20', '\xEB', '\x71', '\x60', '\x3D', '\x64', '\x8E', '\xBD', '\x06', '\xD1', '\x42',
                           '\x5D', '\x0E', '\x06', '\xFB', '\xD8', '\x17', '\xC6', '\x4A', '\x41', '\x09', '\x0A', '\x9D', '\xBD', '\x36', '\xF0', '\x79',
                           '\x6A', '\x6F', '\x7F', '\x8F', '\xB8', '\x21', '\x99'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x0399459DAF454F0FULL>(source), sizeof(CharacterInfo));
    g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
    return;
  }

  const unsigned int total = this->unknown_01D8 + count;
  if (total > 68) {
    char text[256];
    memset(text, 0, sizeof(text));
    const char source[] = {'\xE8', '\xE7', '\xCC', '\xDF', '\xF2', '\xCE', '\xF3', '\x90', '\xF9', '\xF0', '\xC4', '\xC3', '\xF5', '\xC2', '\x95', '\xF5',
                           '\xE8', '\xC1', '\xF9', '\xAD', '\xFC', '\xFB', '\xC2', '\xA7', '\x8B', '\x8A', '\xE9', '\xAD', '\x9C', '\xAD', '\x82', '\xB1',
                           '\xAB'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0xD5A78DB38D8DAFABULL>(source), total, 68);
    g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
    return;
  }

  memcpy_s(&this->characters[this->unknown_01D8], sizeof(this->characters) - this->unknown_01D8 * sizeof(CharacterInfo),
    packet->characters, count * sizeof(CharacterInfo));
  __time32_t now;
  _time32(&now);
  for (int i = 0; i < static_cast<int>(count); ++i) {
    if (this->characters[this->unknown_01D8 + i].sex == 99) {
      this->characters[this->unknown_01D8 + i].sex = g_session.unknown_0E9C[0];
    }
    if (this->characters[this->unknown_01D8 + i].deleteDate != 0) {
      this->characters[this->unknown_01D8 + i].deleteDate += now;
    }
    CharacterInfo& character = this->characters[this->unknown_01D8 + i];
    this->characters[this->unknown_01D8 + i].bodyStyle =
      static_cast<std::int16_t>(g_session.GetBodyStyle(character.job, character.bodyStyle, character.unknown_0058));
  }

  this->unknown_01D8 += count;
  if (count != 3) {
    this->characterCount = this->unknown_01D8;
    g_session.unknown_59BC = this->unknown_01D8;
    this->unknown_01D8 = 0;
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
  g_session.unknown_59BC = this->characterCount;
  memcpy_s(this->characters, sizeof(this->characters), packet->characters, this->characterCount * sizeof(CharacterInfo));
  this->pendingCursorType = 7;
}

// OFFSET: 0x00D8FA00
// FUNCTION: fn_22281
void CLoginMode::ReceiveDeleteCharacter(const CharacterDeletePacket* packet) {
  std::string message;
  switch (packet->result) {
  case 0:
    message = LookupMessage(1816);
    break;
  case 1:
    this->characterDeleted = true;
    break;
  case 2:
    message = LookupMessage(1820);
    break;
  case 3:
    message = LookupMessage(1817);
    break;
  case 4:
    message = LookupMessage(1821);
    break;
  case 5:
    message = LookupMessage(1822);
    break;
  case 6:
    message = LookupMessage(2369);
    break;
  case 7:
    message = LookupMessage(301);
    break;
  case 8:
    message = LookupMessage(3044);
    break;
  default:
    message = LookupMessage(1816);
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
  if (packet->result == 0) {
    message = LookupMessage(1816);
  } else if (packet->result == 1) {
    for (int i = 0; i < this->characterCount; ++i) {
      if (this->characters[i].gid == packet->gid) {
        this->characters[i].deleteDate = 0;
        break;
      }
    }
    this->pendingCursorType = 7;
  } else if (packet->result == 2) {
    message = LookupMessage(1817);
  } else {
    message = LookupMessage(1816);
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
    message = LookupMessage(1816);
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
    message = LookupMessage(1817);
    break;
  case 4:
    message = LookupMessage(1818);
    break;
  case 5:
    message = LookupMessage(1819);
    break;
  case 6:
    message = LookupMessage(2981);
    break;
  case 7:
    message = LookupMessage(3092);
    break;
  default:
    message = LookupMessage(1816);
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
    const char source[] = {'\xD8', '\xE6', '\x32', '\x48', '\x72', '\x89', '\x4C', '\x11', '\x95', '\xC2', '\x20', '\x55', '\x40', '\x89', '\x4E', '\x0D',
                           '\xD0', '\xAF', '\x32', '\x4C', '\x69', '\x98', '\x07', '\x0A', '\xDC', '\xF5', '\x24', '\x05', '\x2E', '\xDD', '\x0A', '\x1D',
                           '\x9C', '\x8F'};
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
    g_windowMgr.Confirm(LookupMessage(1811), 0, true, false, 0, "Message", 280, 120, 0);
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
  this->unknown_6F50.clear();
}

// OFFSET: 0x00D975C0
// FUNCTION: fn_22301
void CLoginMode::Slot5(int value) {
  if (value == 0) {
    if (this->running == 0) {
      return;
    }
    if (g_modalImmediate != 0) {
      return;
    }
  } else {
    GetRagConnection().UpdateStatus();
  }

  PumpApplicationMessages();
  this->PollNetworkStatus();
  SendClientKeepalive(this->cursorType, this->unknown_6F10, GetRagConnection(), *reinterpret_cast<const PacketLengths*>(&GetRagConnection()), g_session.m_aid);
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
    g_windowMgr.SetSurface0478(bitmap);
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
        g_windowMgr.CenterSurface0478();
      }
    } else {
      g_windowMgr.CenterSurface0478();
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
      this->SendMsg(0x2725, 0, 0, 0, 0);
    }
    if (g_serviceType == 5 || g_serviceType == 7) {
      if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x3A98) {
        if (this->window_6F14 == nullptr) {
          this->window_6F14 = g_windowMgr.EnsureWindow(6);
          if (this->window_6F14 == nullptr) {
            break;
          }
        }
        this->window_6F14->SendMsg(nullptr, 0x58, 0, 0, 0, 0);
        this->window_6F14->Invalidate();
      }
    }
    break;
  case 5:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x13880) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(1), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
        this->otpStarted = timeGetTime();
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
    }
    break;
  case 6:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x3A98) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(1), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
        g_modeMgr.Quit();
      }
    }
    break;
  case 12:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x3A98) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(1), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
        this->otpStarted = timeGetTime();
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
    }
    break;
  case 38:
    if (this->unknown_6F75 == true) {
      if (static_cast<int>(timeGetTime()) >= this->otpStarted + 0xEA60) {
        GetRagConnection().Close();
        if (this->unknown_6F74 == true && this->unknown_6F75 == true) {
          this->unknown_6F76 = true;
          if (!g_windowMgr.HasModalWindow()) {
            if (g_windowMgr.Confirm(LookupMessage(0xD16), 0, true, false, 0, "Message", 280, 120, 0) == 0xB8) {
              this->unknown_6F75 = false;
              this->unknown_6F76 = false;
              this->unknown_6F74 = false;
              this->otpStarted = timeGetTime();
              this->pendingCursorType = 3;
            }
          }
        }
      }
    } else {
      if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x13880) {
        GetRagConnection().Close();
        if (this->unknown_6F74 == false) {
          if (g_windowMgr.Confirm(LookupMessage(0xD70), 0, true, false, 0, "Message", 280, 120, 0) == 0xB8) {
            this->unknown_6F74 = false;
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
  SendClientKeepalive(this->cursorType, this->unknown_6F10, GetRagConnection(), *reinterpret_cast<const PacketLengths*>(&GetRagConnection()), g_session.m_aid);
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
    g_windowMgr.SetSurface0478(bitmap);
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
        g_windowMgr.CenterSurface0478();
      }
    } else {
      g_windowMgr.CenterSurface0478();
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
  case 0:
    if (*reinterpret_cast<const int*>(&g_session.unknown_59DC[0x198]) == 1) {
      const char* url = LookupMessage(1308);
      const char source[] = {'\x30', '\x45', '\xCA', '\xFB', '\x9D'};
      ShellExecuteA(hWnd, DecryptName<0xCFF3899D95AF355FULL>(source), url, nullptr, nullptr, SW_SHOWNORMAL);
    } else {
      const char* url = LookupMessage(528);
      const char source[] = {'\x4A', '\x01', '\xCA', '\xA5', '\x0D'};
      ShellExecuteA(hWnd, DecryptName<0xE52DF30DCBAF7125ULL>(source), url, nullptr, nullptr, SW_SHOWNORMAL);
    }
    break;
  case 1: {
    const char urlSource[] = {'\x0F', '\xDF', '\xE5', '\xD5', '\xD2', '\x05', '\xCC', '\x3C', '\x0E', '\xD9', '\xFE', '\x8B', '\xD3', '\x5E', '\x84', '\x7D',
                              '\x06', '\xD9', '\xFE', '\xCE', '\xCE', '\x51', '\x8F', '\x7A', '\x09', '\xCE', '\xBF', '\xC6', '\xCE', '\x52', '\xCC', '\x7E',
                              '\x02', '\xC6', '\xF3', '\xC0', '\xD3', '\x10', '\x8E', '\x76', '\x0A', '\xC9', '\xF4', '\xD7', '\xCB', '\x50', '\x8A', '\x7D',
                              '\x56', '\x85', '\xF0', '\xD6', '\xD1', '\x00', '\x8E', '\x5D', '\x12', '\xC6', '\xAC', '\x94', '\xA1'};
    const char* file = DecryptName<0x13E33FA1A591AB67ULL>(urlSource);
    const char source[] = {'\x58', '\x37', '\xC8', '\x57', '\xBF'};
    ShellExecuteA(hWnd, DecryptName<0x17A9E5BF39AD4737ULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
    break;
  }
  case 2:
    break;
  case 3: {
    const char urlSource[] = {'\x2B', '\x5D', '\x27', '\x57', '\xEF', '\x90', '\x66', '\xED', '\x6D', '\x42', '\x26', '\x49', '\xB9', '\xCA', '\x27', '\xB7',
                              '\x20', '\x46', '\x3E', '\x08', '\xEA', '\xDE', '\x2A', '\xED', '\x7E', '\x5F', '\x3C', '\x52', '\xB6', '\xD7', '\x2C', '\xEB',
                              '\x6D', '\x44', '\x32', '\x4E', '\xBB', '\x99', '\x2A', '\xF0', '\x27', '\x14', '\x62', '\x17', '\xE5', '\x8F', '\x6F', '\xE9',
                              '\x2A', '\x4D', '\x6E', '\x11', '\xE4', '\xBF'};
    const char* file = DecryptName<0x9949BFD527532943ULL>(urlSource);
    const char source[] = {'\x00', '\x0D', '\x9E', '\x4D', '\x71'};
    ShellExecuteA(hWnd, DecryptName<0xCD8F077123FB7D6FULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
    break;
  }
  default: {
    const char* url = LookupMessage(528);
    const char source[] = {'\x02', '\xCB', '\xF0', '\x43', '\x93'};
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
void nullsub_486(int) {
}

// OFFSET: 0x00D92DA0
// FUNCTION: fn_22295
void CLoginMode::ReceiveRefuseLogin(const RefuseLoginPacket* packet) {
  char text[260];
  memset(text, 0, sizeof(text));
  {
    const char* message = LookupMessage(3202);
    const char source[] = {'\x40', '\x6A', '\x45'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x891B232D09451965ULL>(source), message);
  }

  switch (packet->result) {
  case 1: {
    const char* message = LookupMessage(3196);
    const char source[] = {'\x9C', '\x76', '\x77'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x758F1D0B4F7705B9ULL>(source), message);
    break;
  }
  case 2: {
    const char* message = LookupMessage(3197);
    const char source[] = {'\x50', '\xBE', '\xED'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x35E5656B5DEDCD75ULL>(source), message);
    break;
  }
  case 3: {
    const char* message = LookupMessage(3198);
    const char source[] = {'\xD8', '\xCA', '\xBB'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x4941FD9F21BBB9FDULL>(source), message);
    break;
  }
  case 4: {
    const char* message = LookupMessage(3199);
    const char source[] = {'\xD8', '\xA4', '\x4D'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0xE7978FB5A94DD7FDULL>(source), message);
    break;
  }
  case 5: {
    const char* message = LookupMessage(3200);
    const char source[] = {'\x4E', '\xCA', '\xC7'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0xB7FD8F670FC7B96BULL>(source), message);
    break;
  }
  case 6: {
    const char* message = LookupMessage(3201);
    const char source[] = {'\xAA', '\xEA', '\xEB'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x39FF7FB50DEB998FULL>(source), message);
    break;
  }
  case 7: {
    const char* message = LookupMessage(3214);
    const char source[] = {'\xB2', '\x82', '\xE1'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x2FE993D1DDE1F197ULL>(source), message);
    break;
  }
  case 8:
  case 100:
  case 102: {
    const char* message = LookupMessage(3202);
    const char source[] = {'\xE8', '\x82', '\x99'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0xAD075B8DB599F1CDULL>(source), message);
    break;
  }
  case 9: {
    const char* message = LookupMessage(3911);
    const char source[] = {'\x5A', '\x66', '\x8D'};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), DecryptName<0x2F8B0F37118D157FULL>(source), message);
    break;
  }
  case 101: {
    char blockDate[21] = {};
    memcpy_s(blockDate, 20, packet->blockDate, 20);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(449), blockDate);
    break;
  }
  default:
    break;
  }

  GetRagConnection().Close();
  const int result = packet->result;
  const char source[] = {'\xBE', '\x24', '\x2B', '\xBC', '\x03', '\x2C', '\x05'};
  g_windowMgr.Confirm(FormatString(DecryptName<0x050505679903579BULL>(source), text, result), 0, true, false, 0, "Message", 280, 120, 0);
  g_charServerAccountIdPending = 0;
  this->pendingCursorType = 3;
}

// OFFSET: 0x00D93B10
// FUNCTION: fn_22296
void nullsub_487(int) {
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
  this->unknown_6F74 = true;
  std::string message;
  switch (packet->error) {
  case 0:
    SecureZeroMemory(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText));
    memcpy_s(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText), packet->text,
      std::min<std::size_t>(packet->length - 28, sizeof(g_session.m_loginErrorText)));
    GetRagConnection().Close();
    g_modeMgr.gameMode->SendMsg(10000, 0, 0, 0, 0);
    break;
  case 400:
    if (g_windowMgr.LookupWindow(180)) {
      g_windowMgr.CloseWindow(180);
    }
    g_windowMgr.EnsureWindow(180);
    this->unknown_6F75 = true;
    this->otpStarted = timeGetTime();
    break;
  case 500:
    message = LookupMessage(1752);
    break;
  case 600:
    message = LookupMessage(7);
    break;
  case 700:
    message = LookupMessage(3252);
    break;
  case 900: {
    ShowErrorDialog(GetRenderSystemHolder()->system, LookupMessage(3462), 0);
    const char parametersSource[] = {'\x7F', '\x25', '\x95', '\x31', '\x03', '\xEC', '\xAC', '\xF5', '\x78', '\x7F', '\x86',
                                     '\x2F', '\x53', '\xAC', '\xFA', '\xA9', '\x74', '\x3E', '\x8C', '\x6E', '\x39'};
    const char* parametersText = DecryptName<0x8783C33941E15117ULL>(parametersSource);
    const char fileSource[] = {'\xE3', '\xE9', '\x03', '\xFD', '\xDB', '\xF2', '\x48', '\x65', '\xE4', '\xB3', '\x10',
                               '\xE3', '\x8B', '\xB2', '\x1E', '\x39', '\xE8', '\xF2', '\x1A', '\xA2', '\xE1'};
    const char* fileText = DecryptName<0x1767DDE18D779D8BULL>(fileSource);
    const char operationSource[] = {'\xAE', '\xD3', '\x94', '\xA3', '\x11'};
    ShellExecuteA(nullptr, DecryptName<0xC1BB8111EDD183E1ULL>(operationSource), fileText, parametersText, nullptr, SW_SHOW);
    g_modeMgr.gameMode->SendMsg(2, 0, 0, 0, 0);
    return;
  }
  case 1200:
    message = LookupMessage(4355);
    break;
  default:
    message = LookupMessage(1816);
    break;
  }

  if (!message.empty()) {
    const int error = packet->error;
    const char codeSource[] = {'\xB7', '\xF8', '\xE7', '\x36', '\x77'};
    message += FormatOwnedString(DecryptName<0x271B45771F83DD9FULL>(codeSource), error);
    const char detailSource[] = {'\xAF', '\x02', '\x44', '\x98', '\xE8', '\x19'};
    message += FormatOwnedString(DecryptName<0xC70319B5EB61598FULL>(detailSource), packet->detail);
    g_windowMgr.Confirm(message.c_str(), 5, true, true, 0, "Message", 280, 120, 0);
    if (this->unknown_6F76 != true) {
      if (error == 500) {
        this->otpStarted = timeGetTime();
        if (g_windowMgr.LookupWindow(180)) {
          g_windowMgr.CloseWindow(180);
        }
        g_windowMgr.EnsureWindow(180);
      } else {
        if (g_windowMgr.LookupWindow(180)) {
          g_windowMgr.CloseWindow(180);
        }
        this->unknown_6F74 = false;
        this->unknown_6F75 = false;
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
  this->unknown_6F74 = true;
  std::string message;
  switch (packet->error) {
  case 0:
    SecureZeroMemory(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText));
    memcpy_s(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText), packet->text,
      std::min<std::size_t>(packet->length - 8, sizeof(g_session.m_loginErrorText)));
    GetRagConnection().Close();
    g_modeMgr.gameMode->SendMsg(10000, 0, 0, 0, 0);
    break;
  case 400:
    if (g_windowMgr.LookupWindow(180)) {
      g_windowMgr.CloseWindow(180);
    }
    g_windowMgr.EnsureWindow(180);
    this->unknown_6F75 = true;
    this->otpStarted = timeGetTime();
    break;
  case 500:
    message = LookupMessage(1752);
    break;
  case 600:
    message = LookupMessage(7);
    break;
  case 700:
    message = LookupMessage(3252);
    break;
  case 800:
    message = LookupMessage(3440);
    break;
  default:
    message = LookupMessage(1816);
    break;
  }

  if (!message.empty()) {
    const int error = packet->error;
    const char* caption;
    bool protect;
    if (error == 800) {
      const char captionSource[] = {'\x37', '\x63', '\x53', '\x3B', '\xBC', '\x33', '\xAD'};
      caption = DecryptName<0x75ADC57D87EFBD8FULL>(captionSource);
      protect = true;
    } else {
      const char codeSource[] = {'\x79', '\xAE', '\xA9', '\x28', '\xBB'};
      message += FormatOwnedString(DecryptName<0xD1413DBB01CD8B51ULL>(codeSource), error);
      caption = "Message";
      protect = false;
    }
    g_windowMgr.Confirm(message.c_str(), 5, true, true, 0, caption, 280, 120, protect);
    if (this->unknown_6F76 != true) {
      if (error == 500) {
        this->otpStarted = timeGetTime();
        if (g_windowMgr.LookupWindow(180)) {
          g_windowMgr.CloseWindow(180);
        }
        g_windowMgr.EnsureWindow(180);
      } else {
        if (g_windowMgr.LookupWindow(180)) {
          g_windowMgr.CloseWindow(180);
        }
        this->unknown_6F74 = false;
        this->unknown_6F75 = false;
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
        this->Slot7(this->cursorType);
      }
    }

    if (g_nProtectGameGuardMgr->HasError()) {
      GetRagConnection().Close();
      g_modeMgr.Quit();
      std::string message = g_nProtectGameGuardMgr->ErrorMessage();
      if (!message.empty()) {
        const char source[] = {'\x83', '\x25', '\x13', '\x4C', '\xC3', '\x86', '\x32', '\x59', '\xCD', '\x32', '\x00', '\x4E',
                               '\xD2', '\xA4', '\x24', '\x4C', '\x9F', '\x11', '\x61'};
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
char g_zoneServerIp[16];

// GLOBAL: gl_25079
int g_zoneServerPort;

// GLOBAL: gl_25080
char g_zoneServerHost[129];

// OFFSET: 0x00D929A0
// FUNCTION: fn_22290
void CLoginMode::ConnectZoneServer(std::uint32_t gid, std::string mapName, std::uint32_t ip, int port, std::string host) {
  g_session.m_gid = gid;
  g_modeMgr.gameMode->SendMsg(10009, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0);
  if (g_windowMgr.window_03D4) {
    g_windowMgr.Schedule(g_windowMgr.window_03D4);
  }

  std::string name = mapName;
  name.resize(name.size() - 4);
  g_session.SetNavigationPlayerMap(name.c_str());

  in_addr address;
  address.s_addr = ip;
  strcpy(g_zoneServerIp, inet_ntoa(address));
  g_zoneServerPort = port;
  memcpy_s(g_zoneServerHost, sizeof(g_zoneServerHost), host.c_str(), std::min<std::size_t>(sizeof(g_zoneServerHost), host.size() + 1));

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
  case 0:
    g_windowMgr.CloseWindow(199);
    g_session.m_passwordWndMode = 0;
    this->pendingCursorType = 7;
    break;
  case 1:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 2;
    this->passwordRetryCount = 0;
    this->pendingCursorType = 7;
    break;
  case 2:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 4;
    this->pendingCursorType = 7;
    break;
  case 3:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 3;
    this->pendingCursorType = 7;
    break;
  case 4:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 5;
    this->pendingCursorType = 7;
    break;
  case 5:
    g_windowMgr.CloseWindow(199);
    this->pendingCursorType = 7;
    g_windowMgr.Confirm(LookupMessage(0x768), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.CloseWindow(199);
    this->pendingCursorType = 7;
    g_windowMgr.Confirm(LookupMessage(0x769), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_session.m_passwordWndMode = 1;
    this->pendingCursorType = 7;
    break;
  case 8:
    g_windowMgr.CloseWindow(199);
    if (packet->count != 0) {
      this->pendingCursorType = 7;
      std::string message = FormatOwnedString(LookupMessage(0x764), packet->count);
      g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    } else {
      g_windowMgr.Confirm(LookupMessage(0xD7E), 0, true, false, 0, "Message", 280, 120, 0);
      this->SendMsg(2, 0, 0, 0, 0);
    }
    break;
  case 9: {
    const char* text = LookupMessage(0xC81);
    const char source[] = {'\xF2', '\xE6', '\x37', '\xBE', '\x35', '\x6C', '\x87'};
    g_windowMgr.Confirm(FormatString(DecryptName<0x0B8745519B1F95D7ULL>(source), text, 9), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 10: {
    const char* text = LookupMessage(0xC81);
    const char source[] = {'\xF0', '\x4A', '\x03', '\x6C', '\x83', '\x8E', '\x97'};
    g_windowMgr.Confirm(FormatString(DecryptName<0xA597A7E7492B39D5ULL>(source), text, 10), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 11: {
    const char* text = LookupMessage(0xC81);
    const char source[] = {'\x56', '\x9C', '\x93', '\xF4', '\xB1', '\xCA', '\xAB'};
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
  case 0:
    g_windowMgr.CloseWindow(199);
    g_session.m_passwordWndMode = 0;
    this->pendingCursorType = 7;
    break;
  case 1:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 2;
    this->passwordRetryCount = 0;
    this->pendingCursorType = 7;
    break;
  case 2:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 4;
    this->pendingCursorType = 7;
    break;
  case 3:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 3;
    this->pendingCursorType = 7;
    break;
  case 4:
    g_session.m_passwordWndAccountId = packet->value;
    g_session.m_passwordWndMode = 5;
    this->pendingCursorType = 7;
    break;
  case 5:
    g_windowMgr.CloseWindow(199);
    this->pendingCursorType = 7;
    g_windowMgr.Confirm(LookupMessage(0x768), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.CloseWindow(199);
    this->pendingCursorType = 7;
    g_windowMgr.Confirm(LookupMessage(0x769), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_session.m_passwordWndMode = 1;
    this->pendingCursorType = 7;
    break;
  case 8: {
    g_windowMgr.CloseWindow(199);
    ++this->passwordRetryCount;
    const int remaining = 3 - this->passwordRetryCount;
    this->pendingCursorType = 7;
    if (remaining > 0) {
      std::string message = FormatOwnedString(LookupMessage(0x764), remaining);
      g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    } else {
      g_windowMgr.Confirm(LookupMessage(0x773), 0, true, false, 0, "Message", 280, 120, 0);
      this->SendMsg(2, 0, 0, 0, 0);
    }
    break;
  }
  case 9: {
    const char* text = LookupMessage(0xC81);
    const char source[] = {'\x16', '\x10', '\x7D', '\xD4', '\x4D', '\x1C', '\xD3'};
    g_windowMgr.Confirm(FormatString(DecryptName<0x49D33529F1556333ULL>(source), text, 9), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 10: {
    const char* text = LookupMessage(0xC81);
    const char source[] = {'\x14', '\x8A', '\xFB', '\xDC', '\x79', '\x66', '\xCB'};
    g_windowMgr.Confirm(FormatString(DecryptName<0x01CB4F1DF9D3F931ULL>(source), text, 10), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 11: {
    const char* text = LookupMessage(0xC81);
    const char source[] = {'\xB2', '\xBA', '\xE1', '\xBE', '\xA9', '\x92', '\xB1'};
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
  this->unknown_0070 = 0;
  this->unknown_0074 = 1;
  this->unknown_0078 = false;
  this->wallpaperName = g_loginWallpaperName;
  this->running = 1;
  this->pendingCursorType = -1;
  std::memset(this->billingInfo, 0, sizeof(this->billingInfo));
  this->cursorType = g_loginStartMode;
  this->cursorFrame = 0;
  this->pendingCursorType = -1;
  this->Slot7(g_loginStartMode);
  g_loginStartMode = 3;
  g_isStopByLogin = 1;
  g_session.ClearItemContainers();
  g_guildInfo.Init();
  PreloadCommonSprites();
  if (g_serviceOptions.Get(7)) {
    g_soundMgr->PlayStream("bgm\\01.mp3", 0, false);
  }
  this->slotExtension = CharacterSlotExtension();
  g_session.ClearAllItems1784();
  g_entryQueueMgr->ready = 0;
  g_entryQueueMgr->current = nullptr;
  g_session.ClearShortcutSlots();
  g_session.ClearSecondaryShortcutSlots();
  g_session.unknown_6184 = 0;
  this->unknown_01C4 = 0;
  this->characterCount = 0;
  this->unknown_6F74 = false;
  this->unknown_6F75 = false;
  this->unknown_6F76 = false;
  this->unknown_6F78 = 0;
  g_session.unknown_8074 = 0;
  g_session.unknown_0784.clear();
  g_session.unknown_078C.clear();
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
  g_session.SetPlayerPosition((packet->posDir[0] << 2) | (packet->posDir[1] >> 6), ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4),
    packet->posDir[2] & 0xF);
  g_session.unknown_0F30[0] = packet->font;

  char mapName[36];
  sprintf(mapName, "%s.rsw", g_session.m_navigationPlayerMap);
  g_modeMgr.Switch(1, mapName);
  static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(121), 17, 1);
  GetRagConnection().SetBlock(0);
  g_session.SetOption(17, 0, 0);

  if (g_pingLogEnabled) {
    FILE* file = fopen("PingLog.txt", "at");
    if (file) {
      __time32_t now;
      _time32(&now);
      fprintf(file, "\n");
      fprintf(file, "server:%s\n", g_session.m_accountName.c_str());
      fprintf(file, "ip:%s\n", g_zoneServerIp);
      fprintf(file, "map:%s.gat\n", g_session.m_navigationPlayerMap);
      fprintf(file, "time:%s\n\n", _ctime32(&now));
      fclose(file);
    }
    g_pingLogTicks.clear();
    g_pingLogFirstTick = 0;
  }

  g_session.SetUnknown5C1C(0);
  g_session.unknown_809C.clear();
  for (unsigned int i = 0; i < 68; ++i) {
    std::string name = this->characters[i].name;
    g_session.unknown_809C.push_back(name);
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
  static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(121), 17, 1);
  GetRagConnection().SetBlock(0);

  if (g_pingLogEnabled) {
    FILE* file = fopen("PingLog.txt", "at");
    if (file) {
      __time32_t now;
      _time32(&now);
      fprintf(file, "\n");
      fprintf(file, "server:%s\n", g_session.m_accountName.c_str());
      fprintf(file, "ip:%s\n", g_zoneServerIp);
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
  std::string message;
  if (code != 0x12) {
    GetRagConnection().Close();
  }

  switch (code) {
  case 0x1393:
    message = LookupMessage(0x72F);
    break;
  case 0x1394: {
    const std::string text = FormatOwnedString(LookupMessage(0x337), argument);
    message = text;
    break;
  }
  case 0x1395:
  case 0x13BE:
    message = LookupMessage(0x6FD);
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
      const char source[] = {'\x42', '\x33', '\xC8', '\x47', '\x45'};
      ShellExecuteA(nullptr, DecryptName<0xD98F5345098D630DULL>(source), url.c_str(), url.c_str(), nullptr, SW_SHOWNORMAL);
      message = "";
      g_modeMgr.Quit();
      return;
    }
    message = LookupMessage(0x730);
    break;
  case 0x1399:
    message = LookupMessage(0x731);
    break;
  case 0x13C7:
    message = LookupMessage(0x701);
    break;
  case 0x13C8:
    message = LookupMessage(0x702);
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
    message = LookupMessage(0x72E);
    break;
  case 0x13BA:
    message = LookupMessage(0x726);
    break;
  case 0x13BB:
    message = LookupMessage(5);
    break;
  case 0x14B4:
    message = LookupMessage(0xDCE);
    break;
  case 0x14B5:
    message = LookupMessage(0xDD3);
    break;
  case 33:
    message = LookupMessage(0xA7A);
    break;
  case 0:
    message = LookupMessage(6);
    break;
  case 1:
    if (g_serviceType == 0 && ++g_loginFailCount >= 3) {
      message = std::string(PasswordRetryExceededMessage{}().Text());
      if (g_windowMgr.Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
        if (g_session.m_channelCopId == 1) {
          const char urlSource[] = {'\x17', '\x67', '\xAB', '\xA1', '\xBF', '\x48', '\x56', '\x9A', '\x08', '\x64', '\xF1', '\xB9',
                                    '\xE4', '\x09', '\x1E', '\x8C', '\x12', '\x76', '\xF1', '\xB2', '\xEA', '\x0A', '\x79'};
          const char* file = DecryptName<0xED796785D1DF137FULL>(urlSource);
          const char source[] = {'\xD2', '\xCD', '\x88', '\x59', '\xBB'};
          ShellExecuteA(hWnd, DecryptName<0x83EFBFBB37EDBDBDULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
        } else {
          const char urlSource[] = {'\xF7', '\xB1', '\x19', '\xDB', '\xED', '\x06', '\xD2', '\xD7', '\xFE', '\xA2', '\x03',
                                    '\xCA', '\xA5', '\x46', '\x96', '\x8B', '\xFC', '\xAA', '\x43', '\xC0', '\xA5', '\x29'};
          const char* file = DecryptName<0xA5FD29D7AB6DC59FULL>(urlSource);
          const char source[] = {'\x66', '\x57', '\x4A', '\xD5', '\xBD'};
          ShellExecuteA(hWnd, DecryptName<0xD97D5DBDBB2F2709ULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
        }
        g_modeMgr.Quit();
      }
      message.clear();
    } else {
      message = LookupMessage(0x10B);
    }
    break;
  case 2:
    message = LookupMessage(8);
    break;
  case 4:
  case 0x13C6:
    message = LookupMessage(0x10A);
    break;
  case 5:
    message = LookupMessage(0x136);
    break;
  case 99:
    message = LookupMessage(0x170);
    break;
  case 6: {
    const std::string text = FormatOwnedString(LookupMessage(0x1C1), argument);
    message = text;
    break;
  }
  case 101: {
    const std::string text = FormatOwnedString(LookupMessage(0x32A), argument);
    message = text;
    break;
  }
  case 102:
    message = LookupMessage(0x32B);
    break;
  case 103:
    message = LookupMessage(0x35B);
    break;
  case 104:
    message = LookupMessage(0x35C);
    break;
  case 7:
    message = LookupMessage(0x1B7);
    break;
  case 8:
    message = LookupMessage(0x2A9);
    break;
  case 9:
    message = LookupMessage(0x2BF);
    break;
  case 10:
    message = LookupMessage(0x2C0);
    break;
  case 11:
    message = LookupMessage(0x2C1);
    break;
  case 12:
    message = LookupMessage(0x2C2);
    break;
  case 13:
    message = LookupMessage(0x2C3);
    break;
  case 14:
    message = LookupMessage(0x2C4);
    break;
  case 15:
    message = LookupMessage(0x2C5);
    break;
  case 100: {
    const std::string text = FormatOwnedString(LookupMessage(0x329), argument);
    message = text;
    break;
  }
  case 16:
    if (g_serviceType == 0) {
      const char textSource[] = {'\x4B', '\x36', '\x80', '\xD9', '\x37', '\xC0', '\xB7', '\x39', '\x52', '\xE7', '\x83', '\xA6', '\xA7', '\x9F', '\x6F', '\x39',
                                 '\x35', '\xEB', '\x19', '\xBD', '\xCE', '\xC4', '\x8C', '\xD1', '\x30', '\x65', '\x85', '\xDA', '\x37', '\xBE', '\x97', '\x30',
                                 '\x27', '\x7A', '\xFA', '\xB8', '\xF5', '\x59', '\xF6', '\x28', '\x47', '\x3F', '\x8D', '\xC6', '\xA3', '\xA0', '\x61', '\xF1'};
      message = std::string(DecryptName<0xF14F79170939C7F1ULL>(textSource));
      if (g_windowMgr.Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
        const char urlSource[] = {'\x43', '\x6B', '\x9D', '\xDB', '\xFB', '\xD0', '\x02', '\x08', '\x5C', '\x68', '\xC7', '\xD9', '\xA0', '\x98', '\x43', '\x1E',
                                  '\x59', '\x70', '\x82', '\x85', '\xA2', '\x90', '\x03', '\x14', '\x59', '\x30', '\x9A', '\xCE', '\xB5', '\xB1', '\x48', '\x08',
                                  '\x7B', '\x48', '\xC6', '\xDB', '\xA0', '\x8C', '\x5E', '\x08', '\x44', '\x6D', '\x8D', '\xF4', '\xA2', '\x97', '\x4C', '\x11',
                                  '\x4C', '\x7A', '\xB6', '\xCA', '\xA6', '\x8D', '\x48', '\x1A', '\x46', '\x7A', '\x87', '\xDF', '\xEF', '\x9E', '\x5E', '\x0F',
                                  '\x2B'};
        const char* file = DecryptName<0x7F2DFFC1ABE91F2BULL>(urlSource);
        const char source[] = {'\xE2', '\xF1', '\x74', '\x9D', '\xB3'};
        ShellExecuteA(hWnd, DecryptName<0xFD69A9B3F311818DULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
        g_modeMgr.Quit();
      }
      message.clear();
    }
    break;
  case 106:
    message = LookupMessage(0x50D);
    break;
  case 105:
    message = LookupMessage(0x55C);
    break;
  case 17: {
    const std::string text = FormatOwnedString(LookupMessage(0x564), argument);
    message = text;
    if (g_windowMgr.Confirm(message.c_str(), 2, true, true, 0, "Message", 280, 120, 0) == 0xBB) {
      g_modeMgr.Quit();
    }
    message.clear();
    break;
  }
  case 18:
    message = LookupMessage(0x565);
    break;
  case 19:
    message = LookupMessage(0x6D7);
    break;
  case 20:
    message = LookupMessage(0x6D8);
    break;
  case 108:
    message = LookupMessage(0x571);
    break;
  case 109:
    message = LookupMessage(0x572);
    break;
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 27:
  case 30:
    message = LookupMessage(0xD70);
    break;
  case 28:
    message = LookupMessage(0xAAB);
    break;
  case 29:
    message = LookupMessage(0xAAC);
    break;
  case 21:
  case 0x139A:
    message = LookupMessage(0x6FA);
    break;
  case 110:
    if (g_windowMgr.Confirm(LookupMessage(0x929), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
      const char* file = LookupMessage(0x92A);
      const char source[] = {'\xFC', '\x05', '\xBA', '\x81', '\x4D'};
      ShellExecuteA(hWnd, DecryptName<0x9B0F234DEFDF7593ULL>(source), file, nullptr, nullptr, SW_SHOWNORMAL);
      g_modeMgr.Quit();
    }
    message = "";
    break;
  case 112:
    message = LookupMessage(0x96A);
    break;
  case 242:
    message = LookupMessage(0xD6F);
    break;
  case 36: {
    const int remaining = this->unknown_6F78;
    const std::string text = FormatOwnedString(LookupMessage(0xD7D), remaining / 3600, static_cast<int>(std::ceil(static_cast<float>(remaining % 3600) / 60.0f)));
    message = text;
    break;
  }
  case 243:
    message = LookupMessage(0xE67);
    break;
  case 244:
    message = LookupMessage(0x105E);
    break;
  default: {
    message = LookupMessage(9);
    const char source[] = {'\x6B', '\x1C', '\x3F', '\x42', '\x67'};
    message += FormatOwnedString(DecryptName<0xC9712D676B5B3943ULL>(source), code);
    break;
  }

  }

  if (!message.empty()) {
    g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    if (code == 5 || code == 99) {
      this->SendMsg(2, 0, 0, 0, 0);
    }
  }

  if (g_loginModeFlag == true) {
    this->pendingCursorType = 3;
  } else {
    g_modeMgr.Quit();
  }
}

// OFFSET: 0x00D8C6A0
// FUNCTION: fn_22258
CLoginMode::CLoginMode() : wallpaperName(), unknown_6F1C(), unknown_6F2C(), unknown_6F38(), unknown_6F50(), mapStates() {
  this->fadeInRequested = 1;
  this->unknown_6F10 = timeGetTime() + 12000;
  this->savedInlineCharset = g_inlineCharset;
  g_inlineCharset = 0;
  this->unknown_6F48 = -1;
  this->window_6F14 = nullptr;
  this->passwordRetryCount = 0;
  SecureZeroMemory(this->characters, sizeof(this->characters));
  this->unknown_01D8 = 0;
  this->unknown_01DC = 0;
  this->characterDeleted = false;
}

// OFFSET: 0x00D92B80
// FUNCTION: fn_22293
bool CLoginMode::ShowNotice(int notice, bool append) {
  UIWindow* window;
  if (append) {
    window = g_windowMgr.unknown_0210;
  } else {
    this->unknown_6F1C.clear();
    if (g_windowMgr.unknown_0210) {
      g_windowMgr.CloseWindow(18);
    }
    window = g_windowMgr.EnsureWindow(18);
  }

  if (!window) {
    return false;
  }

  switch (notice) {
  case 45:
    window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(LookupMessage(2163)), 0, 0, 0);
    window->SendMsg(nullptr, 30, 0, 0, 0, 0);
    this->unknown_6F1C.push_back(45);
    break;
  case 46:
    window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(LookupMessage(2164)), 0, 0, 0);
    window->SendMsg(nullptr, 30, 0, 0, 0, 0);
    this->unknown_6F1C.push_back(46);
    break;
  case 47:
    window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(LookupMessage(2165)), 0, 0, 0);
    window->SendMsg(nullptr, 30, 0, 0, 0, 0);
    this->unknown_6F1C.push_back(47);
    break;
  }

  if (this->unknown_6F1C.empty()) {
    return false;
  }

  if (UIWindow* notices = g_windowMgr.unknown_0210) {
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
  *reinterpret_cast<std::string*>(&g_session.unknown_56B8[4]) = LookupMessage(940);

  SessionAccount& account = *reinterpret_cast<SessionAccount*>(g_session.unknown_0E9C);
  account.authCode = packet->authCode;
  account.userLevel = packet->userLevel;
  g_session.m_aid = packet->aid;

  unsigned char digest[16] = {};
  ComputeLoginDigest(digest, packet, offsetof(AcceptLoginPacket, body));
  g_pakRegistry.ApplyAuxiliary(digest, sizeof(digest));
  SecureZeroMemory(digest, sizeof(digest));

  if (packet->sex >= 10) {
    g_session.SetFlag(packet->sex - 10);
    g_doramSex = packet->sex - 10;
  } else {
    if (g_loginExitFlag == 1) {
      exit(0);
    }
    g_session.SetFlag(packet->sex);
    g_doramSex = packet->sex;
  }

  if (g_serviceType == 6 || g_serviceType == 10 || g_serviceType == 12) {
    g_serviceOptions.Set(0x6C, 0);
  }
  const unsigned char flags = g_serviceOptions.Get(0x6C);
  SetServiceFlags(flags, flags);

  account.lastLoginIp = packet->lastLoginIp;
  memcpy_s(account.lastLoginTime, sizeof(packet->lastLoginTime), packet->lastLoginTime, sizeof(packet->lastLoginTime));
  account.lastLoginTime[26] = '\0';

  unsigned int count;
  switch (version) {
  case 2:
    count = (packet->length - (offsetof(AcceptLoginPacket, body) + 4)) / sizeof(ServerAddress);
    this->unknown_01C4 = count;
    memcpy(this->serverList, packet->body + 4, count * sizeof(ServerAddress));
    break;
  case 3:
    count = (packet->length - (offsetof(AcceptLoginPacket, body) + 17)) / sizeof(ServerAddress);
    this->unknown_01C4 = count;
    memcpy(this->serverList, packet->body + 17, count * sizeof(ServerAddress));
    memcpy(&g_session.unknown_807C[8], packet->body, 16);
    break;
  case 4:
    count = (packet->length - offsetof(AcceptLoginPacket, body)) / sizeof(ServerAddress2);
    this->unknown_01C4 = count;
    memcpy(this->serverList, packet->body, count * sizeof(ServerAddress2));
    break;
  case 5:
    count = (packet->length - (offsetof(AcceptLoginPacket, body) + 17)) / sizeof(ServerAddress2);
    this->unknown_01C4 = count;
    memcpy(this->serverList, packet->body + 17, count * sizeof(ServerAddress2));
    memcpy(&g_session.unknown_807C[8], packet->body, 16);
    break;
  case 6:
  case 7:
  case 8:
  case 9:
    break;
  default:
    count = (packet->length - offsetof(AcceptLoginPacket, body)) / sizeof(ServerAddress);
    this->unknown_01C4 = count;
    memcpy(this->serverList, packet->body, count * sizeof(ServerAddress));
    break;
  }

  CNProtectGameGuardMgr* gameGuard = g_nProtectGameGuardMgr;
  const std::uint32_t aid = packet->aid;
  const std::uint8_t source[] = {0x4C, 0x7B, 0x77};
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
    static_cast<CMode*>(g_charViewMode)->SendMsg(1, 0, 0, 0, 0);
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
    case 0x69:
    case 0x6D:
    case 0x187:
    case 0x283:
    case 0x8E3:
    case 0x8E4:
    case 0x99D:
    case 0x9C2:
    case 0xA4D:
    case 0xA73:
    case 0xA79:
    case 0xA7B:
    case 0xAE0:
    case 0xC20:
      break;
    case 0x6A: {
      RefuseLoginPacket* packet = reinterpret_cast<RefuseLoginPacket*>(g_gameModePacketBuffer);
      packet->blockDate[19] = '\0';
      this->ShowLoginError(packet->result, reinterpret_cast<int>(packet->blockDate));
      break;
    }
    case 0x6B:
      this->ReceiveAcceptEnter(reinterpret_cast<const AcceptEnterPacket*>(g_gameModePacketBuffer));
      break;
    case 0x6C:
      GetRagConnection().Close();
      switch (reinterpret_cast<const ResultPacket*>(g_gameModePacketBuffer)->result) {
      case 1:
        g_windowMgr.Confirm(LookupMessage(0xD0), 0, true, true, 0, "Message", 280, 120, 0);
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
        break;
      case 0x6F:
        g_windowMgr.Confirm(LookupMessage(0x749), 0, true, false, 0, "Message", 280, 120, 0);
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
        break;
      case 0x76:
        g_windowMgr.Confirm(LookupMessage(0xEEA), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 6;
        break;
      case 0x77:
        g_windowMgr.Confirm(LookupMessage(0xB71), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 6;
        break;
      default:
        g_windowMgr.Confirm(LookupMessage(9), 0, true, false, 0, "Message", 280, 120, 0);
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
        break;
      }
      break;
    case 0x6E: {
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
    case 0x6F: {
      CharacterInfo* remaining = new CharacterInfo[this->characterCount];
      long long kept = 0;
      for (long long i = 0; i < this->characterCount; ++i) {
        if (this->characters[i].slot != static_cast<unsigned char>(g_selectedSlotIndex)) {
          memcpy_s(&remaining[kept], sizeof(CharacterInfo), &this->characters[i], sizeof(CharacterInfo));
          ++kept;
        }
      }
      --this->characterCount;
      --g_session.unknown_59BC;
      memcpy_s(this->characters, sizeof(this->characters), remaining, this->characterCount * sizeof(CharacterInfo));
      delete[] remaining;
      this->pendingCursorType = 7;
      break;
    }
    case 0x70: {
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
    case 0x71:
      this->ReceiveNotifyZoneServer(reinterpret_cast<const NotifyZoneServerPacket*>(g_gameModePacketBuffer));
      break;
    case 0x73:
      this->Zc_Accept_Enter2(reinterpret_cast<const AcceptEnterPositionPacket*>(g_gameModePacketBuffer));
      return;
    case 0x74:
      g_windowMgr.Confirm(LookupMessage(9), 0, true, false, 0, "Message", 280, 120, 0);
      GetRagConnection().Close();
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
      break;
    case 0x81:
      this->ReceiveNotifyBan(g_gameModePacketBuffer);
      break;
    case 0x1B5:
      this->ReceiveBillingInfo(g_gameModePacketBuffer);
      break;
    case 0x1BE:
      if (g_windowMgr.Confirm(LookupMessage(0x2B3), 2, true, false, 0, "Message", 280, 120, 0) == 0xBB) {
        GameRoomReplyPacket reply;
        reply.id = 0x1BF;
        reply.permission = 1;
        GetRagConnection().Send(g_packetLengths.FixedLength(reply.id), &reply);
      } else {
        GetRagConnection().Close();
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
      break;
    case 0x1C7: {
      const std::uint8_t source[] = {0x17, 0x62, 0xEB, 0xFF, 0x1C, 0x00, 0xFE, 0xD4, 0x2D, 0x2D, 0x8E, 0xC1, 0x1E, 0x11, 0xEC, 0xC1,
                                     0x2D, 0x64, 0xA4, 0xC1, 0x5D, 0x42, 0xB4, 0x90, 0x59};
      thread_local EncryptedName<sizeof(source), 0xB195637DAFCB0D59ULL> text(source);
      ShowErrorDialog(GetRenderSystemHolder()->system, text.Text(), 0);
      break;
    }
    case 0x1F1: {
      const LoginMessagePacket* packet = reinterpret_cast<const LoginMessagePacket*>(g_gameModePacketBuffer);
      const std::string message(packet->text, packet->length - offsetof(LoginMessagePacket, text));
      g_windowMgr.Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0);
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
      break;
    }
    case 0x20D: {
      const BlockedCharacterPacket* packet = reinterpret_cast<const BlockedCharacterPacket*>(g_gameModePacketBuffer);
      BlockedCharacter* blocked = reinterpret_cast<BlockedCharacter*>(&g_session.unknown_07F4[0x40]);
      g_session.unknown_59D0 = (packet->length - offsetof(BlockedCharacterPacket, characters)) / sizeof(BlockedCharacter);
      memset(blocked, 0, g_session.unknown_59D0 * sizeof(BlockedCharacter));
      memcpy(blocked, packet->characters, g_session.unknown_59D0 * sizeof(BlockedCharacter));
      break;
    }
    case 0x23D:
      g_windowMgr.Confirm(LookupMessage(0x3FC), 0, true, false, 0, "Message", 280, 120, 0);
      break;
    case 0x28B: {
      const AcceptEnterNeoUnionPacket* packet = reinterpret_cast<const AcceptEnterNeoUnionPacket*>(g_gameModePacketBuffer);
      this->slotExtension = packet->extension;
      this->slotExtension.unknown_0001 = htonl(this->slotExtension.unknown_0001);
      this->slotExtension.unknown_0005 = htonl(this->slotExtension.unknown_0005);
      this->slotExtension.unknown_0009 = htonl(this->slotExtension.unknown_0009);

      const unsigned int dataLength = packet->length - offsetof(AcceptEnterNeoUnionPacket, characters);
      this->characterCount = dataLength / sizeof(CharacterInfo);
      g_session.unknown_0E94 = this->characterCount;
      const unsigned char* characters = reinterpret_cast<const unsigned char*>(packet->characters) + dataLength % sizeof(CharacterInfo);
      memcpy_s(this->characters, sizeof(this->characters), characters, this->characterCount * sizeof(CharacterInfo));
      this->pendingCursorType = 19;
      break;
    }
    case 0x28E: {
      const ShortResultPacket* packet = reinterpret_cast<const ShortResultPacket*>(g_gameModePacketBuffer);
      if (packet->result != 0) {
        char text[300];
        sprintf(text, LookupMessage(0x4BD), reinterpret_cast<const char*>(&g_session.unknown_5913[5]));
        if (g_windowMgr.Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) == 0xBB) {
          ChangeCharacterNamePacket request;
          request.id = 0x28F;
          request.gid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5913[1]);
          GetRagConnection().Send(g_packetLengths.FixedLength(request.id), &request);
          break;
        }
      } else if (g_windowMgr.Confirm(LookupMessage(10), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
        break;
      }

      if (UIWindow* window = g_windowMgr.window_03D4) {
        window->SendMsg(nullptr, 6, 0x15F, 0, 0, 0);
      }
      break;
    }
    case 0x290:
      this->ReceiveCharacterResult(reinterpret_cast<const CharacterResultPacket*>(g_gameModePacketBuffer));
      break;
    case 0x2CA:
      switch (reinterpret_cast<const ResultPacket*>(g_gameModePacketBuffer)->result) {
      case 0:
        g_windowMgr.Confirm(LookupMessage(0x546), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 1:
        g_windowMgr.Confirm(LookupMessage(0x547), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 2:
        g_windowMgr.Confirm(LookupMessage(0x548), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 3:
        g_windowMgr.Confirm(LookupMessage(0x549), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 4:
        g_windowMgr.Confirm(LookupMessage(0x54A), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 5:
        g_windowMgr.Confirm(LookupMessage(0x554), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      this->pendingCursorType = GetRagConnection().NativeSocket() != INVALID_SOCKET ? 7 : 3;
      break;
    case 0x2EB:
      this->Zc_Accept_Enter3(reinterpret_cast<const AcceptEnterPositionPacket*>(g_gameModePacketBuffer));
      return;
    case 0x448:
      this->ReceiveCharacterList(reinterpret_cast<const CharacterListPacket*>(g_gameModePacketBuffer));
      break;
    case 0x828:
      this->ReceiveDeleteReservation(reinterpret_cast<const CharacterDeletePacket*>(g_gameModePacketBuffer));
      break;
    case 0x82A:
      this->ReceiveDeleteCharacter(reinterpret_cast<const CharacterDeletePacket*>(g_gameModePacketBuffer));
      break;
    case 0x82C:
      this->ReceiveDeleteCancel(reinterpret_cast<const CharacterDeletePacket*>(g_gameModePacketBuffer));
      break;
    case 0x82D:
      this->ReceiveCharacterSlots(reinterpret_cast<const CharacterSlotsPacket*>(g_gameModePacketBuffer));
      break;
    case 0x83E: {
      LoginErrorCodePacket* packet = reinterpret_cast<LoginErrorCodePacket*>(g_gameModePacketBuffer);
      packet->text[19] = '\0';
      this->ShowLoginError(packet->code, reinterpret_cast<int>(packet->text));
      break;
    }
    case 0x840:
      this->ReceiveMapStates(reinterpret_cast<const MapStatePacket*>(g_gameModePacketBuffer));
      break;
    case 0x851:
      IgnoreRemovedPacket(g_gameModePacketBuffer);
      break;
    case 0x8B9:
      this->SubD911C0(reinterpret_cast<const PasswordModePacket*>(g_gameModePacketBuffer));
      break;
    case 0x8BB: {
      const SecondPasswordResultPacket* packet = reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer);
      g_session.m_loginAccountId = packet->accountId;
      switch (packet->result) {
      case 0:
        g_windowMgr.CloseWindow(200);
        g_windowMgr.Confirm(LookupMessage(0x761), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 1:
        g_windowMgr.CloseWindow(200);
        g_windowMgr.Confirm(LookupMessage(0x76A), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 2:
        g_windowMgr.CloseWindow(200);
        g_windowMgr.Confirm(LookupMessage(0x768), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 3:
        g_windowMgr.CloseWindow(200);
        g_windowMgr.Confirm(LookupMessage(0x769), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 4:
        g_windowMgr.CloseWindow(200);
        g_windowMgr.Confirm(LookupMessage(0x765), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    }
    case 0x8BD: {
      const SecondPasswordResultPacket* packet = reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer);
      g_session.m_loginAccountId = packet->accountId;
      switch (packet->result) {
      case 0:
        g_windowMgr.CloseWindow(200);
        g_windowMgr.Confirm(LookupMessage(0x762), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 1:
        g_windowMgr.CloseWindow(200);
        g_windowMgr.Confirm(LookupMessage(0x766), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    }
    case 0x8BF: {
      const SecondPasswordResultPacket* packet = reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer);
      g_session.m_loginAccountId = packet->accountId;
      switch (packet->result) {
      case 0:
        g_windowMgr.CloseWindow(199);
        g_windowMgr.Confirm(LookupMessage(0x763), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      case 1:
        g_windowMgr.CloseWindow(199);
        g_windowMgr.Confirm(LookupMessage(0x767), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 2:
        g_windowMgr.CloseWindow(199);
        g_windowMgr.Confirm(LookupMessage(0x768), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      case 3:
        g_windowMgr.CloseWindow(199);
        g_windowMgr.Confirm(LookupMessage(0x769), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    }
    case 0x8C4: {
      const SecondPasswordResultPacket* packet = reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer);
      g_session.m_loginAccountId = packet->accountId;
      switch (packet->result) {
      case 0:
        g_windowMgr.CloseWindow(199);
        g_windowMgr.Confirm(LookupMessage(0x76D), 0, true, false, 0, "Message", 280, 120, 0);
        static_cast<UINewSelectCharWnd*>(g_windowMgr.window_03D4)->EnableButtonById(0x1A2);
        g_session.m_passwordWndMode = 1;
        break;
      case 1:
        g_windowMgr.CloseWindow(199);
        g_windowMgr.Confirm(LookupMessage(0x76E), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    }
    case 0x8C6:
      switch (reinterpret_cast<const SecondPasswordResultPacket*>(g_gameModePacketBuffer)->result) {
      case 0:
        g_windowMgr.CloseWindow(199);
        g_windowMgr.Confirm(LookupMessage(0x76F), 0, true, false, 0, "Message", 280, 120, 0);
        static_cast<UINewSelectCharWnd*>(g_windowMgr.window_03D4)->DisableButtonById(0x1A2);
        break;
      case 1:
        g_windowMgr.CloseWindow(199);
        g_windowMgr.Confirm(LookupMessage(0x770), 0, true, false, 0, "Message", 280, 120, 0);
        this->pendingCursorType = 7;
        break;
      }
      break;
    case 0x8D5:
    case 0xB70:
      if (reinterpret_cast<const SecondPasswordStatePacket*>(g_gameModePacketBuffer)->result == 1) {
        g_windowMgr.Confirm(LookupMessage(0x7E1), 0, true, false, 0, "Message", 280, 120, 0);
      }
      break;
    case 0x8FD:
      this->SubD8D500(reinterpret_cast<const char*>(g_gameModePacketBuffer));
      break;
    case 0x986: {
      const ShutdownNoticePacket* packet = reinterpret_cast<const ShutdownNoticePacket*>(g_gameModePacketBuffer);
      __time32_t endTime = packet->endTime;
      const __time32_t startTime = packet->startTime;
      const tm when = *_localtime32(&endTime);
      char text[0x400];
      memset(text, 0, sizeof(text));
      _snprintf(text, sizeof(text), LookupMessage(0x92C), when.tm_year + 1900, when.tm_mon + 1, when.tm_mday, when.tm_hour, when.tm_min, when.tm_sec);
      g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
      const std::uint64_t timestamp = static_cast<std::int64_t>(startTime) * 10000000;
      const std::int64_t interval = 3600000;
      CPartialShutDownSchedule* schedule = new CPartialShutDownSchedule(timestamp, interval, 1);
      schedule->SetPeriod(endTime, startTime);
      g_timeScheduler->AddSchedule(schedule);
      break;
    }
    case 0x9A0:
      for (int i = 1; i <= reinterpret_cast<const IntValuePacket*>(g_gameModePacketBuffer)->value; ++i) {
        std::int16_t request = 0x9A1;
        GetRagConnection().Send(sizeof(request), &request);
      }
      break;
    case 0x9A2:
      this->unknown_6F28 = reinterpret_cast<const IntValuePacket*>(g_gameModePacketBuffer)->value;
      this->pendingCursorType = 24;
      break;
    case 0x9A5: {
      const LoginErrorTimePacket* packet = reinterpret_cast<const LoginErrorTimePacket*>(g_gameModePacketBuffer);
      if (packet->code != 0x12) {
        GetRagConnection().Close();
      }
      this->unknown_6F78 = packet->remaining;
      this->ShowLoginError(packet->code, reinterpret_cast<int>(""));
      break;
    }
    case 0xAC4:
      this->ReceiveAcceptLogin(reinterpret_cast<const AcceptLoginPacket*>(g_gameModePacketBuffer), 5);
      break;
    case 0xAC5:
      this->ReceiveNotifyZoneServer2(reinterpret_cast<const NotifyZoneServer2Packet*>(g_gameModePacketBuffer));
      break;
    case 0xAC9:
      this->ReceiveAcceptLogin(reinterpret_cast<const AcceptLoginPacket*>(g_gameModePacketBuffer), 4);
      break;
    case 0xACA: {
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
      const std::uint8_t source[] = {0xC4, 0x08, 0xD1, 0xF2, 0xB1, 0x3A, 0x39};
      thread_local EncryptedName<sizeof(source), 0xDD3913D5D7F97BE1ULL> format(source);
      g_windowMgr.Confirm(FormatString(format.Text(), text, code), 0, true, false, 0, "Message", 280, 120, 0);
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
      break;
    }
    case 0xACD:
      this->ReceiveRefuseLogin(reinterpret_cast<const RefuseLoginPacket*>(g_gameModePacketBuffer));
      break;
    case 0xAD1:
      this->ReceiveLoginError(reinterpret_cast<const LoginErrorPacket*>(g_gameModePacketBuffer));
      break;
    case 0xADE: {
      const int value = reinterpret_cast<const IntValuePacket*>(g_gameModePacketBuffer)->value;
      if (value > 0) {
        g_unknown173A5BC = value;
      }
      break;
    }
    case 0xAE3:
      this->ReceiveLoginErrorDetail(reinterpret_cast<const LoginErrorDetailPacket*>(g_gameModePacketBuffer));
      break;
    case 0xAE9:
      this->SubD90C90(reinterpret_cast<const PasswordModePacket*>(g_gameModePacketBuffer));
      break;
    case 0xB02: {
      const LoginErrorCodePacket* packet = reinterpret_cast<const LoginErrorCodePacket*>(g_gameModePacketBuffer);
      const std::string text(packet->text, sizeof(packet->text));
      this->ShowLoginError(packet->code, reinterpret_cast<int>(text.c_str()));
      break;
    }
    case 0xB07:
      this->ReceiveAcceptLogin(reinterpret_cast<const AcceptLoginPacket*>(g_gameModePacketBuffer), 6);
      break;
    case 0xB18: {
      const std::int16_t slots = reinterpret_cast<const ShortResultPacket*>(g_gameModePacketBuffer)->result;
      if (slots <= 100) {
        g_session.unknown_7FBC = slots;
      }
      break;
    }
    case 0xB6F: {
      const CharacterInfoPacket* packet = reinterpret_cast<const CharacterInfoPacket*>(g_gameModePacketBuffer);
      g_charInfo = packet->info;
      g_selectedSlotIndex = static_cast<char>(g_charInfo.slot);
      memcpy_s(&this->characters[this->characterCount], sizeof(CharacterInfo), &packet->info, sizeof(CharacterInfo));
      char* mapName = this->characters[this->characterCount].mapName;
      StringFormat(mapName, sizeof(this->characters[this->characterCount].mapName), static_cast<std::size_t>(-1), "%s", g_session.GatName2RswName(mapName));
      ++this->characterCount;
      ++g_session.unknown_59BC;
      this->pendingCursorType = 7;
      break;
    }
    case 0xB71: {
      const CharacterInfoPacket* packet = reinterpret_cast<const CharacterInfoPacket*>(g_gameModePacketBuffer);
      CharacterInfo* character = reinterpret_cast<CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, packet->info.slot, 0, 0, 0));
      if (character && character->gid == *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5913[1])) {
        *character = packet->info;
        StringFormat(character->mapName, sizeof(character->mapName), static_cast<std::size_t>(-1), "%s", g_session.GatName2RswName(character->mapName));
        this->pendingCursorType = 7;
      }
      break;
    }
    case 0xB72:
      this->ReceiveCharacterPage(reinterpret_cast<const CharacterPagePacket*>(g_gameModePacketBuffer));
      break;
    case 0xC05: {
      const CharacterSlotCountsPacket* packet = reinterpret_cast<const CharacterSlotCountsPacket*>(g_gameModePacketBuffer);
      g_session.unknown_8130 = packet->counts[1];
      CharacterSlotsPacket slots;
      slots.id = 0x82D;
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
  case 0:
    switch (argument1) {
    case 10001:
      this->pendingCursorType = 8;
      return 0;
    case 10002:
      this->pendingCursorType = 9;
      return 0;
    case 10004:
      this->pendingCursorType = 7;
      return 0;
    case 10006:
      if (this->cursorType == 7) {
        g_windowMgr.Shutdown();
        GetRagConnection().Close();
        this->pendingCursorType = 6;
      }
      return 0;
    case 10010:
      this->pendingCursorType = 11;
      return 0;
    default:
      return 0;
    }
  case 1:
    GetRagConnection().Close();
    if (g_windowMgr.Confirm(LookupMessage(2), 0, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
    }
    return 0;
  case 8:
    for (int i = 0; i < this->characterCount; ++i) {
      if (this->characters[i].slot == argument1) {
        return reinterpret_cast<int>(&this->characters[i]);
      }
    }
    return 0;
  case 23: {
    unsigned char* buffer = static_cast<unsigned char*>(
      _MallocaComputeSize(0x14) != 0
        ? (_MallocaComputeSize(0x14) <= _ALLOCA_S_THRESHOLD
            ? _MarkAllocaS(_alloca(_MallocaComputeSize(0x14)), _ALLOCA_S_STACK_MARKER)
            : _MarkAllocaS(std::malloc(_MallocaComputeSize(0x14)), _ALLOCA_S_HEAP_MARKER))
        : nullptr
    );
    if (buffer) {
      *reinterpret_cast<std::int16_t*>(buffer) = 0x9A3;
      *reinterpret_cast<int*>(buffer + 4) = this->unknown_6F28;
      StringFormat(reinterpret_cast<char*>(buffer + 8), 10, static_cast<std::size_t>(-1), reinterpret_cast<const char*>(&this->unknown_0138[0x80]));
      *reinterpret_cast<std::int16_t*>(buffer + 2) = 14;
      GetRagConnection().Send(*reinterpret_cast<std::int16_t*>(buffer + 2), buffer);
      this->window_6F14 = g_windowMgr.EnsureWindow(6);
      static_cast<UIWaitWnd*>(this->window_6F14)->SetMessage(LookupMessage(122), 16, 1);
      _freea(buffer);
    }
    return 0;
  }
  case 24:
    switch (this->unknown_6F1C[argument1]) {
    case 45:
      if (UIWindow* window = g_windowMgr.LookupWindow(0x115)) {
        window->SendMsg(nullptr, 6, 0x1B0, 0, 0, 0);
      }
      return 0;
    case 46:
      if (UIWindow* window = g_windowMgr.LookupWindow(0x115)) {
        window->SendMsg(nullptr, 6, 0x1B1, 0, 0, 0);
      }
      return 0;
    case 47:
      if (UIWindow* window = g_windowMgr.LookupWindow(0x115)) {
        window->SendMsg(nullptr, 6, 0x15F, 0, 0, 0);
      }
      return 0;
    default:
      return 0;
    }
  case 28:
    g_charServerAccountIdPending = 0;
    this->pendingCursorType = 3;
    return 0;
  case 39: {
    struct PacketAD0 {
      /* 0000 */ std::int16_t id;
      /* 0002 */ char code[9];
    };
    PacketAD0 packet;
    packet.id = 0xAD0;
    memcpy_s(packet.code, sizeof(packet.code), &this->unknown_0138[0x80], sizeof(packet.code));
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    this->window_6F14 = g_windowMgr.EnsureWindow(6);
    static_cast<UIWaitWnd*>(this->window_6F14)->SetMessage(LookupMessage(122), 16, 1);
    return 0;
  }
  case 258:
    if (this->characterCount == 0) {
      return 0;
    }
    return reinterpret_cast<int>(&this->characters[this->characterCount - 1]);
  case 10000:
    this->pendingCursorType = 4;
    return 0;
  case 10003: {
    *reinterpret_cast<int*>(this->unknown_01C8) = argument1;
    if (argument1 == -1) {
      return 0;
    }

    ServerAddress2* servers = reinterpret_cast<ServerAddress2*>(this->serverList);
    if (servers[argument1].address.state == 3) {
      g_windowMgr.Confirm(LookupMessage(0x2FC), 0, true, false, 0, "Message", 280, 120, 0);
      this->pendingCursorType = 6;
      return 0;
    }

    g_session.m_accountName = servers[argument1].address.name;
    const int selected = *reinterpret_cast<int*>(this->unknown_01C8);
    const std::uint32_t ip = servers[selected].address.ip;
    const std::uint8_t ipFormatSource[] = {0xB0, 0xEB, 0xCF, 0x28, 0x39, 0x27, 0x44, 0x95, 0xBB, 0xAA, 0x85, 0x0D};
    thread_local EncryptedName<sizeof(ipFormatSource), 0xF161095D0DE18F95ULL> ipFormat(ipFormatSource);
    CharServerEndpoint endpoint;
    wsprintfA(endpoint.ip, ipFormat.Text(), ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, ip >> 24);
    const int selected2 = *reinterpret_cast<int*>(this->unknown_01C8);
    endpoint.port = servers[selected2].address.port;
    const std::uint8_t hostFormatSource[] = {0xFC, 0x8A, 0xE1};
    thread_local EncryptedName<sizeof(hostFormatSource), 0x2BAD156DE3E1F9D9ULL> hostFormat(hostFormatSource);
    StringFormat(endpoint.host, sizeof(endpoint.host), static_cast<std::size_t>(-1), hostFormat.Text(), servers[selected2].domain);
    g_charServerEndpoint = endpoint;
    g_charServerIp = servers[*reinterpret_cast<int*>(this->unknown_01C8)].address.ip;
    g_session.m_serverName = servers[*reinterpret_cast<int*>(this->unknown_01C8)].address.name;
    g_checkAttendanceMgr->LoadLocal(g_session.m_serverName);
    this->pendingCursorType = 5;
    return 0;
  }
  case 10005:
    this->pendingCursorType = 10;
    return 0;
  case 10006:
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
  case 10007:
    strcpy(reinterpret_cast<char*>(this->unknown_0138), reinterpret_cast<const char*>(argument1));
    return 0;
  case 10008:
    strcpy(reinterpret_cast<char*>(&this->unknown_0138[0x40]), reinterpret_cast<const char*>(argument1));
    strcpy(reinterpret_cast<char*>(g_session.unknown_07F4), reinterpret_cast<const char*>(argument1));
    return 0;
  case 10009: {
    g_session.unknown_0F50 = static_cast<unsigned char>(g_selectedSlotIndex);
    g_windowMgr.LoadShortcutItems();
    for (int i = 0; i < this->characterCount; ++i) {
      if (this->characters[i].slot == static_cast<unsigned char>(g_selectedSlotIndex)) {
        *reinterpret_cast<int*>(&g_session.unknown_0E9C[0x04]) = this->characters[i].slot;
        g_session.SetJob(this->characters[i].job);
        g_session.m_gid = this->characters[i].gid;
        *reinterpret_cast<int*>(g_session.unknown_16F4) = this->characters[i].speed;
        g_session.InvalidateWindowFrames();
        g_session.m_level = this->characters[i].level;
        g_session.InvalidateWindowFrames();
        g_session.m_skillPoint = this->characters[i].skillPoint;
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<int*>(g_session.unknown_1654) = this->characters[i].jobPoint;
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
        *reinterpret_cast<std::int64_t*>(&g_session.unknown_162C[0x04]) = this->characters[i].exp;
        g_session.InvalidateWindowFrames();
        const int hp = static_cast<int>(this->characters[i].hp);
        if (g_session.unknown_5568 != hp) {
          g_session.unknown_5568 = hp;
          g_session.RefreshHpSpWindows();
        }
        const int maxHp = static_cast<int>(this->characters[i].maxHp);
        if (g_session.unknown_556C != maxHp) {
          g_session.unknown_556C = maxHp;
          g_session.RefreshHpSpWindows();
        }
        const int sp = static_cast<int>(this->characters[i].sp);
        if (*reinterpret_cast<int*>(&g_session.unknown_5570[0x00]) != sp) {
          *reinterpret_cast<int*>(&g_session.unknown_5570[0x00]) = sp;
          g_session.RefreshHpSpWindows();
        }
        const int maxSp = static_cast<int>(this->characters[i].maxSp);
        if (*reinterpret_cast<int*>(&g_session.unknown_5570[0x04]) != maxSp) {
          *reinterpret_cast<int*>(&g_session.unknown_5570[0x04]) = maxSp;
          g_session.RefreshHpSpWindows();
        }
        g_session.m_zeny = *reinterpret_cast<const int*>(&this->characters[i].unknown_000C[0x00]);
        g_session.RefreshHpSpWindows();
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<std::int64_t*>(&g_session.unknown_162C[0x1C]) = *reinterpret_cast<const std::int64_t*>(&this->characters[i].unknown_000C[0x04]);
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<int*>(g_session.unknown_1704) = *reinterpret_cast<const int*>(&this->characters[i].unknown_000C[0x1C]);
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<int*>(&g_session.unknown_16F4[0x04]) = *reinterpret_cast<const int*>(&this->characters[i].unknown_000C[0x20]);
        g_session.InvalidateWindowFrames();
        g_session.m_jobLevel = *reinterpret_cast<const int*>(&this->characters[i].unknown_000C[0x0C]);
        g_session.InvalidateWindowFrames();
        *reinterpret_cast<int*>(&g_session.unknown_0E9C[0x34]) = this->characters[i].unknown_008B[0];
        g_session.m_head = this->characters[i].head;
        *reinterpret_cast<int*>(&g_session.unknown_0EDC[0x04]) = 0;
        *reinterpret_cast<int*>(&g_session.unknown_0EDC[0x08]) = 0;
        *reinterpret_cast<int*>(&g_session.unknown_0EF4[0x00]) = this->characters[i].accessory;
        *reinterpret_cast<int*>(&g_session.unknown_0EDC[0x0C]) = this->characters[i].shield;
        g_session.UpdateBodyStyle(this->characters[i].job, this->characters[i].bodyStyle, this->characters[i].unknown_0058);
        g_session.m_headPalette = this->characters[i].headPalette;
        *reinterpret_cast<int*>(&g_session.unknown_0EF4[0x04]) = this->characters[i].accessory2;
        *reinterpret_cast<int*>(&g_session.unknown_0EF4[0x08]) = this->characters[i].accessory3;
        g_session.m_robe = this->characters[i].robe;
        g_session.m_bodyCostume = this->characters[i].unknown_0058;
        *reinterpret_cast<int*>(&g_session.unknown_0F0C[0x00]) = *reinterpret_cast<const int*>(&this->characters[i].unknown_000C[0x10]);
        *reinterpret_cast<int*>(&g_session.unknown_0F0C[0x04]) = *reinterpret_cast<const int*>(&this->characters[i].unknown_000C[0x14]);
        g_session.m_jobFlags = *reinterpret_cast<const std::uint32_t*>(&this->characters[i].unknown_000C[0x18]);
        return 0;
      }
    }
    return 0;
  }
  case 10011:
    GetRagConnection().Close();
    g_charServerAccountIdPending = 0;
    this->pendingCursorType = 3;
    return 0;
  case 10012:
    g_windowMgr.InvalidateAll();
    strcpy(reinterpret_cast<char*>(this->billingInfo), reinterpret_cast<const char*>(argument1));
    return 0;
  case 10013:
    return reinterpret_cast<int>(this->billingInfo);
  case 10014:
    return reinterpret_cast<int>(&this->unknown_0078);
  case 10015:
    strcpy(reinterpret_cast<char*>(&this->unknown_0078), reinterpret_cast<const char*>(argument1));
    return 0;
  case 10016:
    return reinterpret_cast<int>(this->charParams);
  case 10017: {
    const int* values = reinterpret_cast<const int*>(argument1);
    this->charParams[0] = values[0];
    this->charParams[1] = values[1];
    this->charParams[2] = values[2];
    this->charParams[3] = values[3];
    this->charParams[4] = values[4];
    this->charParams[5] = values[5];
    this->unknown_0070 = values[6];
    this->unknown_0074 = values[7];
    return 0;
  }
  case 10018:
    if (argument2 == 201) {
      g_modeMgr.gameMode->SendMsg(2, 0, 0, 0, 0);
      return 0;
    }
    this->pendingCursorType = 2;
    return 0;
  case 10019:
    this->unknown_6F48 = argument1;
    *reinterpret_cast<int*>(&g_session.unknown_5478[4]) = argument1;
    TraceSelection(argument1);
    if (g_serviceType == 19) {
      std::string suffix;
      if (argument1 == 1) {
        g_tipOfTheDayEnglish = true;
        g_serverLanguage->alternateLanguage = static_cast<std::uint16_t>(argument1);
        const std::uint8_t prefixSource[] = {0xBC, 0x94, 0x3A, 0xD7, 0x8F};
        thread_local EncryptedName<sizeof(prefixSource), 0x23BBD58F8B5FF5C9ULL> prefix(prefixSource);
        g_serverLanguage->SetServicePrefix(prefix.Text());
        const std::uint8_t suffixSource[] = {0xF2, 0x72, 0x0E, 0x23, 0x39};
        thread_local EncryptedName<sizeof(suffixSource), 0x0B7109397F6B1387ULL> suffixName(suffixSource);
        suffix = suffixName.Text();
      } else {
        g_tipOfTheDayEnglish = false;
        g_serverLanguage->alternateLanguage = 9;
        const std::uint8_t prefixSource[] = {0xC0, 0xF3, 0x0A, 0x95, 0x0A, 0x54, 0x1F, 0x5F, 0xA5};
        thread_local EncryptedName<sizeof(prefixSource), 0x03772763F96D9DA5ULL> prefix(prefixSource);
        g_serverLanguage->SetServicePrefix(prefix.Text());
        const std::uint8_t suffixSource[] = {0x8A, 0x1F, 0xB8, 0x7F, 0x04, 0x3C, 0xAF, 0xB1, 0xEF};
        thread_local EncryptedName<sizeof(suffixSource), 0xEDC74F6D13DF71EFULL> suffixName(suffixSource);
        suffix = suffixName.Text();
      }
      g_session.ReloadLanguageTables();
      g_session.IgnoreText(suffix);
    }
    this->pendingCursorType = (g_serviceType == 5 || g_serviceType == 7) ? 13 : 3;
    return 0;
  case 10020: {
    char* output = reinterpret_cast<char*>(argument1);
    switch (this->slotExtension.unknown_0001) {
    case 100:
      std::sprintf(output, LookupMessage(0x215), this->slotExtension.unknown_0005 / 60);
      return 0;
    case 200: {
      const std::uint32_t value = this->slotExtension.unknown_0005;
      std::sprintf(output, LookupMessage(0x216), value / 3600, (value / 60) % 60, value % 60);
      return 0;
    }
    case 300:
      std::sprintf(output, LookupMessage(0x217));
      return 0;
    case 400: {
      const std::uint32_t value = this->slotExtension.unknown_0009 - this->slotExtension.unknown_0005;
      std::sprintf(output, LookupMessage(0x218), value / 3600, (value / 60) % 60, value % 60);
      return 0;
    }
    case 500: {
      const std::uint32_t value = this->slotExtension.unknown_0005 != 0 ? this->slotExtension.unknown_0005 : this->slotExtension.unknown_0009;
      const int messageId = this->slotExtension.unknown_0005 != 0 ? 0x219 : 0x21A;
      std::sprintf(output, LookupMessage(messageId), value / 3600, (value / 60) % 60, value % 60);
      return 0;
    }
    case 600:
      std::sprintf(output, LookupMessage(0x21B));
      return 0;
    case 700:
      std::sprintf(output, LookupMessage(0x21C));
      return 0;
    case 800:
      std::sprintf(output, LookupMessage(0x21D));
      return 0;
    default:
      std::sprintf(output, LookupMessage(0x64));
      return 0;
    }
  }
  case 10021:
    GetRagConnection().Close();
    if (g_windowMgr.Confirm(LookupMessage(1), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
      this->otpStarted = timeGetTime();
      g_charServerAccountIdPending = 0;
      this->pendingCursorType = 3;
    }
    return 0;
  case 10022:
    this->unknown_6F4C = argument1;
    this->unknown_6F38.clear();
    TraceSelectionRange(this->unknown_6F48, this->unknown_6F4C);
    this->pendingCursorType = 3;
    return 0;
  case 10024:
    strcpy(reinterpret_cast<char*>(&this->unknown_0138[0x80]), reinterpret_cast<const char*>(argument1));
    return 0;
  case 10025:
    reinterpret_cast<unsigned char*>(&GetRagConnection())[0x6C] = 0;
    g_session.m_aid = ReplayFileList::Instance().unknown_011C;
    g_session.SetFlag(static_cast<char>(ReplayFileList::Instance().unknown_0120));
    this->pendingCursorType = 6;
    return 0;
  case 10026:
    reinterpret_cast<unsigned char*>(&GetRagConnection())[0x6C] = 1;
    this->pendingCursorType = 27;
    return 0;
  case 10027:
    g_session.SetFlag(static_cast<char>(Replay::Instance().header.unknown_0014));
    this->pendingCursorType = 28;
    return 0;
  case 10028:
    if (argument1 != -1) {
      MapStateEntry entry;
      entry.index = this->mapStates[argument1].index;
      this->mapStates.clear();
      this->mapStates.push_back(entry);
      this->pendingCursorType = 31;
    }
    return 0;
  case 10037:
    this->pendingCursorType = argument1;
    return reinterpret_cast<int>(this->billingInfo);
  case 10038:
    strcpy(reinterpret_cast<char*>(this->unknown_01D0), reinterpret_cast<const char*>(argument1));
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
  /* 000E */ std::uint16_t unknown_000E;
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
  /* 000E */ unsigned char unknown_000E[4];
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
  mode->unknown_01C4 = 0;
  *reinterpret_cast<int*>(mode->unknown_01C8) = -1;

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

  mode->unknown_01C4 = count;
  if (count != 0) {
    *reinterpret_cast<int*>(mode->unknown_01C8) = 0;
  }
}

// OFFSET: 0x00D93B20
// FUNCTION: fn_22297
void CLoginMode::Slot7(int value) {
  this->otpStarted = timeGetTime();
  g_windowMgr.Shutdown();
  this->tooltip = nullptr;
  g_session.unknown_5480 = 0;
  this->unknown_0018 = 1;

  ServerAddress2* servers = reinterpret_cast<ServerAddress2*>(this->serverList);
  SessionAccount& account = *reinterpret_cast<SessionAccount*>(g_session.unknown_0E9C);

  switch (value) {
  case 0:
  case 1:
  case 13:
    this->SendMsg(28, 0, 0, 0, 0);
    break;
  case 38: {
    CharServerEndpoint endpoint;
    std::memset(&endpoint, 0, sizeof(endpoint));
    if (!std::strstr(g_session.unknown_58FA, "Dev")) {
      const int port = rand() % 3 + 6951;
      const std::uint8_t hostSource[] = {0x24, 0xD3, 0xA0, 0x18, 0x02, 0xB0, 0x24, 0x17, 0x2C, 0xD8, 0xE1, 0x47, 0x02,
                                         0xB0, 0x2F, 0x18, 0x3D, 0xCE, 0xA4, 0x1B, 0x00, 0xB8, 0x6F, 0x12, 0x3D, 0xA1};
      thread_local EncryptedName<sizeof(hostSource), 0x7941D76335CFA14FULL> host(hostSource);
      const char* hostText = host.Text();
      const std::uint8_t formatSource[] = {0xC4, 0xEA, 0xBD, 0x6A, 0xE5, 0xC3};
      thread_local EncryptedName<sizeof(formatSource), 0x7321C3814F8799E1ULL> format(formatSource);
      StringFormat(endpoint.host, sizeof(endpoint.host), static_cast<std::size_t>(-1), format.Text(), hostText, port);
    } else {
      const std::uint8_t hostSource[] = {0xA2, 0x61, 0x96, 0x5E, 0x88, 0xB4, 0x38, 0x10, 0xAE, 0x76, 0x97, 0x10, 0x80, 0xF7, 0x67, 0x10, 0xAE,
                                         0x7D, 0x98, 0x01, 0x96, 0xB2, 0x3B, 0x12, 0xA6, 0x3D, 0x92, 0x01, 0xC3, 0xEF, 0x2C, 0x44, 0xF8, 0x13};
      thread_local EncryptedName<sizeof(hostSource), 0x7115D9F973F913C9ULL> host(hostSource);
      const char* hostText = host.Text();
      const std::uint8_t formatSource[] = {0x46, 0x86, 0xE1};
      thread_local EncryptedName<sizeof(formatSource), 0xCB1D253B7FE1F563ULL> format(formatSource);
      StringFormat(endpoint.host, sizeof(endpoint.host), static_cast<std::size_t>(-1), format.Text(), hostText);
    }

    this->unknown_0018 = GetRagConnection().Connect(*reinterpret_cast<const ConnectionEndpoint*>(&endpoint));
    if (!this->unknown_0018) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(1), 1, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        this->pendingCursorType = 3;
      }
      break;
    }

    ZoneLoginPacket packet;
    std::memset(&packet, 0, sizeof(packet));
    packet.id = 0xACF;
    const std::uint8_t codeSource[] = {0x7F, 0x57, 0xDC, 0xB4, 0x09};
    thread_local EncryptedName<sizeof(codeSource), 0x793F370985ED674FULL> code(codeSource);
    std::memcpy(packet.code, code.Text(), sizeof(packet.code));
    const std::uint8_t nameFormatSource[] = {0x38, 0xB4, 0x4B};
    thread_local EncryptedName<sizeof(nameFormatSource), 0x234977DDB54BC71DULL> nameFormat(nameFormatSource);
    StringFormat(packet.name, sizeof(packet.name), static_cast<std::size_t>(-1), nameFormat.Text(), reinterpret_cast<const char*>(&this->unknown_0138[0x40]));
    char password[33] = {};
    const std::uint8_t passwordFormatSource[] = {0xE0, 0x5E, 0x2B};
    thread_local EncryptedName<sizeof(passwordFormatSource), 0xEF6D1D893F2B2DC5ULL> passwordFormat(passwordFormatSource);
    StringFormat(password, sizeof(password), static_cast<std::size_t>(-1), passwordFormat.Text(), reinterpret_cast<const char*>(this->unknown_0138));
    DecryptRijndael256(password, packet.password);
    const std::uint8_t extraFormatSource[] = {0x7E, 0x20, 0x5F};
    thread_local EncryptedName<sizeof(extraFormatSource), 0x39A39F87DD5F535BULL> extraFormat(extraFormatSource);
    StringFormat(packet.extra, sizeof(packet.extra), static_cast<std::size_t>(-1), extraFormat.Text(), reinterpret_cast<const char*>(this->unknown_01D0));
    GetRagConnection().Send(sizeof(packet), &packet);
    this->otpStarted = timeGetTime();
    this->window_6F14 = g_windowMgr.EnsureWindow(6);
    static_cast<UIWaitWnd*>(this->window_6F14)->SetMessage(LookupMessage(122), 16, 1);
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
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    if (!g_session.unknown_5912) {
      if (g_session.m_loginModeFlag == true) {
        this->otpStarted = timeGetTime();
        g_charServerAccountIdPending = 0;
        this->unknown_6F74 = false;
        this->unknown_6F75 = false;
        SecureZeroMemory(g_session.m_loginErrorText, sizeof(g_session.m_loginErrorText));
      }

      if (std::strcmp(g_session.m_loginErrorText, "") == 0) {
        g_session.m_loginModeFlag = true;
        if (!g_windowMgr.LookupWindow(3)) {
          g_windowMgr.EnsureWindow(3);
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
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    UISelectServerWnd* window = static_cast<UISelectServerWnd*>(g_windowMgr.EnsureWindow(2));
    const std::uint8_t countNameSource[] = {0x39, 0xC2, 0xCA, 0x4E, 0x66, 0x01, 0xB7, 0x2A, 0x25, 0x97};
    thread_local EncryptedName<sizeof(countNameSource), 0x6FE153231D879777ULL> countName(countNameSource);
    window->SendMsg(nullptr, 80, 10003, reinterpret_cast<std::intptr_t>(countName.Text()), 0, 0);

    if (g_loginCancelRequested == 1) {
      g_loginCancelRequested = 0;
      LoadCustomServerList(this);
    } else if (this->unknown_01C4 < 1 || this->unknown_01C4 > 8) {
      LoadCustomServerList(this);
    }
    if (this->unknown_01C4 != 0) {
      g_customServerListLoaded = 1;
    }

    char(*displayNames)[128] = CustomServerBalloons();
    for (int i = 0; i < this->unknown_01C4; ++i) {
      const ServerAddress2& server = servers[i];
      char text[64];
      std::memset(text, 0, sizeof(text));
      if (server.address.property == 1) {
        std::sprintf(text, LookupMessage(0x1E2));
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
        status = FormatOwnedString("%s%s  ^000000%s", green.c_str(), LookupMessage(0xCBD), text);
        break;
      case 1:
        status = FormatOwnedString("%s%s  ^000000%s", orange.c_str(), LookupMessage(0xCBE), text);
        break;
      case 2:
        status = FormatOwnedString("%s%s  ^000000%s", red.c_str(), LookupMessage(0xCBF), text);
        break;
      case 3:
        status = FormatOwnedString("%s%s  ^000000%s", purple.c_str(), LookupMessage(0xCC0), text);
        break;
      }

      switch (server.address.state) {
      case 2:
        status.append(LookupMessage(0xCBA));
        break;
      case 3:
        status.append(LookupMessage(0xCBB));
        break;
      case 4:
        status.append(LookupMessage(0xCBC));
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

    window->SendMsg(nullptr, 40, 0, 0, 0, 0);
    break;
  }
  case 7:
    g_screenShakeDuration = 0;
    g_statusRefreshSuppressed = 0;
    g_skillTooltipActive = 0;
    g_session.cartCount = 0;
    g_session.cartWeight = 0;
    g_session.unknown_103C = -1;
    g_session.SetUnknown16EC(0);
    g_session.unknown_5450 = 0;
    g_renewQuestMgr->m_ongoingQuests.clear();
    g_session.ClearAchievements();
    g_nProtectGameGuardMgr->IncrementCounter();
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_rodexMailStore->Clear();
    if (UINewSelectCharWnd* window = dynamic_cast<UINewSelectCharWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(0x115)))) {
      window->m_unknown13C = false;
    }
    g_session.ClearEntryList5BD4();
    g_session.ClearPairValues(1, 1, 1);
    g_windowMgr.CloseWindow(198);
    g_windowMgr.CloseWindow(269);
    g_session.RestoreItems5C68();
    g_session.ClearItemQuantities();
    g_session.ClearInventory();
    g_session.ResetEquipment();
    g_session.ResetItems2B50();
    g_session.ClearItems1740();
    g_session.ResetItems61B8();
    g_session.ResetItems6B68();
    switch (g_session.m_passwordWndMode) {
    case 2:
      g_session.m_passwordInputAlternate = false;
      g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
      g_windowMgr.EnsureWindow(199);
      break;
    case 3:
      g_session.m_passwordInputAlternate = false;
      g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
      g_windowMgr.EnsureWindow(199);
      g_windowMgr.unknown_03B8->SendMsg(nullptr, 6, 0x124, 0, 0, 0);
      break;
    case 4:
      g_session.m_passwordInputAlternate = true;
      g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
      g_windowMgr.EnsureWindow(200);
      g_windowMgr.unknown_03BC->SendMsg(nullptr, 6, 0x124, 0, 0, 0);
      g_windowMgr.unknown_03BC->SendMsg(nullptr, 6, 0xB8, 1, 0, 0);
      break;
    case 5:
      g_session.m_passwordInputAlternate = true;
      g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
      g_windowMgr.EnsureWindow(200);
      break;
    }
    break;
  case 8:
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_windowMgr.EnsureWindow(0x116);
    break;
  case 4: {
    g_playerNameDictionary->Clear();
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    if (!std::strstr(g_session.unknown_58FA, "Dev")) {
      const std::uint8_t hostSource[] = {0xBA, 0x29, 0xF8, 0xC8, 0x58, 0xE4, 0x88, 0x1E, 0xFF, 0x29, 0xF6, 0x82, 0x57, 0xE6, 0x99,
                                         0x40, 0xBA, 0x75, 0xF4, 0x8A, 0x17, 0xEC, 0x99, 0x15, 0xE7, 0x62, 0xA7, 0xD5, 0x39};
      thread_local EncryptedName<sizeof(hostSource), 0x2FEB8739E5975BD1ULL> host(hostSource);
      host.Text();
      const std::uint8_t formatSource[] = {0xE8, 0x70, 0xCD};
      thread_local EncryptedName<sizeof(formatSource), 0x33133B977BCD03CDULL> format(formatSource);
      format.Text();
    } else {
      const std::uint8_t hostSource[] = {0x44, 0xA5, 0x2A, 0x08, 0x48, 0xC6, 0x00, 0x90, 0x4E, 0xF9, 0x37, 0x44, 0x5E, 0xC5, 0x4C,
                                         0xD3, 0x40, 0xBC, 0x6B, 0x46, 0x56, 0x85, 0x46, 0xD3, 0x15, 0xE1, 0x7C, 0x15, 0x09, 0xAB};
      thread_local EncryptedName<sizeof(hostSource), 0xA12DAB392545D72FULL> host(hostSource);
      host.Text();
      const std::uint8_t formatSource[] = {0x50, 0x48, 0x03};
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

    this->unknown_0018 = GetRagConnection().Connect(*reinterpret_cast<const ConnectionEndpoint*>(&endpoint));
    if (!this->unknown_0018) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(1), 1, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
      break;
    }

    g_packetReassembler->Notify(2);
    SendExecutableDigest(g_serviceType, g_serviceMode, g_instance, GetRagConnection(), *reinterpret_cast<const PacketLengths*>(&GetRagConnection()));
    AccountLoginPacket packet;
    packet.id = 0x64;
    packet.version = g_patchedClientVersion;
    std::memcpy(packet.name, &this->unknown_0138[0x40], sizeof(packet.name));
    if (g_serviceType == 7) {
      char password[24];
      DecryptAES(reinterpret_cast<const char*>(this->unknown_0138), password, sizeof(password));
      std::memcpy(packet.password, password, sizeof(packet.password));
    } else if (g_serviceType == 4) {
      DecryptRijndael192(reinterpret_cast<const char*>(this->unknown_0138), packet.password);
    } else {
      std::memcpy(packet.password, this->unknown_0138, sizeof(packet.password));
    }
    packet.clientType = static_cast<unsigned char>(GetClientType());
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(122), 16, 1);
    break;
  }
  case 24:
    g_windowMgr.EnsureWindow(180);
    break;
  case 5: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_session.m_partyBooking.Reset();
    GetRagConnection().Close();
    this->unknown_0018 = GetRagConnection().Connect(*reinterpret_cast<const ConnectionEndpoint*>(&g_charServerEndpoint));
    if (!this->unknown_0018) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(1), 1, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
      break;
    }

    g_packetReassembler->Notify(2);
    g_charServerAccountIdPending = 1;
    CharServerEnterPacket packet;
    packet.id = 0x65;
    packet.unknown_000E = 0;
    packet.aid = g_session.m_aid;
    packet.authCode = account.authCode;
    packet.userLevel = account.userLevel;
    packet.sex = static_cast<unsigned char>(account.sex);
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(122), 16, 1);
    g_accountLinkedUserDataMgr->Load(g_session.m_aid, g_session.m_accountName);
    break;
  }
  case 10: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    MakeCharacterPacket packet;
    packet.id = 0xA39;
    std::memcpy(packet.name, g_charInfo.name, sizeof(packet.name));
    packet.slot = static_cast<unsigned char>(g_selectedSlotIndex);
    packet.headPalette = g_charInfo.headPalette;
    packet.head = g_charInfo.head;
    packet.job = g_charInfo.job;
    packet.sex = g_charInfo.sex;
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    if (UIWaitWnd* wait = static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))) {
      wait->SetMessage(LookupMessage(123), 17, 1);
    }
    break;
  }
  case 11: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    DeleteCharacterPacket packet;
    packet.id = 0x829;
    std::strncpy(packet.code, reinterpret_cast<const char*>(this->billingInfo), sizeof(packet.code));
    const CharacterInfo* character = reinterpret_cast<const CharacterInfo*>(this->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
    packet.gid = character->gid;
    if (g_serviceType == 0) {
      if (this->billingInfo[5] == '\0') {
        packet.code[0] = '\0';
      }
      GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    } else if (g_serviceType == 10 || g_serviceType == 12 || g_serviceType == 1) {
      DeleteCharacterEmailPacket email;
      email.gid = character->gid;
      email.id = 0x1FB;
      std::strncpy(email.code, reinterpret_cast<const char*>(this->billingInfo), sizeof(email.code));
      email.code[49] = '\0';
      GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(email.id), &email);
    } else {
      GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    }
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(124), 17, 1);
    break;
  }
  case 9: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    SelectCharacterPacket packet;
    packet.id = 0x66;
    packet.slot = static_cast<unsigned char>(g_selectedSlotIndex);
    g_cashEmotionMgr->ClearOwnedPacks();
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(122), 16, 1);
    break;
  }
  case 12: {
    g_characterLinkedUserDataMgr->Load(g_session.m_aid, g_session.m_gid, g_session.m_accountName);
    g_runeSystemMgr->Reset();
    g_session.unknown_593C = 0;
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    this->unknown_0018 = GetRagConnection().Connect(*reinterpret_cast<const ConnectionEndpoint*>(g_zoneServerIp));
    if (!this->unknown_0018) {
      GetRagConnection().Close();
      if (g_windowMgr.Confirm(LookupMessage(1), 1, true, true, 0, "Message", 280, 120, 0) != 0x10D) {
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
      break;
    }

    g_nProtectGameGuardMgr->IncrementCounter();
    g_packetReassembler->Notify(1);
    char keyName[MAX_PATH];
    std::strcpy(keyName, g_subKey);
    const std::uint8_t cashKeySource[] = {0xD1, 0xF5, 0xAA, 0x14, 0xD2, 0xC6, 0xEC, 0xDB, 0x8D};
    thread_local EncryptedName<sizeof(cashKeySource), 0xB39FA7917BCFBB8DULL> cashKey(cashKeySource);
    std::strcat(keyName, cashKey.Text());
    DWORD category = 0;
    DWORD disposition = 0;
    HKEY key;
    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, keyName, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
      const std::uint8_t valueNameSource[] = {0xB4, 0x52, 0x78, 0xE7, 0xAE, 0x4C, 0x5C, 0xD9, 0xB2, 0x54, 0x64, 0xFD, 0xA8, 0x0F};
      thread_local EncryptedName<sizeof(valueNameSource), 0x8D1D0FF1AF2B13F7ULL> valueName(valueNameSource);
      RegSetValueExA(key, valueName.Text(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&category), sizeof(category));
      RegCloseKey(key);
    }

    ZoneEnterPacket packet;
    packet.packetType = 0x436;
    packet.clientTime = timeGetTime();
    packet.aid = g_session.m_aid;
    packet.gid = g_session.m_gid;
    packet.authCode = account.authCode;
    packet.sex = static_cast<unsigned char>(g_session.SyncUnknown0E98());
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.packetType), &packet);
    g_session.SetOption(17, 1, 0);
    GetRagConnection().SetBlock(1);
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(122), 16, 1);
    break;
  }
  case 19:
    g_screenShakeDuration = 0;
    g_statusRefreshSuppressed = 0;
    g_skillTooltipActive = 0;
    g_session.unknown_103C = -1;
    g_session.SetUnknown16EC(0);
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_windowMgr.EnsureWindow(127);
    break;
  case 29: {
    if (g_serviceType == 6 || g_serviceType == 10 || g_serviceType == 12) {
      g_serviceOptions.Set(0x6C, 0);
    }
    const unsigned char flags = g_serviceOptions.Get(0x6C);
    SetServiceFlags(flags, flags);
    reinterpret_cast<Replay&>(ReplayFileList::Instance()).SetBaseFrame(reinterpret_cast<std::uint32_t>(&Replay::Instance()));
    const std::uint8_t wallpaperSource[] = {0x77, 0x9E, 0x2F, 0x39, 0xC3, 0x11, 0x5C, 0x82, 0x71, 0xD5, 0x2F, 0x0F, 0xBE, 0x65, 0xC5,
                                            0x23, 0xD8, 0x56, 0x86, 0xAD, 0x5C, 0xB6, 0xF7, 0x3B, 0xD2, 0x43, 0x89, 0xA2, 0x60, 0xBA,
                                            0xC5, 0x38, 0xD6, 0x43, 0x81, 0xAA, 0x6D, 0xB8, 0xAB, 0x61, 0xD5, 0x5C, 0x9F, 0xC3};
    thread_local EncryptedName<sizeof(wallpaperSource), 0x4F99DF03C3EF31B7ULL> wallpaper(wallpaperSource);
    this->wallpaperName = std::string(wallpaper.Text());
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    g_windowMgr.EnsureWindow(20)->SendMsg(nullptr, 80, 10026, 0, 0, 0);
    break;
  }
  case 25:
    if (ReplayFileList::Instance().Open(32, 0, 0) == 1) {
      this->pendingCursorType = 28;
    } else {
      g_windowMgr.Confirm(LookupMessage(0xC4E), 4, true, true, 0, "Message", 280, 120, 0);
      this->pendingCursorType = 26;
    }
    break;
  case 26:
    this->unknown_01C4 = ReplayFileList::Instance().unknown_0124;
    if (this->unknown_01C4 > 100) {
      this->unknown_01C4 = 0;
      ReplayFileList::Instance().unknown_0124 = 0;
    }
    for (int i = 0; i < this->unknown_01C4; ++i) {
      std::memcpy(&servers[i], &reinterpret_cast<const ServerAddress2*>(ReplayFileList::Instance().buffer)[i], sizeof(ServerAddress2));
    }
    this->SendMsg(10026, 0, 0, 0, 0);
  case 27: {
    this->wallpaperName = g_loginWallpaperName;
    g_windowMgr.SetSurface0478(reinterpret_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->wallpaperName.c_str()))));
    UISelectReplayDataWnd* window = static_cast<UISelectReplayDataWnd*>(g_windowMgr.EnsureWindow(186));
    ReplayFileList::Instance().Refresh();
    const int count = static_cast<int>(ReplayFileList::Instance().files.size());
    for (int i = 0; i < count; ++i) {
      window->m_list->AddLine(ReplayFileList::Instance().FileName(i));
    }
    window->SendMsg(nullptr, 40, 0, 0, 0, 0);
    ReplayFileList::Instance().unknown_0124 = this->unknown_01C4;
    if (this->unknown_01C4 > 100) {
      this->unknown_01C4 = 0;
      ReplayFileList::Instance().unknown_0124 = 0;
    }
    for (int i = 0; i < this->unknown_01C4; ++i) {
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
    g_windowMgr.CloseWindow(269);
    g_session.RestoreItems5C68();
    g_session.ClearItemQuantities();
    g_session.ClearInventory();
    g_session.ResetEquipment();
    g_session.ResetItems2B50();
    g_session.ClearItems1740();
    g_session.ResetItems61B8();
    g_session.ResetItems6B68();
    Replay::Instance().SetState(6);
    ReplayHeader header = Replay::Instance().header;
    g_session.SetPlayerNameKey(reinterpret_cast<const unsigned char*>(header.characterName));
    g_session.SetNavigationPlayerMap(header.mapName);
    g_session.unknown_0000 = header.accountId;
    g_session.m_playerX = header.cellX;
    g_session.m_playerY = header.cellY;
    g_session.m_playerDir = header.unknown_0078;
    g_windowMgr.ApplyReplayOptions(header.options);
    g_session.ResetUnknownState();
    g_session.ReadReplayCharacter(Replay::Instance().ReadSection(3));
    char mapName[56];
    std::sprintf(mapName, "%s.rsw", g_session.m_navigationPlayerMap);
    g_modeMgr.Switch(1, mapName);
    break;
  }
  case 30: {
    UISelectServerWnd* window = static_cast<UISelectServerWnd*>(g_windowMgr.EnsureWindow(2));
    const std::uint8_t countNameSource[] = {0xB9, 0x9C, 0x2E, 0x33, 0x32, 0xD7, 0x34, 0x91};
    thread_local EncryptedName<sizeof(countNameSource), 0x9171997D6963C9F7ULL> countName(countNameSource);
    window->SendMsg(nullptr, 80, 10028, reinterpret_cast<std::intptr_t>(countName.Text()), 0, 0);
    for (unsigned int i = 0; i < this->mapStates.size(); ++i) {
      char mapName[16];
      std::strncpy(mapName, g_session.GatName2RswName(reinterpret_cast<const char*>(&this->mapStates[i].data)), sizeof(mapName));
      window->m_serverList->AddLine(g_session.GetMapDisplayName(mapName));
    }
    window->SendMsg(nullptr, 40, 0, 0, 0, 0);
    break;
  }
  case 31: {
    SelectMapPacket packet;
    packet.id = 0x841;
    packet.slot = static_cast<unsigned char>(g_selectedSlotIndex);
    packet.map = static_cast<unsigned char>(this->mapStates.front().index);
    GetRagConnection().Send(reinterpret_cast<const PacketLengths*>(&GetRagConnection())->FixedLength(packet.id), &packet);
    break;
  }
  default:
    break;
  }
}
