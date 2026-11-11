#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMessage.h"
#include "Network/Packets/NoticeEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include "Resource/MSGSTRINGID.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

namespace {

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

// OFFSET: 0x00D9A350
// FUNCTION: fn_22311
void CLoginMode::ReceiveBillingInfo(const unsigned char* data) {
  const BillingInfoPacket* packet = reinterpret_cast<const BillingInfoPacket*>(data);
  char text[256];
  const std::uint32_t remainingTime = packet->remainingTime;

  if (remainingTime == 0xFFFFFFFE && packet->remainingFreeTime == 0xFFFFFFFE) {
    const char* message = LookupMessage(MSI_BILLING_FREE_USER);
    const char* format = [] {
      constexpr std::array<std::uint8_t, 4> sourceArray = CRYPT_NAME("%s\x0A", 0x4B755DB19953E311ULL);
      const std::uint8_t (&source)[4] = sourceArray._Elems;
      // GLOBAL: gl_25061
      thread_local EncryptedName<sizeof(source), 0x4B755DB19953E311ULL> name(source);
      return name.Text();
    }();
    sprintf(text, format, message);
  } else if (remainingTime == 0xFFFFFFFD && packet->remainingFreeTime == 0xFFFFFFFD) {
    const char* message = LookupMessage(MSI_BILLING_PCB);
    const char* format = [] {
      constexpr std::array<std::uint8_t, 4> sourceArray = CRYPT_NAME("%s\x0A", 0xF715514F5961B75BULL);
      const std::uint8_t (&source)[4] = sourceArray._Elems;
      // GLOBAL: gl_25062
      thread_local EncryptedName<sizeof(source), 0xF715514F5961B75BULL> name(source);
      return name.Text();
    }();
    sprintf(text, format, message);
  } else if (remainingTime == 0xFFFFFFFF && packet->remainingFreeTime == 0xFFFFFFFF) {
    const char* message = LookupMessage(MSI_BILLING_INTERNET_CAFE);
    const char* format = [] {
      constexpr std::array<std::uint8_t, 4> sourceArray = CRYPT_NAME("%s\x0A", 0x4191BB5531FD3999ULL);
      const std::uint8_t (&source)[4] = sourceArray._Elems;
      // GLOBAL: gl_25063
      thread_local EncryptedName<sizeof(source), 0x4191BB5531FD3999ULL> name(source);
      return name.Text();
    }();
    sprintf(text, format, message);
  } else if (remainingTime != 0) {
    const std::uint32_t freeTime = packet->remainingFreeTime;
    if (freeTime != 0) {
      const char* message = LookupMessage(MSI_BILLING_BOTH);
      sprintf(text, message, remainingTime / (60 * 24), remainingTime / 60 % 24, remainingTime % 60, freeTime / 60, freeTime % 60);
    } else {
      const char* message = LookupMessage(MSI_BILLING_100);
      const char* format = [] {
        constexpr std::array<std::uint8_t, 4> sourceArray = CRYPT_NAME("%s\x0A", 0xB19D1FA9116F39D3ULL);
        const std::uint8_t (&source)[4] = sourceArray._Elems;
        // GLOBAL: gl_25064
        thread_local EncryptedName<sizeof(source), 0xB19D1FA9116F39D3ULL> name(source);
        return name.Text();
      }();
      sprintf(text, format, message);
      const std::uint32_t time = packet->remainingTime;
      const char* dayFormat = LookupMessage(MSI_BILLING_DAY_HOUR_MINUTES);
      sprintf(text + strlen(text), dayFormat, time / (60 * 24), time / 60 % 24, time % 60);
    }
  } else {
    if (packet->remainingFreeTime > 0) {
      const char* message = LookupMessage(MSI_BILLING_800);
      const char* format = [] {
        constexpr std::array<std::uint8_t, 4> sourceArray = CRYPT_NAME("%s\x0A", 0x9569C9C7E70909AFULL);
        const std::uint8_t (&source)[4] = sourceArray._Elems;
        // GLOBAL: gl_25065
        thread_local EncryptedName<sizeof(source), 0x9569C9C7E70909AFULL> name(source);
        return name.Text();
      }();
      sprintf(text, format, message);
      const std::uint32_t freeTime = packet->remainingFreeTime;
      const char* hourFormat = LookupMessage(MSI_BILLING_HOUR_MINUTES);
      sprintf(text + strlen(text), hourFormat, freeTime / 60, freeTime % 60);
    } else {
      const char* format = [] {
        constexpr std::array<std::uint8_t, 2> sourceArray = CRYPT_NAME("\x0A", 0xD7A749EB27D5BF6DULL);
        const std::uint8_t (&source)[2] = sourceArray._Elems;
        // GLOBAL: gl_25066
        thread_local EncryptedName<sizeof(source), 0xD7A749EB27D5BF6DULL> name(source);
        return name.Text();
      }();
      sprintf(text, format);
    }
  }

  this->otpStarted = timeGetTime() + 90000;
  g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 280, 120, 0);
}

// OFFSET: 0x00D9A9F0
// FUNCTION: fn_22312
void CLoginMode::ReceiveNotifyBan(const unsigned char* data) {
  const NotifyBanPacket* packet = reinterpret_cast<const NotifyBanPacket*>(data);
  char text[256];
  text[0] = '\0';
  const int errorCode = packet->errorCode;
  GetRagConnection().Close();

  switch (errorCode) {
  case BAN_UNFAIR:
    strcpy(text, LookupMessage(MSI_BANNED));
    break;
  case BAN_SERVER_SHUTDOWN:
    strcpy(text, LookupMessage(MSI_SERVER_OFF));
    break;
  case BAN_ANOTHER_LOGIN:
    strcpy(text, LookupMessage(MSI_DOUBLE_LOGIN_PROHIBITED));
    break;
  case BAN_DETECTED_SPEEDHACK:
    strcpy(text, LookupMessage(MSI_SPEEDHACK));
    break;
  case BAN_PC_OVERFLOW:
    strcpy(text, LookupMessage(MSI_PC_OVERFLOW));
    break;
  case BAN_UNDER_AGE:
    strcpy(text, LookupMessage(MSI_UNDER_AGE));
    break;
  case BAN_NOT_SETTLED:
    strcpy(text, LookupMessage(MSI_NOT_SETTLED));
    if (g_windowMgr.Confirm(text, 2, true, true, 0, "Message", 280, 120, 0) == 187) {
      g_modeMgr.Quit();
      this->OpenAccountRegistration();
    }
    text[0] = '\0';
    break;
  case BAN_ACCOUNT_BUSY:
    strcpy(text, LookupMessage(MSI_ACCOUNT_BUSY));
    break;
  case BAN_INFORMATION_REMAINED:
    strcpy(text, LookupMessage(MSI_INFORMATION_REMAINED));
    break;
  case BAN_IP_OVERFLOW:
    strcpy(text, LookupMessage(MSI_BAN_IP_OVERFLOW));
    break;
  case BAN_PAY_OUT:
    strcpy(text, LookupMessage(MSI_BAN_PAY_OUT));
    break;
  case BAN_PAY_SUSPEND:
    strcpy(text, LookupMessage(MSI_BAN_PAY_SUSPEND));
    break;
  case BAN_PAY_CHANGE:
    strcpy(text, LookupMessage(MSI_BAN_PAY_CHANGE));
    break;
  case BAN_PAY_WRONGIP:
    strcpy(text, LookupMessage(MSI_BAN_PAY_WRONGIP));
    break;
  case BAN_PAY_PNGAMEROOM:
    strcpy(text, LookupMessage(MSI_BAN_PAY_PNGAMEROOM));
    break;
  case BAN_JAPAN_REFUSE1:
    strcpy(text, LookupMessage(MSI_BAN_JAPAN_REFUSE1));
    break;
  case BAN_JAPAN_REFUSE2:
    strcpy(text, LookupMessage(MSI_BAN_JAPAN_REFUSE2));
    break;
  case BAN_INFORMATION_REMAINED_ANOTHER_ACCOUNT:
    strcpy(text, LookupMessage(MSI_BAN_INFORMATION_REMAINED_ANOTHER_ACCOUNT));
    break;
  case BAN_PC_IP_UNFAIR:
    strcpy(text, LookupMessage(MSI_BAN_PC_IP_UNFAIR));
    break;
  case BAN_PC_IP_COUNT_ALL:
    strcpy(text, LookupMessage(MSI_BAN_PC_IP_COUNT_ALL));
    break;
  case BAN_PC_IP_COUNT:
    strcpy(text, LookupMessage(MSI_BAN_PC_IP_COUNT));
    break;
  case BAN_GRAVITY_MEM_AGREE:
    strcpy(text, LookupMessage(MSI_BAN_GRAVITY_MEM_AGREE));
    break;
  case BAN_GAME_MEM_AGREE:
    strcpy(text, LookupMessage(MSI_BAN_GAME_MEM_AGREE));
    break;
  case BAN_HAN_VALID:
    strcpy(text, LookupMessage(MSI_BAN_HAN_VALID));
    break;
  case 107:
    if (g_serviceType == 0) {
      char buffer[200];
      sprintf(buffer, LookupMessage(MSI_BAN_OVER_CHARACTER_LIST), 9);
      strcpy(text, buffer);
    }
    break;
  case 108:
    strcpy(text, LookupMessage(MSI_BAN_IP_BLOCK));
    break;
  case 109:
    strcpy(text, LookupMessage(MSI_BAN_INVALID_PWD_CNT));
    break;
  case 110:
    strcpy(text, LookupMessage(MSI_BAN_NOT_ALLOWED_JOBCLASS));
    break;
  case 113:
    strcpy(text, LookupMessage(MSI_BAN_SHUTDOWN_TEENAGER));
    break;
  case 114:
    strcpy(text, LookupMessage(MSI_PLANNED_TIME_SHUTDOWN));
    break;
  case 116:
    strcpy(text, LookupMessage(MSI_MCSTORE_BACKUP_TOOLTIP));
    break;
  default:
    strcpy(text, LookupMessage(MSI_BANNED));
    break;
  }

  if (strlen(text) != 0) {
    constexpr std::array<std::uint8_t, 7> source = CRYPT_NAME("%s(%d)", 0xBD2BA1C11B01E73BULL);
    // GLOBAL: gl_25060
    thread_local EncryptedName<7, 0xBD2BA1C11B01E73BULL> format(source);
    char message[256];
    sprintf(message, format.Text(), text, errorCode);
    g_windowMgr.Confirm(message, 0, true, true, 0, "Message", 280, 120, 0);
  }

  if (g_session.m_loginModeFlag == true) {
    this->pendingCursorType = 3;
  } else {
    g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
  }
}
