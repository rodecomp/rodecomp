#include "Client/Mode/CLoginMode.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Stove/StoveMgr.h"
#include "Client/ModeFadeOut.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EntryQueueMgr.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/ResourcePreload.h"
#include "Security/NProtectGameGuardMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Dialogs/UIPasswordWnd.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <iphlpapi.h>
#include <new>
#include <timeapi.h>

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
#pragma pack(pop)

// GLOBAL: gl_25054
int g_charServerAccountIdPending;
// GLOBAL: gl_25147
int g_isStopByLogin;
// GLOBAL: gl_25146
int g_loginStartMode = 3;
// GLOBAL: gl_25148
const char* g_loginWallpaperName = "";

static_assert(sizeof(CharacterSlotsPacket) == 0x1D);
static_assert(offsetof(AcceptEnterPacket, characters) == 0x1B);

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

struct CharacterInfo2SizeErrorFormat {
  __declspec(noinline) EncryptedName<75, 0xED15938D333D2DB9ULL>& operator()() const;
};

// OFFSET: 0x00D8CA50
// FUNCTION: fn_22261
EncryptedName<75, 0xED15938D333D2DB9ULL>& CharacterInfo2SizeErrorFormat::operator()() const {
  const std::uint8_t source[] = {0xFA, 0x65, 0x7C, 0x61, 0xCC, 0xD0, 0x41, 0xA8, 0xEB, 0x72, 0x74, 0x7D, 0xCB, 0xDC, 0x27, 0xCD,
                                 0xCA, 0x44, 0x47, 0x56, 0xAD, 0xF6, 0x67, 0x9F, 0xD6, 0x5F, 0x1C, 0x12, 0xAD, 0xF0, 0x7D, 0x8C,
                                 0xCB, 0x4C, 0x5E, 0x47, 0xE8, 0xE1, 0x35, 0x83, 0xCC, 0x40, 0x1D, 0x09, 0xAD, 0xB6, 0x71, 0xCD,
                                 0x99, 0x6E, 0x75, 0x72, 0xDF, 0xD2, 0x56, 0xB9, 0xFC, 0x7F, 0x62, 0x7A, 0xC3, 0xD5, 0x5A, 0xDF,
                                 0x99, 0x5E, 0x54, 0x49, 0xE8, 0xB3, 0x2F, 0xCD, 0x9C, 0x49, 0x3D};
  thread_local EncryptedName<sizeof(source), 0xED15938D333D2DB9ULL> name(source);
  return name;
}

struct PasswordRetryExceededMessage {
  __declspec(noinline) EncryptedName<103, 0xBF2B4B31397FF5A7ULL>& operator()() const;
};

// OFFSET: 0x00D8CB50
// FUNCTION: fn_22262
EncryptedName<103, 0xBF2B4B31397FF5A7ULL>& PasswordRetryExceededMessage::operator()() const {
  const std::uint8_t source[] = {0x1D, 0x04, 0xC6, 0xE9, 0x88, 0xB3, 0xE3, 0x1C, 0x1F, 0x53, 0x5F, 0x0A, 0xF9, 0xF3, 0x0B, 0x7F,
                                 0x78, 0x4D, 0x87, 0xF9, 0xE5, 0xFC, 0xE9, 0x78, 0x68, 0x4A, 0xCB, 0x84, 0xF1, 0xFF, 0xE4, 0x0B,
                                 0x7E, 0xDB, 0x5F, 0x83, 0xC0, 0xF2, 0xFB, 0x06, 0x5F, 0x3D, 0xDC, 0x81, 0x97, 0x6B, 0xE3, 0x11,
                                 0x67, 0x3B, 0xB8, 0xE1, 0xF0, 0x9D, 0x97, 0x03, 0x18, 0x11, 0x51, 0x19, 0x11, 0x6B, 0x0B, 0x9F,
                                 0x87, 0xD5, 0x5F, 0x19, 0x11, 0x6B, 0x0B, 0x9F, 0x87, 0x30, 0xD3, 0x8F, 0xC2, 0x8B, 0xE7, 0x01,
                                 0x57, 0x33, 0xD1, 0x19, 0x8C, 0x8C, 0xEC, 0x5F, 0x67, 0x4E, 0x5F, 0xF8, 0x8F, 0xFC, 0xCA, 0x78,
                                 0x72, 0x41, 0xB0, 0x8D, 0xE8, 0x65, 0x2B};
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
  WindowDialogs& dialogs = *reinterpret_cast<WindowDialogs*>(&g_windowMgr);
  int message;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 0:
    dialogs.Confirm(LookupMessage(0x4BE), 0, true, false, 0, "Message", 280, 120, 0);
    if (window) {
      window->SendMsg(nullptr, 6, 0x160, 0, 0, 0);
    }
    return;
  case 1:
    message = 0x4C2;
    break;
  case 2:
    if (dialogs.Confirm(LookupMessage(0x4C3), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
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
    if (dialogs.Confirm(LookupMessage(message), 0, true, true, 0, "Message", 280, 120, 0) != 0xB8) {
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

  if (dialogs.Confirm(LookupMessage(message), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8 && g_windowMgr.window_03DC) {
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
    case 14:
      return 25;
    case 15:
      return 33;
    case 19:
      return 35;
    case 20:
      return 36;
    default:
      return 2;
    }

  case 5:
    switch (g_serviceType) {
    case 2:
      return 28;
    case 5:
      return 29;
    case 6:
      return 34;
    case 7:
      return 27;
    case 15:
      return 32;
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
      const std::uint8_t source[] = {0x32, 0x2B, 0x1F, 0xFB, 0x90, 0xCE, 0x71, 0x3D, 0x4F, 0x36, 0x08, 0x93, 0x8F, 0xB3, 0x6C,
                                     0x2A, 0x27, 0x29, 0x75, 0x8E, 0x98, 0xDB, 0x73, 0x57, 0x3A, 0x3E, 0x1D, 0x91, 0xE5, 0xEB};
      thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> format(source);
      _snprintf(text, sizeof(text), format.Text(), adapter->Address[0], adapter->Address[1], adapter->Address[2],
        adapter->Address[3], adapter->Address[4], adapter->Address[5]);
      delete[] reinterpret_cast<char*>(adapter);
      return text;
    }
  }

  const std::uint8_t source[] = {0xF0, 0x76, 0xE8, 0x25, 0xD3, 0xB4, 0xE6, 0xC8, 0xEC, 0x73, 0xF1, 0x3A, 0xD4, 0xAC, 0xF8, 0xCD,
                                 0xF7, 0x47};
  thread_local EncryptedName<sizeof(source), 0xFBD599E117C547C1ULL> fallback(source);
  return fallback.Text();
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

    maxSlot = std::max(maxSlot, static_cast<int>(this->characters[i].slot));
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
void CLoginMode::ReceiveAcceptEnter(const AcceptEnterPacket* packet, WindowDialogs& dialogs) {
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
    dialogs.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
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
void CLoginMode::ReceiveCharacterResult(const CharacterResultPacket* packet, WindowDialogs& dialogs) {
  UIWindow* window = g_windowMgr.window_03D4;
  int message;
  switch (packet->result) {
  case 0:
    dialogs.Confirm(LookupMessage(1214), 0, true, false, 0, "Message", 280, 120, 0);
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
    if (dialogs.Confirm(LookupMessage(1215), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
      window->SendMsg(nullptr, 6, 351, 0, 0, 0);
    }
    return;
  case 4:
    if (dialogs.Confirm(LookupMessage(1220), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
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

  if (dialogs.Confirm(LookupMessage(message), 0, true, true, 0, "Message", 280, 120, 0) == 184 && window) {
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
    const std::uint8_t source[] = {0x4C, 0x07, 0x04, 0xFD, 0xDC, 0x06, 0xCD, 0x46, 0x5D, 0x10, 0x0C, 0xE1, 0xDB, 0x0A, 0xAB, 0x23,
                                   0x7C, 0x26, 0x3F, 0xCA, 0xBD, 0x20, 0xEB, 0x71, 0x60, 0x3D, 0x64, 0x8E, 0xBD, 0x06, 0xD1, 0x42,
                                   0x5D, 0x0E, 0x06, 0xFB, 0xD8, 0x17, 0xC6, 0x4A, 0x41, 0x09, 0x0A, 0x9D, 0xBD, 0x36, 0xF0, 0x79,
                                   0x6A, 0x6F, 0x7F, 0x8F, 0xB8, 0x21, 0x99};
    thread_local EncryptedName<sizeof(source), 0x0399459DAF454F0FULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), sizeof(CharacterInfo));
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
    return;
  }

  const unsigned int total = this->unknown_01D8 + count;
  if (total > 68) {
    char text[256];
    memset(text, 0, sizeof(text));
    const std::uint8_t source[] = {0xE8, 0xE7, 0xCC, 0xDF, 0xF2, 0xCE, 0xF3, 0x90, 0xF9, 0xF0, 0xC4, 0xC3, 0xF5, 0xC2, 0x95, 0xF5,
                                   0xE8, 0xC1, 0xF9, 0xAD, 0xFC, 0xFB, 0xC2, 0xA7, 0x8B, 0x8A, 0xE9, 0xAD, 0x9C, 0xAD, 0x82, 0xB1,
                                   0xAB};
    thread_local EncryptedName<sizeof(source), 0xD5A78DB38D8DAFABULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), total, 68);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
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
void CLoginMode::ReceiveCharacterList(const CharacterListPacket* packet) {
  this->characterCount = (packet->length - 4) / sizeof(CharacterInfo);
  g_session.unknown_59BC = this->characterCount;
  memcpy_s(this->characters, sizeof(this->characters), packet->characters, this->characterCount * sizeof(CharacterInfo));
  this->pendingCursorType = 7;
}

// OFFSET: 0x00D8FA00
// FUNCTION: fn_22281
void CLoginMode::ReceiveDeleteCharacter(const CharacterDeletePacket* packet, WindowDialogs& dialogs) {
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
    dialogs.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
  }
  if (this->characterDeleted != true) {
    this->pendingCursorType = 7;
  }
}

// OFFSET: 0x00D8FC80
// FUNCTION: fn_22282
void CLoginMode::ReceiveDeleteCancel(const CharacterDeletePacket* packet, WindowDialogs& dialogs) {
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
    dialogs.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

// OFFSET: 0x00D8FE40
// FUNCTION: fn_22283
void CLoginMode::ReceiveDeleteReservation(const CharacterDeletePacket* packet, WindowDialogs& dialogs) {
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
    dialogs.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

// OFFSET: 0x00D904B0
// FUNCTION: fn_22284
void CLoginMode::ReceiveMapStates(const MapStatePacket* packet, WindowDialogs& dialogs) {
  const unsigned int count = (packet->length - 4) / sizeof(MapStateInfo);
  if ((packet->length - 4) % sizeof(MapStateInfo) != 0) {
    const std::uint8_t source[] = {0xD8, 0xE6, 0x32, 0x48, 0x72, 0x89, 0x4C, 0x11, 0x95, 0xC2, 0x20, 0x55, 0x40, 0x89, 0x4E, 0x0D,
                                   0xD0, 0xAF, 0x32, 0x4C, 0x69, 0x98, 0x07, 0x0A, 0xDC, 0xF5, 0x24, 0x05, 0x2E, 0xDD, 0x0A, 0x1D,
                                   0x9C, 0x8F};
    thread_local EncryptedName<sizeof(source), 0x792FFD1325418FB5ULL> format(source);
    char text[256];
    sprintf(text, format.Text(), sizeof(MapStateInfo));
    dialogs.Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
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
    dialogs.Confirm(LookupMessage(1811), 0, true, false, 0, "Message", 280, 120, 0);
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
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
        this->otpStarted = timeGetTime();
        g_charServerAccountIdPending = 0;
        this->pendingCursorType = 3;
      }
    }
    break;
  case 6:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x3A98) {
      GetRagConnection().Close();
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
        g_modeMgr.Quit();
      }
    }
    break;
  case 12:
    if (static_cast<int>(timeGetTime()) > this->otpStarted + 0x3A98) {
      GetRagConnection().Close();
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1), 0, true, false, 0, "Message", 280, 120, 0) != 0x10D) {
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
            if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xD16), 0, true, false, 0, "Message", 280, 120, 0) == 0xB8) {
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
          if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xD70), 0, true, false, 0, "Message", 280, 120, 0) == 0xB8) {
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
void OpenAccountRegistration() {
  switch (g_serviceType) {
  case 0:
    if (*reinterpret_cast<const int*>(&g_session.unknown_59DC[0x198]) == 1) {
      const char* url = LookupMessage(1308);
      const std::uint8_t source[] = {0x30, 0x45, 0xCA, 0xFB, 0x9D};
      thread_local EncryptedName<sizeof(source), 0xCFF3899D95AF355FULL> operation(source);
      ShellExecuteA(hWnd, operation.Text(), url, nullptr, nullptr, SW_SHOWNORMAL);
    } else {
      const char* url = LookupMessage(528);
      const std::uint8_t source[] = {0x4A, 0x01, 0xCA, 0xA5, 0x0D};
      thread_local EncryptedName<sizeof(source), 0xE52DF30DCBAF7125ULL> operation(source);
      ShellExecuteA(hWnd, operation.Text(), url, nullptr, nullptr, SW_SHOWNORMAL);
    }
    break;
  case 1: {
    const std::uint8_t urlSource[] = {0x0F, 0xDF, 0xE5, 0xD5, 0xD2, 0x05, 0xCC, 0x3C, 0x0E, 0xD9, 0xFE, 0x8B, 0xD3, 0x5E, 0x84, 0x7D,
                                      0x06, 0xD9, 0xFE, 0xCE, 0xCE, 0x51, 0x8F, 0x7A, 0x09, 0xCE, 0xBF, 0xC6, 0xCE, 0x52, 0xCC, 0x7E,
                                      0x02, 0xC6, 0xF3, 0xC0, 0xD3, 0x10, 0x8E, 0x76, 0x0A, 0xC9, 0xF4, 0xD7, 0xCB, 0x50, 0x8A, 0x7D,
                                      0x56, 0x85, 0xF0, 0xD6, 0xD1, 0x00, 0x8E, 0x5D, 0x12, 0xC6, 0xAC, 0x94, 0xA1};
    thread_local EncryptedName<sizeof(urlSource), 0x13E33FA1A591AB67ULL> url(urlSource);
    const char* file = url.Text();
    const std::uint8_t source[] = {0x58, 0x37, 0xC8, 0x57, 0xBF};
    thread_local EncryptedName<sizeof(source), 0x17A9E5BF39AD4737ULL> operation(source);
    ShellExecuteA(hWnd, operation.Text(), file, nullptr, nullptr, SW_SHOWNORMAL);
    break;
  }
  case 2:
    break;
  case 3: {
    const std::uint8_t urlSource[] = {0x2B, 0x5D, 0x27, 0x57, 0xEF, 0x90, 0x66, 0xED, 0x6D, 0x42, 0x26, 0x49, 0xB9, 0xCA, 0x27, 0xB7,
                                      0x20, 0x46, 0x3E, 0x08, 0xEA, 0xDE, 0x2A, 0xED, 0x7E, 0x5F, 0x3C, 0x52, 0xB6, 0xD7, 0x2C, 0xEB,
                                      0x6D, 0x44, 0x32, 0x4E, 0xBB, 0x99, 0x2A, 0xF0, 0x27, 0x14, 0x62, 0x17, 0xE5, 0x8F, 0x6F, 0xE9,
                                      0x2A, 0x4D, 0x6E, 0x11, 0xE4, 0xBF};
    thread_local EncryptedName<sizeof(urlSource), 0x9949BFD527532943ULL> url(urlSource);
    const char* file = url.Text();
    const std::uint8_t source[] = {0x00, 0x0D, 0x9E, 0x4D, 0x71};
    thread_local EncryptedName<sizeof(source), 0xCD8F077123FB7D6FULL> operation(source);
    ShellExecuteA(hWnd, operation.Text(), file, nullptr, nullptr, SW_SHOWNORMAL);
    break;
  }
  default: {
    const char* url = LookupMessage(528);
    const std::uint8_t source[] = {0x02, 0xCB, 0xF0, 0x43, 0x93};
    thread_local EncryptedName<sizeof(source), 0xC517FB932D95BB6DULL> operation(source);
    ShellExecuteA(hWnd, operation.Text(), url, nullptr, nullptr, SW_SHOWNORMAL);
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
    const std::uint8_t source[] = {0x40, 0x6A, 0x45};
    thread_local EncryptedName<sizeof(source), 0x891B232D09451965ULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
  }

  switch (packet->result) {
  case 1: {
    const char* message = LookupMessage(3196);
    const std::uint8_t source[] = {0x9C, 0x76, 0x77};
    thread_local EncryptedName<sizeof(source), 0x758F1D0B4F7705B9ULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
    break;
  }
  case 2: {
    const char* message = LookupMessage(3197);
    const std::uint8_t source[] = {0x50, 0xBE, 0xED};
    thread_local EncryptedName<sizeof(source), 0x35E5656B5DEDCD75ULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
    break;
  }
  case 3: {
    const char* message = LookupMessage(3198);
    const std::uint8_t source[] = {0xD8, 0xCA, 0xBB};
    thread_local EncryptedName<sizeof(source), 0x4941FD9F21BBB9FDULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
    break;
  }
  case 4: {
    const char* message = LookupMessage(3199);
    const std::uint8_t source[] = {0xD8, 0xA4, 0x4D};
    thread_local EncryptedName<sizeof(source), 0xE7978FB5A94DD7FDULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
    break;
  }
  case 5: {
    const char* message = LookupMessage(3200);
    const std::uint8_t source[] = {0x4E, 0xCA, 0xC7};
    thread_local EncryptedName<sizeof(source), 0xB7FD8F670FC7B96BULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
    break;
  }
  case 6: {
    const char* message = LookupMessage(3201);
    const std::uint8_t source[] = {0xAA, 0xEA, 0xEB};
    thread_local EncryptedName<sizeof(source), 0x39FF7FB50DEB998FULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
    break;
  }
  case 7: {
    const char* message = LookupMessage(3214);
    const std::uint8_t source[] = {0xB2, 0x82, 0xE1};
    thread_local EncryptedName<sizeof(source), 0x2FE993D1DDE1F197ULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
    break;
  }
  case 8:
  case 100:
  case 102: {
    const char* message = LookupMessage(3202);
    const std::uint8_t source[] = {0xE8, 0x82, 0x99};
    thread_local EncryptedName<sizeof(source), 0xAD075B8DB599F1CDULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
    break;
  }
  case 9: {
    const char* message = LookupMessage(3911);
    const std::uint8_t source[] = {0x5A, 0x66, 0x8D};
    thread_local EncryptedName<sizeof(source), 0x2F8B0F37118D157FULL> format(source);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), format.Text(), message);
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
  const std::uint8_t source[] = {0xBE, 0x24, 0x2B, 0xBC, 0x03, 0x2C, 0x05};
  thread_local EncryptedName<sizeof(source), 0x050505679903579BULL> format(source);
  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatString(format.Text(), text, result), 0, true, false, 0, "Message", 280, 120, 0);
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
void sub_D9A230() {
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
    const std::uint8_t parametersSource[] = {0x7F, 0x25, 0x95, 0x31, 0x03, 0xEC, 0xAC, 0xF5, 0x78, 0x7F, 0x86,
                                             0x2F, 0x53, 0xAC, 0xFA, 0xA9, 0x74, 0x3E, 0x8C, 0x6E, 0x39};
    thread_local EncryptedName<sizeof(parametersSource), 0x8783C33941E15117ULL> parameters(parametersSource);
    const char* parametersText = parameters.Text();
    const std::uint8_t fileSource[] = {0xE3, 0xE9, 0x03, 0xFD, 0xDB, 0xF2, 0x48, 0x65, 0xE4, 0xB3, 0x10,
                                       0xE3, 0x8B, 0xB2, 0x1E, 0x39, 0xE8, 0xF2, 0x1A, 0xA2, 0xE1};
    thread_local EncryptedName<sizeof(fileSource), 0x1767DDE18D779D8BULL> file(fileSource);
    const char* fileText = file.Text();
    const std::uint8_t operationSource[] = {0xAE, 0xD3, 0x94, 0xA3, 0x11};
    thread_local EncryptedName<sizeof(operationSource), 0xC1BB8111EDD183E1ULL> operation(operationSource);
    ShellExecuteA(nullptr, operation.Text(), fileText, parametersText, nullptr, SW_SHOW);
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
    const std::uint8_t codeSource[] = {0xB7, 0xF8, 0xE7, 0x36, 0x77};
    thread_local EncryptedName<sizeof(codeSource), 0x271B45771F83DD9FULL> codeFormat(codeSource);
    message += FormatOwnedString(codeFormat.Text(), error);
    const std::uint8_t detailSource[] = {0xAF, 0x02, 0x44, 0x98, 0xE8, 0x19};
    thread_local EncryptedName<sizeof(detailSource), 0xC70319B5EB61598FULL> detailFormat(detailSource);
    message += FormatOwnedString(detailFormat.Text(), packet->detail);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 5, true, true, 0, "Message", 280, 120, 0);
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
      const std::uint8_t captionSource[] = {0x37, 0x63, 0x53, 0x3B, 0xBC, 0x33, 0xAD};
      thread_local EncryptedName<sizeof(captionSource), 0x75ADC57D87EFBD8FULL> captionText(captionSource);
      caption = captionText.Text();
      protect = true;
    } else {
      const std::uint8_t codeSource[] = {0x79, 0xAE, 0xA9, 0x28, 0xBB};
      thread_local EncryptedName<sizeof(codeSource), 0xD1413DBB01CD8B51ULL> codeFormat(codeSource);
      message += FormatOwnedString(codeFormat.Text(), error);
      caption = "Message";
      protect = false;
    }
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 5, true, true, 0, caption, 280, 120, protect);
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
        const std::uint8_t source[] = {0x83, 0x25, 0x13, 0x4C, 0xC3, 0x86, 0x32, 0x59, 0xCD, 0x32, 0x00, 0x4E,
                                       0xD2, 0xA4, 0x24, 0x4C, 0x9F, 0x11, 0x61};
        thread_local EncryptedName<sizeof(source), 0x2D51E3B7236175EDULL> name(source);
        MessageBoxA(hWnd, message.c_str(), name.Text(), 0);
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
  static_cast<CMode*>(g_charViewMode)->SendMsg(10009, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0);
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
  memcpy_s(g_zoneServerHost, sizeof(g_zoneServerHost), host.c_str(), std::min<std::size_t>(host.size() + 1, sizeof(g_zoneServerHost)));

  GetRagConnection().Close();
  this->pendingCursorType = 12;
}

// OFFSET: 0x00D90740
// FUNCTION: fn_22285
void CLoginMode::ReceiveNotifyZoneServer(const NotifyZoneServerPacket* packet) {
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

// GLOBAL: gl_25133
std::uint32_t g_loginAccountId;

// GLOBAL: gl_25134
std::int32_t g_passwordWndAccountId;

// OFFSET: 0x00D90C90
// FUNCTION: fn_22286
void CLoginMode::SubD90C90(const PasswordModePacket* packet) {
  g_loginAccountId = packet->accountId;
  switch (packet->mode) {
  case 0:
    g_windowMgr.CloseWindow(199);
    g_passwordWndMode = 0;
    this->pendingCursorType = 7;
    break;
  case 1:
    g_passwordWndAccountId = packet->value;
    g_passwordWndMode = 2;
    this->passwordRetryCount = 0;
    this->pendingCursorType = 7;
    break;
  case 2:
    g_passwordWndAccountId = packet->value;
    g_passwordWndMode = 4;
    this->pendingCursorType = 7;
    break;
  case 3:
    g_passwordWndAccountId = packet->value;
    g_passwordWndMode = 3;
    this->pendingCursorType = 7;
    break;
  case 4:
    g_passwordWndAccountId = packet->value;
    g_passwordWndMode = 5;
    this->pendingCursorType = 7;
    break;
  case 5:
    g_windowMgr.CloseWindow(199);
    this->pendingCursorType = 7;
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x768), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.CloseWindow(199);
    this->pendingCursorType = 7;
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x769), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_passwordWndMode = 1;
    this->pendingCursorType = 7;
    break;
  case 8:
    g_windowMgr.CloseWindow(199);
    if (packet->count != 0) {
      this->pendingCursorType = 7;
      std::string message = FormatOwnedString(LookupMessage(0x764), packet->count);
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    } else {
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xD7E), 0, true, false, 0, "Message", 280, 120, 0);
      this->SendMsg(2, 0, 0, 0, 0);
    }
    break;
  case 9: {
    const char* text = LookupMessage(0xC81);
    const std::uint8_t source[] = {0xF2, 0xE6, 0x37, 0xBE, 0x35, 0x6C, 0x87};
    thread_local EncryptedName<sizeof(source), 0x0B8745519B1F95D7ULL> format(source);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatString(format.Text(), text, 9), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 10: {
    const char* text = LookupMessage(0xC81);
    const std::uint8_t source[] = {0xF0, 0x4A, 0x03, 0x6C, 0x83, 0x8E, 0x97};
    thread_local EncryptedName<sizeof(source), 0xA597A7E7492B39D5ULL> format(source);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatString(format.Text(), text, 10), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 11: {
    const char* text = LookupMessage(0xC81);
    const std::uint8_t source[] = {0x56, 0x9C, 0x93, 0xF4, 0xB1, 0xCA, 0xAB};
    thread_local EncryptedName<sizeof(source), 0xFFABE3D5D1BBEF73ULL> format(source);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatString(format.Text(), text, 11), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  }
}

// OFFSET: 0x00D911C0
// FUNCTION: fn_22287
void CLoginMode::SubD911C0(const PasswordModePacket* packet) {
  g_loginAccountId = packet->accountId;
  switch (packet->mode) {
  case 0:
    g_windowMgr.CloseWindow(199);
    g_passwordWndMode = 0;
    this->pendingCursorType = 7;
    break;
  case 1:
    g_passwordWndAccountId = packet->value;
    g_passwordWndMode = 2;
    this->passwordRetryCount = 0;
    this->pendingCursorType = 7;
    break;
  case 2:
    g_passwordWndAccountId = packet->value;
    g_passwordWndMode = 4;
    this->pendingCursorType = 7;
    break;
  case 3:
    g_passwordWndAccountId = packet->value;
    g_passwordWndMode = 3;
    this->pendingCursorType = 7;
    break;
  case 4:
    g_passwordWndAccountId = packet->value;
    g_passwordWndMode = 5;
    this->pendingCursorType = 7;
    break;
  case 5:
    g_windowMgr.CloseWindow(199);
    this->pendingCursorType = 7;
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x768), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.CloseWindow(199);
    this->pendingCursorType = 7;
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x769), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_passwordWndMode = 1;
    this->pendingCursorType = 7;
    break;
  case 8: {
    g_windowMgr.CloseWindow(199);
    ++this->passwordRetryCount;
    const int remaining = 3 - this->passwordRetryCount;
    this->pendingCursorType = 7;
    if (remaining > 0) {
      std::string message = FormatOwnedString(LookupMessage(0x764), remaining);
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    } else {
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x773), 0, true, false, 0, "Message", 280, 120, 0);
      this->SendMsg(2, 0, 0, 0, 0);
    }
    break;
  }
  case 9: {
    const char* text = LookupMessage(0xC81);
    const std::uint8_t source[] = {0x16, 0x10, 0x7D, 0xD4, 0x4D, 0x1C, 0xD3};
    thread_local EncryptedName<sizeof(source), 0x49D33529F1556333ULL> format(source);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatString(format.Text(), text, 9), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 10: {
    const char* text = LookupMessage(0xC81);
    const std::uint8_t source[] = {0x14, 0x8A, 0xFB, 0xDC, 0x79, 0x66, 0xCB};
    thread_local EncryptedName<sizeof(source), 0x01CB4F1DF9D3F931ULL> format(source);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatString(format.Text(), text, 10), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  case 11: {
    const char* text = LookupMessage(0xC81);
    const std::uint8_t source[] = {0xB2, 0xBA, 0xE1, 0xBE, 0xA9, 0x92, 0xB1};
    thread_local EncryptedName<sizeof(source), 0xC5B1BBCD9BC9C997ULL> format(source);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(FormatString(format.Text(), text, 11), 0, true, false, 0, "Message",
                                                          280, 120, 0);
    break;
  }
  }
}

struct SessionItemLists {
  void ClearItems1780();
};

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
  reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId))->Init();
  PreloadCommonSprites();
  if (g_serviceOptions.Get(7)) {
    g_soundMgr->PlayStream("bgm\\01.mp3", 0, false);
  }
  this->slotExtension = CharacterSlotExtension();
  reinterpret_cast<SessionItemLists*>(&g_session)->ClearItems1780();
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

// GLOBAL: gl_25156
int g_hangameLogin;

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
      const std::uint8_t source[] = {0x42, 0x33, 0xC8, 0x47, 0x45};
      thread_local EncryptedName<sizeof(source), 0xD98F5345098D630DULL> operation(source);
      ShellExecuteA(nullptr, operation.Text(), url.c_str(), url.c_str(), nullptr, SW_SHOWNORMAL);
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
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
        if (g_hangameLogin == 1) {
          const std::uint8_t urlSource[] = {0x17, 0x67, 0xAB, 0xA1, 0xBF, 0x48, 0x56, 0x9A, 0x08, 0x64, 0xF1, 0xB9,
                                            0xE4, 0x09, 0x1E, 0x8C, 0x12, 0x76, 0xF1, 0xB2, 0xEA, 0x0A, 0x79};
          thread_local EncryptedName<sizeof(urlSource), 0xED796785D1DF137FULL> url(urlSource);
          const char* file = url.Text();
          const std::uint8_t source[] = {0xD2, 0xCD, 0x88, 0x59, 0xBB};
          thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> operation(source);
          ShellExecuteA(hWnd, operation.Text(), file, nullptr, nullptr, SW_SHOWNORMAL);
        } else {
          const std::uint8_t urlSource[] = {0xF7, 0xB1, 0x19, 0xDB, 0xED, 0x06, 0xD2, 0xD7, 0xFE, 0xA2, 0x03,
                                            0xCA, 0xA5, 0x46, 0x96, 0x8B, 0xFC, 0xAA, 0x43, 0xC0, 0xA5, 0x29};
          thread_local EncryptedName<sizeof(urlSource), 0xA5FD29D7AB6DC59FULL> url(urlSource);
          const char* file = url.Text();
          const std::uint8_t source[] = {0x66, 0x57, 0x4A, 0xD5, 0xBD};
          thread_local EncryptedName<sizeof(source), 0xD97D5DBDBB2F2709ULL> operation(source);
          ShellExecuteA(hWnd, operation.Text(), file, nullptr, nullptr, SW_SHOWNORMAL);
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
      const std::uint8_t textSource[] = {0x4B, 0x36, 0x80, 0xD9, 0x37, 0xC0, 0xB7, 0x39, 0x52, 0xE7, 0x83, 0xA6, 0xA7, 0x9F, 0x6F, 0x39,
                                         0x35, 0xEB, 0x19, 0xBD, 0xCE, 0xC4, 0x8C, 0xD1, 0x30, 0x65, 0x85, 0xDA, 0x37, 0xBE, 0x97, 0x30,
                                         0x27, 0x7A, 0xFA, 0xB8, 0xF5, 0x59, 0xF6, 0x28, 0x47, 0x3F, 0x8D, 0xC6, 0xA3, 0xA0, 0x61, 0xF1};
      thread_local EncryptedName<sizeof(textSource), 0xF14F79170939C7F1ULL> text(textSource);
      message = std::string(text.Text());
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
        const std::uint8_t urlSource[] = {0x43, 0x6B, 0x9D, 0xDB, 0xFB, 0xD0, 0x02, 0x08, 0x5C, 0x68, 0xC7, 0xD9, 0xA0, 0x98, 0x43, 0x1E,
                                          0x59, 0x70, 0x82, 0x85, 0xA2, 0x90, 0x03, 0x14, 0x59, 0x30, 0x9A, 0xCE, 0xB5, 0xB1, 0x48, 0x08,
                                          0x7B, 0x48, 0xC6, 0xDB, 0xA0, 0x8C, 0x5E, 0x08, 0x44, 0x6D, 0x8D, 0xF4, 0xA2, 0x97, 0x4C, 0x11,
                                          0x4C, 0x7A, 0xB6, 0xCA, 0xA6, 0x8D, 0x48, 0x1A, 0x46, 0x7A, 0x87, 0xDF, 0xEF, 0x9E, 0x5E, 0x0F,
                                          0x2B};
        thread_local EncryptedName<sizeof(urlSource), 0x7F2DFFC1ABE91F2BULL> url(urlSource);
        const char* file = url.Text();
        const std::uint8_t source[] = {0xE2, 0xF1, 0x74, 0x9D, 0xB3};
        thread_local EncryptedName<sizeof(source), 0xFD69A9B3F311818DULL> operation(source);
        ShellExecuteA(hWnd, operation.Text(), file, nullptr, nullptr, SW_SHOWNORMAL);
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
    if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 2, true, true, 0, "Message", 280, 120, 0) == 0xBB) {
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
    if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x929), 0, true, true, 0, "Message", 280, 120, 0) == 0xB8) {
      const char* file = LookupMessage(0x92A);
      const std::uint8_t source[] = {0xFC, 0x05, 0xBA, 0x81, 0x4D};
      thread_local EncryptedName<sizeof(source), 0x9B0F234DEFDF7593ULL> operation(source);
      ShellExecuteA(hWnd, operation.Text(), file, nullptr, nullptr, SW_SHOWNORMAL);
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
    const std::uint8_t source[] = {0x6B, 0x1C, 0x3F, 0x42, 0x67};
    thread_local EncryptedName<sizeof(source), 0xC9712D676B5B3943ULL> format(source);
    message += FormatOwnedString(format.Text(), code);
    break;
  }

  }

  if (!message.empty()) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
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
