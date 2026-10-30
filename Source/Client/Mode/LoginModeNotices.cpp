#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

namespace {
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

#pragma pack(push, 1)
  struct BillingInfoPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint32_t remainingTime;
    /* 0006 */ std::uint32_t remainingFreeTime;
  };

  struct NotifyBanPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint8_t errorCode;
  };
#pragma pack(pop)

  static_assert(sizeof(BillingInfoPacket) == 10);
  static_assert(sizeof(NotifyBanPacket) == 3);
}

// GLOBAL: gl_25059
bool g_loginModeFlag;

// OFFSET: 0x00D9A350
// FUNCTION: fn_22311
void CLoginMode::ReceiveBillingInfo(const unsigned char* data) {
  const BillingInfoPacket* packet = reinterpret_cast<const BillingInfoPacket*>(data);
  char text[256];
  const std::uint32_t remainingTime = packet->remainingTime;

  if (remainingTime == 0xFFFFFFFE && packet->remainingFreeTime == 0xFFFFFFFE) {
    const char* message = LookupMessage(680);
    const std::uint8_t source[] = {0x34, 0x90, 0x59, 0x99};
    // GLOBAL: gl_25061
    thread_local EncryptedName<sizeof(source), 0x4B755DB19953E311ULL> format(source);
    sprintf(text, format.Text(), message);
  } else if (remainingTime == 0xFFFFFFFD && packet->remainingFreeTime == 0xFFFFFFFD) {
    const char* message = LookupMessage(1052);
    const std::uint8_t source[] = {0x7E, 0xC4, 0x6B, 0x59};
    // GLOBAL: gl_25062
    thread_local EncryptedName<sizeof(source), 0xF715514F5961B75BULL> format(source);
    sprintf(text, format.Text(), message);
  } else if (remainingTime == 0xFFFFFFFF && packet->remainingFreeTime == 0xFFFFFFFF) {
    const char* message = LookupMessage(725);
    const std::uint8_t source[] = {0xBC, 0x4A, 0xF7, 0x31};
    // GLOBAL: gl_25063
    thread_local EncryptedName<sizeof(source), 0x4191BB5531FD3999ULL> format(source);
    sprintf(text, format.Text(), message);
  } else if (remainingTime == 0) {
    if (packet->remainingFreeTime > 0) {
      const char* message = LookupMessage(541);
      const std::uint8_t source[] = {0x8A, 0x7A, 0x03, 0xE7};
      // GLOBAL: gl_25065
      thread_local EncryptedName<sizeof(source), 0x9569C9C7E70909AFULL> format(source);
      sprintf(text, format.Text(), message);
      const std::uint32_t freeTime = packet->remainingFreeTime;
      sprintf(text + strlen(text), LookupMessage(679), freeTime / 60, freeTime % 60);
    } else {
      const std::uint8_t source[] = {0x67, 0xBF};
      // GLOBAL: gl_25066
      thread_local EncryptedName<sizeof(source), 0xD7A749EB27D5BF6DULL> format(source);
      sprintf(text, format.Text());
    }
  } else {
    const std::uint32_t freeTime = packet->remainingFreeTime;
    if (freeTime != 0) {
      sprintf(
        text, LookupMessage(726), remainingTime / 1440, remainingTime / 60 % 24, remainingTime % 60, freeTime / 60, freeTime % 60
      );
    } else {
      const char* message = LookupMessage(533);
      const std::uint8_t source[] = {0xF6, 0x4A, 0x65, 0x11};
      // GLOBAL: gl_25064
      thread_local EncryptedName<sizeof(source), 0xB19D1FA9116F39D3ULL> format(source);
      sprintf(text, format.Text(), message);
      const std::uint32_t time = packet->remainingTime;
      sprintf(text + strlen(text), LookupMessage(677), time / 1440, time / 60 % 24, time % 60);
    }
  }

  this->otpStarted = timeGetTime() + 90000;
  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
}

// OFFSET: 0x00D9A9F0
// FUNCTION: fn_22312
void CLoginMode::ReceiveNotifyBan(const unsigned char* data) {
  const NotifyBanPacket* packet = reinterpret_cast<const NotifyBanPacket*>(data);
  char text[256];
  text[0] = '\0';
  const std::uint8_t errorCode = packet->errorCode;
  GetRagConnection().Close();

  switch (errorCode) {
  case 0:
    strcpy(text, LookupMessage(3));
    break;
  case 1:
    strcpy(text, LookupMessage(4));
    break;
  case 2:
    strcpy(text, LookupMessage(5));
    break;
  case 3:
    strcpy(text, LookupMessage(241));
    break;
  case 4:
    strcpy(text, LookupMessage(264));
    break;
  case 5:
    strcpy(text, LookupMessage(305));
    break;
  case 6:
    strcpy(text, LookupMessage(438));
    if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 2, true, true, 0, "Message", 280, 120, 0) == 187) {
      g_modeMgr.Quit();
      OpenAccountRegistration();
    }
    text[0] = '\0';
    break;
  case 7:
    strcpy(text, LookupMessage(439));
    break;
  case 8:
    strcpy(text, LookupMessage(440));
    break;
  case 9:
    strcpy(text, LookupMessage(529));
    break;
  case 10:
    strcpy(text, LookupMessage(530));
    break;
  case 11:
    strcpy(text, LookupMessage(575));
    break;
  case 12:
    strcpy(text, LookupMessage(576));
    break;
  case 13:
    strcpy(text, LookupMessage(577));
    break;
  case 14:
    strcpy(text, LookupMessage(578));
    break;
  case 16:
    strcpy(text, LookupMessage(606));
    break;
  case 17:
    strcpy(text, LookupMessage(607));
    break;
  case 18:
    strcpy(text, LookupMessage(678));
    break;
  case 100:
    strcpy(text, LookupMessage(1123));
    break;
  case 101:
    strcpy(text, LookupMessage(1178));
    break;
  case 102:
    strcpy(text, LookupMessage(1179));
    break;
  case 103:
    strcpy(text, LookupMessage(1309));
    break;
  case 104:
    strcpy(text, LookupMessage(1310));
    break;
  case 105:
    strcpy(text, LookupMessage(1311));
    break;
  case 107:
    if (g_serviceType == 0) {
      char buffer[200];
      sprintf(buffer, LookupMessage(1429), 9);
      strcpy(text, buffer);
    }
    break;
  case 108:
    strcpy(text, LookupMessage(1582));
    break;
  case 109:
    strcpy(text, LookupMessage(1583));
    break;
  case 110:
    strcpy(text, LookupMessage(1589));
    break;
  case 113:
    strcpy(text, LookupMessage(2021));
    break;
  case 114:
    strcpy(text, LookupMessage(2344));
    break;
  case 116:
    strcpy(text, LookupMessage(3635));
    break;
  default:
    strcpy(text, LookupMessage(3));
    break;
  }

  if (strlen(text) != 0) {
    const std::uint8_t source[] = {0x1E, 0x94, 0x29, 0x3E, 0xA5, 0x88, 0x2B};
    // GLOBAL: gl_25060
    thread_local EncryptedName<sizeof(source), 0xBD2BA1C11B01E73BULL> format(source);
    char message[256];
    sprintf(message, format.Text(), text, packet->errorCode);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message, 0, true, true, 0, "Message", 280, 120, 0);
  }

  if (g_loginModeFlag == true) {
    this->pendingCursorType = 3;
  } else {
    g_modeMgr.gameMode->SendMsg(2, 0, 0, 0, 0);
  }
}
