#include "Client/Mode/CLoginMode.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <string>
#include <winsock2.h>
#include <windows.h>

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

struct CharacterListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ CharacterInfo characters[1];
};
#pragma pack(pop)

static_assert(sizeof(CharacterSlotsPacket) == 0x1D);

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

// OFFSET: 0x00D91780
// FUNCTION: fn_22288
void CLoginMode::SubD91780(int) {
}
