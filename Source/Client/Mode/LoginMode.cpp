#include "Client/Mode/CLoginMode.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Contents/Sound/SoundMgr.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>

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
#pragma pack(pop)

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

// OFFSET: 0x00D8D4F0
// FUNCTION: fn_22265
void CLoginMode::SubD8D4F0(int) {
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
