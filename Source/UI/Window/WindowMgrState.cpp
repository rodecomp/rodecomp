#include "UI/Window/UIWindowMgr.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/WindowDialogs.h"
#include "Client/Registry.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "UI/Dialogs/UIMemorialDunWnd.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UINavigationWnd.h"
#include "UI/Window/UIRoulletteWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "Render/Renderer.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/World/World.h"
#include "Contents/Actors/Pc.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/Surface.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/WindowMouseHit.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Mode/ModeDragState.h"
#include "Contents/UserData/NormalItemSlots.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>
#include <windows.h>

// GLOBAL: gl_24986
const char* g_uiRectInfoKeySuffix = "\\UIRectInfo";

// GLOBAL: gl_24987
const char* g_shortcutItemKeySuffix = "\\ShortcutItem";

struct ClientItemLists {
  void ResetHotkeyDescriptions();
};

// OFFSET: 0x00A911E0
// FUNCTION: fn_17156
void UIWindowMgr::NotifyHoverWindow() {
  if (this->hoverWindow) {
    this->hoverWindow->Slot24(0);
  }
}

// OFFSET: 0x00A91400
// FUNCTION: fn_17159
UIWindow* UIWindowMgr::GetGuildWindow() {
  if (this->window_0320) {
    return this->window_0320;
  }
  if (this->window_0324) {
    return this->window_0324;
  }
  if (this->unknown_0328) {
    return this->unknown_0328;
  }
  if (this->window_032C) {
    return this->window_032C;
  }
  if (this->unknown_0330) {
    return this->unknown_0330;
  }
  if (this->unknown_0334) {
    return this->unknown_0334;
  }
  return this->unknown_0338;
}

// OFFSET: 0x00A91670
// FUNCTION: fn_17162
int UIWindowMgr::FindMiniPartyCell() {
  if (!g_miniPartyWndAreaMgr) {
    g_miniPartyWndAreaMgr = new (std::nothrow) CMiniPartyWndAreaMgr;
  }
  return g_miniPartyWndAreaMgr->FindFreeCell();
}

// OFFSET: 0x00A916F0
// FUNCTION: fn_17163
UIWindow* UIWindowMgr::GetHoverWindow() {
  return this->hoverWindow;
}

// OFFSET: 0x00A91C10
// FUNCTION: fn_17169
int UIWindowMgr::GetRenderedWindowIds(int* ids) {
  if (ids == nullptr) {
    return 0;
  }

  for (UIWindow* window : this->rendered) {
    *ids++ = window->geometry.id;
  }

  return static_cast<int>(this->rendered.size());
}

// OFFSET: 0x00A91C50
// FUNCTION: fn_17170
void UIWindowMgr::HideWindow01C8() {
  if (this->window_01C8) {
    this->window_01C8->Show(0);
  }
}

// OFFSET: 0x00A91D90
// FUNCTION: fn_17172
bool UIWindowMgr::HasUnknown030C() {
  const std::uint32_t* unknown = reinterpret_cast<const std::uint32_t*>(this->unknown_030C);
  return unknown != nullptr && unknown[1] != 0;
}

// OFFSET: 0x00A91F60
// FUNCTION: fn_17175
void UIWindowMgr::RestoreMiniPartyWindows() {
  const int count = static_cast<int>(g_session.unknown_0734.size());
  WindowDialogs& dialogs = *reinterpret_cast<WindowDialogs*>(this);
  for (int index = 0; index < count; ++index) {
    MessengerMember member = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyAt(index);
    if (member.valid == 0) {
      continue;
    }

    const SessionRecord0734 record = g_session.FindRecord0734(member.accountId);
    UIMiniPartyWnd* window = new UIMiniPartyWnd(dialogs.windows, dialogs.frames);
    window->SetSize(174, 52);
    window->m_cell = record.unknown_0008;
    this->AddMiniPartyWindow(window);
    window->SendMsg(nullptr, 0x17, reinterpret_cast<std::intptr_t>(&member), 0, 0, 0);
  }
}

// OFFSET: 0x00A92090
// FUNCTION: fn_17176
void UIWindowMgr::LoadShortcutItems() {
  char path[512];
  std::strcpy(path, g_subKey);
  std::strcat(path, g_shortcutItemKeySuffix);
  std::strcat(path, "\\");
  if (std::strstr(g_session.m_serverName.c_str(), "사크라이") != nullptr) {
    std::strcat(path, "사크라이");
  } else {
    std::strcat(path, g_session.m_serverName.c_str());
  }

  HKEY probe;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &probe) == ERROR_SUCCESS) {
    RegCloseKey(probe);
    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
      reinterpret_cast<ClientItemLists*>(&g_session)->ResetHotkeyDescriptions();
      g_session.ClearUnknown0420();
      for (int index = 0; index < 36; ++index) {
        DWORD type = REG_SZ;
        DWORD size = 32;
        char name[36];
        std::sprintf(name, "SITEM%d%d%d", index, g_serviceType, g_session.unknown_0F50);
        char description[33] = {};
        LSTATUS result = RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(description), &size);
        bool hasSkillLevel = false;
        g_session.m_hotkeyPage = static_cast<unsigned char>(index / 9);
        if (result == ERROR_SUCCESS) {
          if (description[0] == '#') {
            hasSkillLevel = true;
          } else {
            g_session.SetHotkeyDescription(description, index);
          }
        } else {
          g_session.SetHotkeyDescription("nothing", index);
        }

        type = REG_DWORD;
        size = 4;
        std::sprintf(name, "S_SKILL_USE_LEVEL%d%d%d", index, g_serviceType, g_session.unknown_0F50);
        DWORD level = 0;
        if (RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(&level), &size) == ERROR_SUCCESS) {
          g_session.SetHotkeySkillLevel(level, index);
        } else {
          g_session.SetHotkeySkillLevel(0, index);
          level = 0;
        }
        if (hasSkillLevel) {
          g_session.SetHotkeyDescription(description, level, index);
        }
      }
      RegCloseKey(key);
    }
  }
  g_session.m_hotkeyPage = 0;
}

// OFFSET: 0x00A92630
// FUNCTION: fn_17178
void UIWindowMgr::LoadUIRectInfo() {
  BYTE* const base = reinterpret_cast<BYTE*>(this);
  char path[512];
  std::strcpy(path, g_subKey);
  std::strcat(path, g_uiRectInfoKeySuffix);

  HKEY probe;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &probe) != ERROR_SUCCESS) {
    this->ResetUIRectInfo();
    return;
  }

  RegCloseKey(probe);
  HKEY key;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS) {
    return;
  }

  DWORD type = REG_DWORD;
  DWORD size = 4;
  char name[256];
  RegQueryValueExA(key, "m_minimapZoomWnd", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0050), &size);
  if (RegQueryValueExA(key, "M_ISDRAGALL", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0064), &size) != ERROR_SUCCESS || this->unknown_0064 != 0) {
    this->unknown_0064 = 1;
  }
  if (RegQueryValueExA(key, "M_ISDRAWCOMPASS", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0060), &size) != ERROR_SUCCESS) {
    this->unknown_0060 = 0;
  }
  if (RegQueryValueExA(key, "M_MINIMAPARGB", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_005C), &size) != ERROR_SUCCESS || this->unknown_005C != 0xFFFFFFFF) {
    this->unknown_005C = 0xAAFFFFFF;
  }
  RegQueryValueExA(key, "M_CHATWNDSTATUS", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_004C), &size);
  RegQueryValueExA(key, "M_CHATWNDHEIGHT", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0038), &size);
  RegQueryValueExA(key, "M_CHATWNDX", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0030), &size);
  RegQueryValueExA(key, "M_CHATWNDY", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0034), &size);
  RegQueryValueExA(key, "M_CHATWNDSHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_003C), &size);
  RegQueryValueExA(key, "SAYDIALOGWNDINFO.X", nullptr, &type, base + 0x5E4, &size);
  RegQueryValueExA(key, "SAYDIALOGWNDINFO.Y", nullptr, &type, base + 0x5E8, &size);
  RegQueryValueExA(key, "SAYDIALOGWNDINFO.W", nullptr, &type, base + 0x5EC, &size);
  RegQueryValueExA(key, "SAYDIALOGWNDINFO.H", nullptr, &type, base + 0x5F0, &size);
  RegQueryValueExA(key, "CHOOSEWNDINFO.X", nullptr, &type, base + 0x5F4, &size);
  RegQueryValueExA(key, "CHOOSEWNDINFO.Y", nullptr, &type, base + 0x5F8, &size);
  RegQueryValueExA(key, "CHOOSEWNDINFO.W", nullptr, &type, base + 0x5FC, &size);
  RegQueryValueExA(key, "CHOOSEWNDINFO.H", nullptr, &type, base + 0x600, &size);
  RegQueryValueExA(key, "BASICINFOWNDINFO.X", nullptr, &type, base + 0x514, &size);
  RegQueryValueExA(key, "BASICINFOWNDINFO.Y", nullptr, &type, base + 0x518, &size);
  RegQueryValueExA(key, "BASICINFOWNDINFO.W", nullptr, &type, base + 0x51C, &size);
  RegQueryValueExA(key, "BASICINFOWNDINFO.H", nullptr, &type, base + 0x520, &size);
  RegQueryValueExA(key, "BASICINFOWNDINFO.ISSHOW", nullptr, &type, base + 0x524, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.X", nullptr, &type, base + 0x528, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.Y", nullptr, &type, base + 0x52C, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.W", nullptr, &type, base + 0x530, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.H", nullptr, &type, base + 0x534, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.ORGHEIGHT", nullptr, &type, base + 0x538, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.CURTAB", nullptr, &type, base + 0x53C, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.CURRADIO1", nullptr, &type, base + 0x540, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.CURRADIO2", nullptr, &type, base + 0x544, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.CURRADIO3", nullptr, &type, base + 0x548, &size);
  RegQueryValueExA(key, "ITEMWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_054C), &size);
  RegQueryValueExA(key, "QUESTWNDINFO.X", nullptr, &type, base + 0x550, &size);
  RegQueryValueExA(key, "QUESTWNDINFO.Y", nullptr, &type, base + 0x554, &size);
  RegQueryValueExA(key, "QUESTWNDINFO.W", nullptr, &type, base + 0x558, &size);
  RegQueryValueExA(key, "QUESTWNDINFO.H", nullptr, &type, base + 0x55C, &size);
  RegQueryValueExA(key, "QUESTWNDINFO.ORGHEIGHT", nullptr, &type, base + 0x560, &size);
  RegQueryValueExA(key, "QUESTWNDINFO.CURTAB", nullptr, &type, base + 0x564, &size);
  RegQueryValueExA(key, "QUESTWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0568), &size);
  RegQueryValueExA(key, "QUESTDISPWNDINFO.QUESTDISPLAY", nullptr, &type, base + 0x570, &size);
  RegQueryValueExA(key, "STATUSWNDINFO.X", nullptr, &type, base + 0x574, &size);
  RegQueryValueExA(key, "STATUSWNDINFO.Y", nullptr, &type, base + 0x578, &size);
  RegQueryValueExA(key, "STATUSWNDINFO.W", nullptr, &type, base + 0x57C, &size);
  RegQueryValueExA(key, "STATUSWNDINFO.H", nullptr, &type, base + 0x580, &size);
  RegQueryValueExA(key, "STATUSWNDINFO.ORGHEIGHT", nullptr, &type, base + 0x584, &size);
  if (RegQueryValueExA(key, "STATUSWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0588), &size) != ERROR_SUCCESS) {
    this->unknown_0588 = 0;
  }
  RegQueryValueExA(key, "EQUIPWNDINFO.X", nullptr, &type, base + 0x590, &size);
  RegQueryValueExA(key, "EQUIPWNDINFO.Y", nullptr, &type, base + 0x594, &size);
  RegQueryValueExA(key, "EQUIPWNDINFO.W", nullptr, &type, base + 0x598, &size);
  RegQueryValueExA(key, "EQUIPWNDINFO.H", nullptr, &type, base + 0x59C, &size);
  RegQueryValueExA(key, "EQUIPWNDINFO.ORGHEIGHT", nullptr, &type, base + 0x5A0, &size);
  if (RegQueryValueExA(key, "EQUIPWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_05A4), &size) != ERROR_SUCCESS) {
    this->unknown_05A4 = 0;
  }
  RegQueryValueExA(key, "OPTIONWNDINFO.X", nullptr, &type, base + 0x5A8, &size);
  RegQueryValueExA(key, "OPTIONWNDINFO.Y", nullptr, &type, base + 0x5AC, &size);
  RegQueryValueExA(key, "OPTIONWNDINFO.W", nullptr, &type, base + 0x5B0, &size);
  RegQueryValueExA(key, "OPTIONWNDINFO.H", nullptr, &type, base + 0x5B4, &size);
  RegQueryValueExA(key, "OPTIONWNDINFO.ORGHEIGHT", nullptr, &type, base + 0x5B8, &size);
  if (RegQueryValueExA(key, "OPTIONWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_05BC), &size) != ERROR_SUCCESS) {
    this->unknown_05BC = 0;
  }
  RegQueryValueExA(key, "SHORTENITEMWNDINFO.X", nullptr, &type, base + 0x5C0, &size);
  RegQueryValueExA(key, "SHORTENITEMWNDINFO.Y", nullptr, &type, base + 0x5C4, &size);
  RegQueryValueExA(key, "SHORTENITEMWNDINFO.W", nullptr, &type, base + 0x5C8, &size);
  RegQueryValueExA(key, "SHORTENITEMWNDINFO.H", nullptr, &type, base + 0x5CC, &size);
  if (RegQueryValueExA(key, "SHORTENITEMWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_05D0), &size) != ERROR_SUCCESS) {
    this->unknown_05D0 = 0;
  }
  RegQueryValueExA(key, "CHATROOMWNDINFO.X", nullptr, &type, base + 0x604, &size);
  RegQueryValueExA(key, "CHATROOMWNDINFO.Y", nullptr, &type, base + 0x608, &size);
  RegQueryValueExA(key, "CHATROOMWNDINFO.W", nullptr, &type, base + 0x60C, &size);
  RegQueryValueExA(key, "CHATROOMWNDINFO.H", nullptr, &type, base + 0x610, &size);
  RegQueryValueExA(key, "ITEMSTOREWNDINFO.X", nullptr, &type, base + 0x614, &size);
  RegQueryValueExA(key, "ITEMSTOREWNDINFO.Y", nullptr, &type, base + 0x618, &size);
  RegQueryValueExA(key, "ITEMSTOREWNDINFO.W", nullptr, &type, base + 0x61C, &size);
  RegQueryValueExA(key, "ITEMSTOREWNDINFO.H", nullptr, &type, base + 0x620, &size);
  RegQueryValueExA(key, "ITEMSTOREWNDINFO.subWnd", nullptr, &type, base + 0x624, &size);
  std::memset(name, 0, sizeof(name) - 1);
  RegQueryValueExA(key, "ITEMSTOREWNDNUM", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_0058), &size);
  if (this->unknown_0058 == 8) {
    for (int index = 0; index < 8; ++index) {
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.X", index);
      RegQueryValueExA(key, name, nullptr, &type, base + 0x62C + index * 0x10, &size);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.Y", index);
      RegQueryValueExA(key, name, nullptr, &type, base + 0x630 + index * 0x10, &size);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.W", index);
      RegQueryValueExA(key, name, nullptr, &type, base + 0x634 + index * 0x10, &size);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.H", index);
      RegQueryValueExA(key, name, nullptr, &type, base + 0x638 + index * 0x10, &size);
    }
  } else {
    ItemStoreFindSlotInfo* const storeSlots = reinterpret_cast<ItemStoreFindSlotInfo*>(base + 0x62C);
    for (int index = 0; index < 8; ++index) {
      storeSlots[index].Reset(index);
    }
    this->unknown_0058 = 8;
    reinterpret_cast<ItemStoreFindListInfo*>(base + 0x614)->Reset();
  }
  RegQueryValueExA(key, "ITEMSTOREWNDINFO.findWnd", nullptr, &type, base + 0x628, &size);
  RegQueryValueExA(key, "ITEMSTOREFINDWNDINFO.X", nullptr, &type, base + 0x6AC, &size);
  RegQueryValueExA(key, "ITEMSTOREFINDWNDINFO.Y", nullptr, &type, base + 0x6B0, &size);
  RegQueryValueExA(key, "ITEMSTOREFINDWNDINFO.W", nullptr, &type, base + 0x6B4, &size);
  RegQueryValueExA(key, "ITEMSTOREFINDWNDINFO.H", nullptr, &type, base + 0x6B8, &size);
  RegQueryValueExA(key, "BOOKINGLISTWNDINFO.X", nullptr, &type, base + 0x6BC, &size);
  RegQueryValueExA(key, "BOOKINGLISTWNDINFO.Y", nullptr, &type, base + 0x6C0, &size);
  RegQueryValueExA(key, "BOOKINGLISTWNDINFO.W", nullptr, &type, base + 0x6C4, &size);
  RegQueryValueExA(key, "BOOKINGLISTWNDINFO.H", nullptr, &type, base + 0x6C8, &size);
  RegQueryValueExA(key, "BOOKINGMBWNDINFO.X", nullptr, &type, base + 0x6CC, &size);
  RegQueryValueExA(key, "BOOKINGMBWNDINFO.Y", nullptr, &type, base + 0x6D0, &size);
  RegQueryValueExA(key, "BOOKINGMBWNDINFO.W", nullptr, &type, base + 0x6D4, &size);
  RegQueryValueExA(key, "BOOKINGMBWNDINFO.H", nullptr, &type, base + 0x6D8, &size);
  RegQueryValueExA(key, "BOOKINGWNDINFO.X", nullptr, &type, base + 0x6DC, &size);
  RegQueryValueExA(key, "BOOKINGWNDINFO.Y", nullptr, &type, base + 0x6E0, &size);
  RegQueryValueExA(key, "BOOKINGWNDINFO.W", nullptr, &type, base + 0x6E4, &size);
  RegQueryValueExA(key, "BOOKINGWNDINFO.H", nullptr, &type, base + 0x6E8, &size);
  RegQueryValueExA(key, "BOOKINGMBCHECKWNDINFO.X", nullptr, &type, base + 0x6EC, &size);
  RegQueryValueExA(key, "BOOKINGMBCHECKWNDINFO.Y", nullptr, &type, base + 0x6F0, &size);
  RegQueryValueExA(key, "BOOKINGMBCHECKWNDINFO.W", nullptr, &type, base + 0x6F4, &size);
  RegQueryValueExA(key, "BOOKINGMBCHECKWNDINFO.H", nullptr, &type, base + 0x6F8, &size);
  RegQueryValueExA(key, "ITEMSHOPWNDINFO.X", nullptr, &type, base + 0x6FC, &size);
  RegQueryValueExA(key, "ITEMSHOPWNDINFO.Y", nullptr, &type, base + 0x700, &size);
  RegQueryValueExA(key, "ITEMSHOPWNDINFO.W", nullptr, &type, base + 0x704, &size);
  RegQueryValueExA(key, "ITEMSHOPWNDINFO.H", nullptr, &type, base + 0x708, &size);
  RegQueryValueExA(key, "ITEMSELLWNDINFO.X", nullptr, &type, base + 0x70C, &size);
  RegQueryValueExA(key, "ITEMSELLWNDINFO.Y", nullptr, &type, base + 0x710, &size);
  RegQueryValueExA(key, "ITEMSELLWNDINFO.W", nullptr, &type, base + 0x714, &size);
  RegQueryValueExA(key, "ITEMSELLWNDINFO.H", nullptr, &type, base + 0x718, &size);
  RegQueryValueExA(key, "ITEMPURCHASEWNDINFO.X", nullptr, &type, base + 0x71C, &size);
  RegQueryValueExA(key, "ITEMPURCHASEWNDINFO.Y", nullptr, &type, base + 0x720, &size);
  RegQueryValueExA(key, "ITEMPURCHASEWNDINFO.W", nullptr, &type, base + 0x724, &size);
  RegQueryValueExA(key, "ITEMPURCHASEWNDINFO.H", nullptr, &type, base + 0x728, &size);
  RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.X", nullptr, &type, base + 0x72C, &size);
  RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.Y", nullptr, &type, base + 0x730, &size);
  RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.W", nullptr, &type, base + 0x734, &size);
  RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.H", nullptr, &type, base + 0x738, &size);
  if (RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_073C), &size) != ERROR_SUCCESS) {
    this->unknown_073C = 0;
  }
  if (RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.RADIO", nullptr, &type, base + 0x740, &size) != ERROR_SUCCESS) {
    *reinterpret_cast<int*>(base + 0x740) = 1;
  }
  if (RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.SIZEMODE", nullptr, &type, base + 0x744, &size) != ERROR_SUCCESS) {
    *reinterpret_cast<int*>(base + 0x744) = 0;
  }
  RegQueryValueExA(key, "EXCHANGEACCEPTWNDINFO.X", nullptr, &type, base + 0x75C, &size);
  RegQueryValueExA(key, "EXCHANGEACCEPTWNDINFO.Y", nullptr, &type, base + 0x760, &size);
  RegQueryValueExA(key, "EXCHANGEACCEPTWNDINFO.W", nullptr, &type, base + 0x764, &size);
  RegQueryValueExA(key, "EXCHANGEACCEPTWNDINFO.H", nullptr, &type, base + 0x768, &size);
  RegQueryValueExA(key, "SKILLLISTWNDINFO.X", nullptr, &type, base + 0x76C, &size);
  RegQueryValueExA(key, "SKILLLISTWNDINFO.Y", nullptr, &type, base + 0x770, &size);
  RegQueryValueExA(key, "SKILLLISTWNDINFO.W", nullptr, &type, base + 0x774, &size);
  RegQueryValueExA(key, "SKILLLISTWNDINFO.H", nullptr, &type, base + 0x778, &size);
  if (RegQueryValueExA(key, "SKILLLISTWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_077C), &size) != ERROR_SUCCESS) {
    this->unknown_077C = 0;
  }
  RegQueryValueExA(key, "MERCHANTITEMWNDINFO.X", nullptr, &type, base + 0x780, &size);
  RegQueryValueExA(key, "MERCHANTITEMWNDINFO.Y", nullptr, &type, base + 0x784, &size);
  RegQueryValueExA(key, "MERCHANTITEMWNDINFO.W", nullptr, &type, base + 0x788, &size);
  RegQueryValueExA(key, "MERCHANTITEMWNDINFO.H", nullptr, &type, base + 0x78C, &size);
  if (RegQueryValueExA(key, "MERCHANTITEMWNDINFO.SHOW", nullptr, &type, base + 0x790, &size) != ERROR_SUCCESS) {
    *reinterpret_cast<int*>(base + 0x790) = 0;
  }
  RegQueryValueExA(key, "MERCHANTMIRRORITEMWNDINFO.X", nullptr, &type, base + 0x794, &size);
  RegQueryValueExA(key, "MERCHANTMIRRORITEMWNDINFO.Y", nullptr, &type, base + 0x798, &size);
  RegQueryValueExA(key, "MERCHANTMIRRORITEMWNDINFO.W", nullptr, &type, base + 0x79C, &size);
  RegQueryValueExA(key, "MERCHANTMIRRORITEMWNDINFO.H", nullptr, &type, base + 0x7A0, &size);
  RegQueryValueExA(key, "MERCHANTSHOPMAKEWNDINFO.X", nullptr, &type, base + 0x7A4, &size);
  RegQueryValueExA(key, "MERCHANTSHOPMAKEWNDINFO.Y", nullptr, &type, base + 0x7A8, &size);
  RegQueryValueExA(key, "MERCHANTSHOPMAKEWNDINFO.W", nullptr, &type, base + 0x7AC, &size);
  RegQueryValueExA(key, "MERCHANTSHOPMAKEWNDINFO.H", nullptr, &type, base + 0x7B0, &size);
  RegQueryValueExA(key, "MERCHANTITEMSHOPWNDINFO.X", nullptr, &type, base + 0x7B4, &size);
  RegQueryValueExA(key, "MERCHANTITEMSHOPWNDINFO.Y", nullptr, &type, base + 0x7B8, &size);
  RegQueryValueExA(key, "MERCHANTITEMSHOPWNDINFO.W", nullptr, &type, base + 0x7BC, &size);
  RegQueryValueExA(key, "MERCHANTITEMSHOPWNDINFO.H", nullptr, &type, base + 0x7C0, &size);
  RegQueryValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.X", nullptr, &type, base + 0x7C4, &size);
  RegQueryValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.Y", nullptr, &type, base + 0x7C8, &size);
  RegQueryValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.W", nullptr, &type, base + 0x7CC, &size);
  RegQueryValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.H", nullptr, &type, base + 0x7D0, &size);
  RegQueryValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.X", nullptr, &type, base + 0x7D4, &size);
  RegQueryValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.Y", nullptr, &type, base + 0x7D8, &size);
  RegQueryValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.W", nullptr, &type, base + 0x7DC, &size);
  RegQueryValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.H", nullptr, &type, base + 0x7E0, &size);
  RegQueryValueExA(key, "ITEMCOLLECTIONWNDINFO.X", nullptr, &type, base + 0x7E4, &size);
  RegQueryValueExA(key, "ITEMCOLLECTIONWNDINFO.Y", nullptr, &type, base + 0x7E8, &size);
  RegQueryValueExA(key, "ITEMCOLLECTIONWNDINFO.W", nullptr, &type, base + 0x7EC, &size);
  RegQueryValueExA(key, "ITEMCOLLECTIONWNDINFO.H", nullptr, &type, base + 0x7F0, &size);
  RegQueryValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.X", nullptr, &type, base + 0x7F4, &size);
  RegQueryValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.Y", nullptr, &type, base + 0x7F8, &size);
  RegQueryValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.W", nullptr, &type, base + 0x7FC, &size);
  RegQueryValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.H", nullptr, &type, base + 0x800, &size);
  RegQueryValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.X", nullptr, &type, base + 0x804, &size);
  RegQueryValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.Y", nullptr, &type, base + 0x808, &size);
  RegQueryValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.W", nullptr, &type, base + 0x80C, &size);
  RegQueryValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.H", nullptr, &type, base + 0x810, &size);
  RegQueryValueExA(key, "PARTYSETTINGWNDINFO.X", nullptr, &type, base + 0x814, &size);
  RegQueryValueExA(key, "PARTYSETTINGWNDINFO.Y", nullptr, &type, base + 0x818, &size);
  RegQueryValueExA(key, "PARTYSETTINGWNDINFO.W", nullptr, &type, base + 0x81C, &size);
  RegQueryValueExA(key, "PARTYSETTINGWNDINFO.H", nullptr, &type, base + 0x820, &size);
  RegQueryValueExA(key, "DETAILLEVELWNDINFO.X", nullptr, &type, base + 0x824, &size);
  RegQueryValueExA(key, "DETAILLEVELWNDINFO.Y", nullptr, &type, base + 0x828, &size);
  RegQueryValueExA(key, "DETAILLEVELWNDINFO.W", nullptr, &type, base + 0x82C, &size);
  RegQueryValueExA(key, "DETAILLEVELWNDINFO.H", nullptr, &type, base + 0x830, &size);
  RegQueryValueExA(key, "WHISPERWNDINFO.X", nullptr, &type, base + 0x848, &size);
  RegQueryValueExA(key, "WHISPERWNDINFO.Y", nullptr, &type, base + 0x84C, &size);
  RegQueryValueExA(key, "WHISPERWNDINFO.W", nullptr, &type, base + 0x850, &size);
  RegQueryValueExA(key, "WHISPERWNDINFO.H", nullptr, &type, base + 0x854, &size);
  RegQueryValueExA(key, "FRIENDOPTIONWNDINFO.X", nullptr, &type, base + 0x86C, &size);
  RegQueryValueExA(key, "FRIENDOPTIONWNDINFO.Y", nullptr, &type, base + 0x870, &size);
  RegQueryValueExA(key, "FRIENDOPTIONWNDINFO.W", nullptr, &type, base + 0x874, &size);
  RegQueryValueExA(key, "FRIENDOPTIONWNDINFO.H", nullptr, &type, base + 0x878, &size);
  RegQueryValueExA(key, "ITEMCOMPOSITIONWNDINFO.X", nullptr, &type, base + 0x87C, &size);
  RegQueryValueExA(key, "ITEMCOMPOSITIONWNDINFO.Y", nullptr, &type, base + 0x880, &size);
  RegQueryValueExA(key, "ITEMCOMPOSITIONWNDINFO.W", nullptr, &type, base + 0x884, &size);
  RegQueryValueExA(key, "ITEMCOMPOSITIONWNDINFO.H", nullptr, &type, base + 0x888, &size);
  RegQueryValueExA(key, "ITEMIDENTIFYWNDINFO.X", nullptr, &type, base + 0x88C, &size);
  RegQueryValueExA(key, "ITEMIDENTIFYWNDINFO.Y", nullptr, &type, base + 0x890, &size);
  RegQueryValueExA(key, "ITEMIDENTIFYWNDINFO.W", nullptr, &type, base + 0x894, &size);
  RegQueryValueExA(key, "ITEMIDENTIFYWNDINFO.H", nullptr, &type, base + 0x898, &size);
  RegQueryValueExA(key, "SUBCHATWNDST.X", nullptr, &type, base + 0x914, &size);
  RegQueryValueExA(key, "SUBCHATWNDST.Y", nullptr, &type, base + 0x918, &size);
  RegQueryValueExA(key, "SUBCHATWNDST.W", nullptr, &type, base + 0x91C, &size);
  RegQueryValueExA(key, "SUBCHATWNDST.H", nullptr, &type, base + 0x920, &size);
  RegQueryValueExA(key, "SUBCHATWNDBT.X", nullptr, &type, base + 0x924, &size);
  RegQueryValueExA(key, "SUBCHATWNDBT.Y", nullptr, &type, base + 0x928, &size);
  RegQueryValueExA(key, "SUBCHATWNDBT.W", nullptr, &type, base + 0x92C, &size);
  RegQueryValueExA(key, "SUBCHATWNDBT.H", nullptr, &type, base + 0x930, &size);
  RegQueryValueExA(key, "CHATWNDSTICKON", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_03F4), &size);
  RegQueryValueExA(key, "ONSTSUBCHAT", nullptr, &type, base + 0x3F5, &size);
  RegQueryValueExA(key, "ONBTSUBCHAT", nullptr, &type, reinterpret_cast<BYTE*>(&this->unknown_03F6), &size);
  RegQueryValueExA(key, "MEMORIALDUNWND.X", nullptr, &type, reinterpret_cast<BYTE*>(&g_memorialDunWndX), &size);
  RegQueryValueExA(key, "MEMORIALDUNWND.Y", nullptr, &type, reinterpret_cast<BYTE*>(&g_memorialDunWndY), &size);
  RegQueryValueExA(key, "QUICKSLOTWNDINFO.X", nullptr, &type, base + 0x934, &size);
  RegQueryValueExA(key, "QUICKSLOTWNDINFO.Y", nullptr, &type, base + 0x938, &size);
  RegQueryValueExA(key, "QUICKSLOTWNDINFO.W", nullptr, &type, base + 0x93C, &size);
  RegQueryValueExA(key, "QUICKSLOTWNDINFO.H", nullptr, &type, base + 0x940, &size);
  RegQueryValueExA(key, "BATTLEFIELDWNDINFO.X", nullptr, &type, base + 0x944, &size);
  RegQueryValueExA(key, "BATTLEFIELDWNDINFO.Y", nullptr, &type, base + 0x948, &size);
  RegQueryValueExA(key, "BATTLEFIELDWNDINFO.W", nullptr, &type, base + 0x94C, &size);
  RegQueryValueExA(key, "BATTLEFIELDWNDINFO.H", nullptr, &type, base + 0x950, &size);
  RegQueryValueExA(key, "GUILDHELPERWNDINFO.X", nullptr, &type, base + 0x9D4, &size);
  RegQueryValueExA(key, "GUILDHELPERWNDINFO.Y", nullptr, &type, base + 0x9D8, &size);
  RegQueryValueExA(key, "GUILDHELPERWNDINFO.W", nullptr, &type, base + 0x9DC, &size);
  RegQueryValueExA(key, "GUILDHELPERWNDINFO.H", nullptr, &type, base + 0x9E0, &size);
  RegQueryValueExA(key, "BUYINGSTOREMAKEWNDINFO.X", nullptr, &type, base + 0x954, &size);
  RegQueryValueExA(key, "BUYINGSTOREMAKEWNDINFO.Y", nullptr, &type, base + 0x958, &size);
  RegQueryValueExA(key, "BUYINGSTOREMAKEWNDINFO.W", nullptr, &type, base + 0x95C, &size);
  RegQueryValueExA(key, "BUYINGSTOREMAKEWNDINFO.H", nullptr, &type, base + 0x960, &size);
  RegQueryValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.X", nullptr, &type, base + 0x964, &size);
  RegQueryValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.Y", nullptr, &type, base + 0x968, &size);
  RegQueryValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.W", nullptr, &type, base + 0x96C, &size);
  RegQueryValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.H", nullptr, &type, base + 0x970, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.X", nullptr, &type, base + 0x974, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.Y", nullptr, &type, base + 0x978, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.W", nullptr, &type, base + 0x97C, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.H", nullptr, &type, base + 0x980, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.X", nullptr, &type, base + 0x984, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.Y", nullptr, &type, base + 0x988, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.W", nullptr, &type, base + 0x98C, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.H", nullptr, &type, base + 0x990, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.X", nullptr, &type, base + 0x994, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.Y", nullptr, &type, base + 0x998, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.W", nullptr, &type, base + 0x99C, &size);
  RegQueryValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.H", nullptr, &type, base + 0x9A0, &size);
  RegQueryValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.X", nullptr, &type, base + 0x9A4, &size);
  RegQueryValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.Y", nullptr, &type, base + 0x9A8, &size);
  RegQueryValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.W", nullptr, &type, base + 0x9AC, &size);
  RegQueryValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.H", nullptr, &type, base + 0x9B0, &size);
  RegQueryValueExA(key, "REPLAYRECCONTROLWNDINFO.X", nullptr, &type, base + 0x4F154, &size);
  RegQueryValueExA(key, "REPLAYRECCONTROLWNDINFO.Y", nullptr, &type, base + 0x4F158, &size);
  RegQueryValueExA(key, "REPLAYRECCONTROLWNDINFO.W", nullptr, &type, base + 0x4F15C, &size);
  RegQueryValueExA(key, "REPLAYRECCONTROLWNDINFO.H", nullptr, &type, base + 0x4F160, &size);
  RegQueryValueExA(key, "NAVIGATIONWNDINFO.X", nullptr, &type, base + 0x4F178, &size);
  RegQueryValueExA(key, "NAVIGATIONWNDINFO.Y", nullptr, &type, base + 0x4F17C, &size);
  RegQueryValueExA(key, "NAVIGATIONWNDINFO.W", nullptr, &type, base + 0x4F180, &size);
  RegQueryValueExA(key, "NAVIGATIONWNDINFO.H", nullptr, &type, base + 0x4F184, &size);
  RegQueryValueExA(key, "NAVIGATIONWNDINFO.ISSCREEN", nullptr, &type, base + 0x4F188, &size);
  reinterpret_cast<MiniMapWndInfo*>(base + 0x4F1A8)->Reset();
  RegQueryValueExA(key, "MINIMAPWNDINFO.X", nullptr, &type, base + 0x4F1B0, &size);
  RegQueryValueExA(key, "MINIMAPWNDINFO.Y", nullptr, &type, base + 0x4F1B4, &size);
  RegQueryValueExA(key, "MINIMAPWNDINFO.SHOWQUEST", nullptr, &type, base + 0x4F1C0, &size);
  RegQueryValueExA(key, "MINIMAPWNDINFO.WHOWNPC", nullptr, &type, base + 0x4F1C4, &size);
  RegQueryValueExA(key, "MINIMAPWNDINFO.WHOWMEMBER", nullptr, &type, base + 0x4F1C8, &size);
  RegQueryValueExA(key, "ACHTRACINGWNDINFO.X", nullptr, &type, base + 0x4F1D8, &size);
  RegQueryValueExA(key, "ACHTRACINGWNDINFO.Y", nullptr, &type, base + 0x4F1DC, &size);
  RegQueryValueExA(key, "PREVIEWEQUIPWNDINFO.X", nullptr, &type, base + 0x4F220, &size);
  RegQueryValueExA(key, "PREVIEWEQUIPWNDINFO.Y", nullptr, &type, base + 0x4F224, &size);
  RegQueryValueExA(key, "SWAPEQUIPMENTWNDINFO.X", nullptr, &type, base + 0x4F228, &size);
  RegQueryValueExA(key, "SWAPEQUIPMENTWNDINFO.Y", nullptr, &type, base + 0x4F22C, &size);
  RegQueryValueExA(key, "PETINFOWNDINFO.X", nullptr, &type, base + 0x8BC, &size);
  RegQueryValueExA(key, "PETINFOWNDINFO.Y", nullptr, &type, base + 0x8C0, &size);
  RegQueryValueExA(key, "PETINFOWNDINFO.W", nullptr, &type, base + 0x8C4, &size);
  RegQueryValueExA(key, "PETINFOWNDINFO.H", nullptr, &type, base + 0x8C8, &size);
  RegQueryValueExA(key, "HOMUNINFOWNDINFO.X", nullptr, &type, base + 0x8CC, &size);
  RegQueryValueExA(key, "HOMUNINFOWNDINFO.Y", nullptr, &type, base + 0x8D0, &size);
  RegQueryValueExA(key, "HOMUNINFOWNDINFO.W", nullptr, &type, base + 0x8D4, &size);
  RegQueryValueExA(key, "HOMUNINFOWNDINFO.H", nullptr, &type, base + 0x8D8, &size);
  RegQueryValueExA(key, "MERINFOWNDINFO.X", nullptr, &type, base + 0x8F0, &size);
  RegQueryValueExA(key, "MERINFOWNDINFO.Y", nullptr, &type, base + 0x8F4, &size);
  RegQueryValueExA(key, "MERINFOWNDINFO.W", nullptr, &type, base + 0x8F8, &size);
  RegQueryValueExA(key, "MERINFOWNDINFO.H", nullptr, &type, base + 0x8FC, &size);
  RegQueryValueExA(key, "ADVEN_REGISTERWNDINFO.X", nullptr, &type, base + 0xA00, &size);
  RegQueryValueExA(key, "ADVEN_REGISTERWNDINFO.Y", nullptr, &type, base + 0xA04, &size);
  RegQueryValueExA(key, "ADVEN_REGISTERWNDINFO.W", nullptr, &type, base + 0xA08, &size);
  RegQueryValueExA(key, "ADVEN_REGISTERWNDINFO.H", nullptr, &type, base + 0xA0C, &size);
  RegQueryValueExA(key, "ADVEN_JOINWNDINFO.X", nullptr, &type, base + 0xA10, &size);
  RegQueryValueExA(key, "ADVEN_JOINWNDINFO.Y", nullptr, &type, base + 0xA14, &size);
  RegQueryValueExA(key, "ADVEN_JOINWNDINFO.W", nullptr, &type, base + 0xA18, &size);
  RegQueryValueExA(key, "ADVEN_JOINWNDINFO.H", nullptr, &type, base + 0xA1C, &size);
  RegQueryValueExA(key, "ADVEN_PARTYBOARDWNDINFO.X", nullptr, &type, base + 0xA20, &size);
  RegQueryValueExA(key, "ADVEN_PARTYBOARDWNDINFO.Y", nullptr, &type, base + 0xA24, &size);
  RegQueryValueExA(key, "ADVEN_PARTYBOARDWNDINFO.W", nullptr, &type, base + 0xA28, &size);
  RegQueryValueExA(key, "ADVEN_PARTYBOARDWNDINFO.H", nullptr, &type, base + 0xA2C, &size);
  RegCloseKey(key);
}

// OFFSET: 0x00AAADF0
// FUNCTION: fn_17272
int UIWindowMgr::ScaleToViewportWidth(int value) {
  return g_renderer->viewport.width * value / 640;
}

// OFFSET: 0x00AAAE20
// FUNCTION: fn_17273
int UIWindowMgr::CenterOnViewportX(int x) {
  return (g_renderer->viewport.width - 640) / 2 + x;
}

// OFFSET: 0x00AAAE40
// FUNCTION: fn_17274
int UIWindowMgr::CenterOnViewportY(int y) {
  return (g_renderer->viewport.height - 480) / 2 + y;
}

// OFFSET: 0x00AAAE60
// FUNCTION: fn_17275
void UIWindowMgr::ConfirmActiveDialog() {
  UIWindow* window = this->unknown_01F8;
  if (window || (window = this->unknown_01F0) != nullptr || (window = this->unknown_01F4) != nullptr || (window = this->unknown_0214) != nullptr ||
      (window = this->unknown_021C) != nullptr || (window = this->unknown_020C) != nullptr || (window = this->unknown_0374) != nullptr ||
      (window = this->hoverWindow) != nullptr) {
    window->SendMsg(nullptr, 18, 0, 0, 0, 0);
    return;
  }

  for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
    if ((*entry)->Slot02()) {
      (*entry)->SendMsg(nullptr, 18, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00AAAD90
// FUNCTION: fn_17271
void __stdcall Nullsub371() {
}

// OFFSET: 0x00AAB260
// FUNCTION: fn_17281
void UIWindowMgr::SaveVisibleWindowIds() {
  if (this->rendered.size() < 3) {
    return;
  }

  this->savedWindowIds.clear();
  for (UIWindow* window : this->rendered) {
    int id = window->geometry.id;
    if (id == 8) {
      if (this->unknown_054C) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 10) {
      if (this->unknown_05A4) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 11) {
      if (this->unknown_0588) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 13) {
      if (this->unknown_05BC) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 37) {
      if (this->unknown_077C) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 321) {
      if (this->unknown_0568) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 205) {
      if (this->unknown_0568) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 134) {
      if (this->unknown_0568) {
        this->savedWindowIds.push_back(id);
      }
    } else if ((id == 69 || id == 34) && this->unknown_073C) {
      this->savedWindowIds.push_back(id);
    } else if (id == 271) {
      if (g_session.HasUnknown6178Records() == true) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 286) {
      if (this->unknown_4F210 == true) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 10014) {
      if (g_session.unknown_5470 && g_session.unknown_5474 && g_effectDetailLevel == 2 && g_textureDetailLevel == 2) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 301) {
      for (auto entry = this->savedWindowIds.begin(); entry != this->savedWindowIds.end(); ++entry) {
        if (*entry == 301) {
          this->savedWindowIds.erase(entry);
          break;
        }
      }
      this->savedWindowIds.push_back(id);
    }
  }
}

// OFFSET: 0x00AAB8A0
// FUNCTION: fn_17282
void UIWindowMgr::UpdateNavigationList() {
  if (this->window_4F174 && !g_navigationMinimized && static_cast<UINavigationV4Wnd*>(this->window_4F174)->IsListShown()) {
    this->window_4F174->SendMsg(nullptr, 6, 202, 0, 0, 0);
  }
}

// OFFSET: 0x00AAB920
// FUNCTION: fn_17283
void UIWindowMgr::SaveShortcutItems() {
  char path[512];
  std::strcpy(path, g_subKey);
  std::strcat(path, g_shortcutItemKeySuffix);
  std::strcat(path, "\\");
  if (std::strstr(g_session.m_serverName.c_str(), "사크라이") != nullptr) {
    std::strcat(path, "사크라이");
  } else {
    std::strcat(path, g_session.m_serverName.c_str());
  }

  HKEY key;
  DWORD disposition = 0;
  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, path, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) != ERROR_SUCCESS) {
    return;
  }

  for (int index = 0; index < 36; ++index) {
    char name[36];
    char description[33] = {};
    std::strcpy(description, g_session.GetHotkeyDescription(index));
    std::sprintf(name, "SITEM%d%d%d", index, g_serviceType, g_session.unknown_0F50);
    RegSetValueExA(key, name, 0, REG_SZ, reinterpret_cast<const BYTE*>(description), 32);

    DWORD level = g_session.GetUnknown0420(index);
    std::sprintf(name, "S_SKILL_USE_LEVEL%d%d%d", index, g_serviceType, g_session.unknown_0F50);
    RegSetValueExA(key, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&level), 4);
  }

  RegCloseKey(key);
}

// OFFSET: 0x00AABB40
// FUNCTION: fn_17284
void UIWindowMgr::SaveUIRectInfo() {
  const BYTE* const base = reinterpret_cast<const BYTE*>(this);
  char path[512];
  std::strcpy(path, g_subKey);
  std::strcat(path, g_uiRectInfoKeySuffix);

  HKEY key;
  DWORD disposition = 0;
  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, path, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
    char name[256];
    RegSetValueExA(key, "m_minimapZoomWnd", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0050), 4);
    RegSetValueExA(key, "M_ISDRAGALL", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0064), 4);
    RegSetValueExA(key, "M_ISDRAWCOMPASS", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0060), 4);
    RegSetValueExA(key, "M_MINIMAPARGB", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_005C), 4);
    RegSetValueExA(key, "M_CHATWNDSTATUS", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_004C), 4);
    RegSetValueExA(key, "M_CHATWNDHEIGHT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0038), 4);
    RegSetValueExA(key, "M_CHATWNDX", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0030), 4);
    RegSetValueExA(key, "M_CHATWNDY", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0034), 4);
    RegSetValueExA(key, "M_CHATWNDSHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_003C), 4);
    RegSetValueExA(key, "SAYDIALOGWNDINFO.X", 0, REG_DWORD, base + 0x5E4, 4);
    RegSetValueExA(key, "SAYDIALOGWNDINFO.Y", 0, REG_DWORD, base + 0x5E8, 4);
    RegSetValueExA(key, "SAYDIALOGWNDINFO.W", 0, REG_DWORD, base + 0x5EC, 4);
    RegSetValueExA(key, "SAYDIALOGWNDINFO.H", 0, REG_DWORD, base + 0x5F0, 4);
    RegSetValueExA(key, "CHOOSEWNDINFO.X", 0, REG_DWORD, base + 0x5F4, 4);
    RegSetValueExA(key, "CHOOSEWNDINFO.Y", 0, REG_DWORD, base + 0x5F8, 4);
    RegSetValueExA(key, "CHOOSEWNDINFO.W", 0, REG_DWORD, base + 0x5FC, 4);
    RegSetValueExA(key, "CHOOSEWNDINFO.H", 0, REG_DWORD, base + 0x600, 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.X", 0, REG_DWORD, base + 0x514, 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.Y", 0, REG_DWORD, base + 0x518, 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.W", 0, REG_DWORD, base + 0x51C, 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.H", 0, REG_DWORD, base + 0x520, 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.ISSHOW", 0, REG_DWORD, base + 0x524, 4);
    RegSetValueExA(key, "ITEMWNDINFO.X", 0, REG_DWORD, base + 0x528, 4);
    RegSetValueExA(key, "ITEMWNDINFO.Y", 0, REG_DWORD, base + 0x52C, 4);
    RegSetValueExA(key, "ITEMWNDINFO.W", 0, REG_DWORD, base + 0x530, 4);
    RegSetValueExA(key, "ITEMWNDINFO.H", 0, REG_DWORD, base + 0x534, 4);
    RegSetValueExA(key, "ITEMWNDINFO.ORGHEIGHT", 0, REG_DWORD, base + 0x538, 4);
    RegSetValueExA(key, "ITEMWNDINFO.CURTAB", 0, REG_DWORD, base + 0x53C, 4);
    RegSetValueExA(key, "ITEMWNDINFO.CURRADIO1", 0, REG_DWORD, base + 0x540, 4);
    RegSetValueExA(key, "ITEMWNDINFO.CURRADIO2", 0, REG_DWORD, base + 0x544, 4);
    RegSetValueExA(key, "ITEMWNDINFO.CURRADIO3", 0, REG_DWORD, base + 0x548, 4);
    RegSetValueExA(key, "ITEMWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_054C), 4);
    RegSetValueExA(key, "QUESTWNDINFO.X", 0, REG_DWORD, base + 0x550, 4);
    RegSetValueExA(key, "QUESTWNDINFO.Y", 0, REG_DWORD, base + 0x554, 4);
    RegSetValueExA(key, "QUESTWNDINFO.W", 0, REG_DWORD, base + 0x558, 4);
    RegSetValueExA(key, "QUESTWNDINFO.H", 0, REG_DWORD, base + 0x55C, 4);
    RegSetValueExA(key, "QUESTWNDINFO.ORGHEIGHT", 0, REG_DWORD, base + 0x560, 4);
    RegSetValueExA(key, "QUESTWNDINFO.CURTAB", 0, REG_DWORD, base + 0x564, 4);
    RegSetValueExA(key, "QUESTWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0568), 4);
    RegSetValueExA(key, "QUESTDISPWNDINFO.QUESTDISPLAY", 0, REG_DWORD, base + 0x570, 4);
    RegSetValueExA(key, "STATUSWNDINFO.X", 0, REG_DWORD, base + 0x574, 4);
    RegSetValueExA(key, "STATUSWNDINFO.Y", 0, REG_DWORD, base + 0x578, 4);
    RegSetValueExA(key, "STATUSWNDINFO.W", 0, REG_DWORD, base + 0x57C, 4);
    RegSetValueExA(key, "STATUSWNDINFO.H", 0, REG_DWORD, base + 0x580, 4);
    RegSetValueExA(key, "STATUSWNDINFO.ORGHEIGHT", 0, REG_DWORD, base + 0x584, 4);
    RegSetValueExA(key, "STATUSWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0588), 4);
    RegSetValueExA(key, "EQUIPWNDINFO.X", 0, REG_DWORD, base + 0x590, 4);
    RegSetValueExA(key, "EQUIPWNDINFO.Y", 0, REG_DWORD, base + 0x594, 4);
    RegSetValueExA(key, "EQUIPWNDINFO.W", 0, REG_DWORD, base + 0x598, 4);
    RegSetValueExA(key, "EQUIPWNDINFO.H", 0, REG_DWORD, base + 0x59C, 4);
    RegSetValueExA(key, "EQUIPWNDINFO.ORGHEIGHT", 0, REG_DWORD, base + 0x5A0, 4);
    RegSetValueExA(key, "EQUIPWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_05A4), 4);
    RegSetValueExA(key, "OPTIONWNDINFO.X", 0, REG_DWORD, base + 0x5A8, 4);
    RegSetValueExA(key, "OPTIONWNDINFO.Y", 0, REG_DWORD, base + 0x5AC, 4);
    RegSetValueExA(key, "OPTIONWNDINFO.W", 0, REG_DWORD, base + 0x5B0, 4);
    RegSetValueExA(key, "OPTIONWNDINFO.H", 0, REG_DWORD, base + 0x5B4, 4);
    RegSetValueExA(key, "OPTIONWNDINFO.ORGHEIGHT", 0, REG_DWORD, base + 0x5B8, 4);
    RegSetValueExA(key, "OPTIONWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_05BC), 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.X", 0, REG_DWORD, base + 0x5C0, 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.Y", 0, REG_DWORD, base + 0x5C4, 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.W", 0, REG_DWORD, base + 0x5C8, 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.H", 0, REG_DWORD, base + 0x5CC, 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_05D0), 4);
    RegSetValueExA(key, "CHATROOMWNDINFO.X", 0, REG_DWORD, base + 0x604, 4);
    RegSetValueExA(key, "CHATROOMWNDINFO.Y", 0, REG_DWORD, base + 0x608, 4);
    RegSetValueExA(key, "CHATROOMWNDINFO.W", 0, REG_DWORD, base + 0x60C, 4);
    RegSetValueExA(key, "CHATROOMWNDINFO.H", 0, REG_DWORD, base + 0x610, 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.X", 0, REG_DWORD, base + 0x614, 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.Y", 0, REG_DWORD, base + 0x618, 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.W", 0, REG_DWORD, base + 0x61C, 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.H", 0, REG_DWORD, base + 0x620, 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.subWnd", 0, REG_DWORD, base + 0x624, 4);
    std::memset(name, 0, sizeof(name) - 1);
    RegSetValueExA(key, "ITEMSTOREWNDNUM", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0058), 4);
    for (int index = 0; index < 8; ++index) {
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.X", index);
      RegSetValueExA(key, name, 0, REG_DWORD, base + 0x62C + index * 0x10, 4);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.Y", index);
      RegSetValueExA(key, name, 0, REG_DWORD, base + 0x630 + index * 0x10, 4);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.W", index);
      RegSetValueExA(key, name, 0, REG_DWORD, base + 0x634 + index * 0x10, 4);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.H", index);
      RegSetValueExA(key, name, 0, REG_DWORD, base + 0x638 + index * 0x10, 4);
    }
    RegSetValueExA(key, "ITEMSTOREWNDINFO.findWnd", 0, REG_DWORD, base + 0x628, 4);
    RegSetValueExA(key, "ITEMSTOREFINDWNDINFO.X", 0, REG_DWORD, base + 0x6AC, 4);
    RegSetValueExA(key, "ITEMSTOREFINDWNDINFO.Y", 0, REG_DWORD, base + 0x6B0, 4);
    RegSetValueExA(key, "ITEMSTOREFINDWNDINFO.W", 0, REG_DWORD, base + 0x6B4, 4);
    RegSetValueExA(key, "ITEMSTOREFINDWNDINFO.H", 0, REG_DWORD, base + 0x6B8, 4);
    RegSetValueExA(key, "BOOKINGLISTWNDINFO.X", 0, REG_DWORD, base + 0x6BC, 4);
    RegSetValueExA(key, "BOOKINGLISTWNDINFO.Y", 0, REG_DWORD, base + 0x6C0, 4);
    RegSetValueExA(key, "BOOKINGLISTWNDINFO.W", 0, REG_DWORD, base + 0x6C4, 4);
    RegSetValueExA(key, "BOOKINGLISTWNDINFO.H", 0, REG_DWORD, base + 0x6C8, 4);
    RegSetValueExA(key, "BOOKINGMBWNDINFO.X", 0, REG_DWORD, base + 0x6CC, 4);
    RegSetValueExA(key, "BOOKINGMBWNDINFO.Y", 0, REG_DWORD, base + 0x6D0, 4);
    RegSetValueExA(key, "BOOKINGMBWNDINFO.W", 0, REG_DWORD, base + 0x6D4, 4);
    RegSetValueExA(key, "BOOKINGMBWNDINFO.H", 0, REG_DWORD, base + 0x6D8, 4);
    RegSetValueExA(key, "BOOKINGWNDINFO.X", 0, REG_DWORD, base + 0x6DC, 4);
    RegSetValueExA(key, "BOOKINGWNDINFO.Y", 0, REG_DWORD, base + 0x6E0, 4);
    RegSetValueExA(key, "BOOKINGWNDINFO.W", 0, REG_DWORD, base + 0x6E4, 4);
    RegSetValueExA(key, "BOOKINGWNDINFO.H", 0, REG_DWORD, base + 0x6E8, 4);
    RegSetValueExA(key, "BOOKINGMBCHECKWNDINFO.X", 0, REG_DWORD, base + 0x6EC, 4);
    RegSetValueExA(key, "BOOKINGMBCHECKWNDINFO.Y", 0, REG_DWORD, base + 0x6F0, 4);
    RegSetValueExA(key, "BOOKINGMBCHECKWNDINFO.W", 0, REG_DWORD, base + 0x6F4, 4);
    RegSetValueExA(key, "BOOKINGMBCHECKWNDINFO.H", 0, REG_DWORD, base + 0x6F8, 4);
    RegSetValueExA(key, "ITEMSHOPWNDINFO.X", 0, REG_DWORD, base + 0x6FC, 4);
    RegSetValueExA(key, "ITEMSHOPWNDINFO.Y", 0, REG_DWORD, base + 0x700, 4);
    RegSetValueExA(key, "ITEMSHOPWNDINFO.W", 0, REG_DWORD, base + 0x704, 4);
    RegSetValueExA(key, "ITEMSHOPWNDINFO.H", 0, REG_DWORD, base + 0x708, 4);
    RegSetValueExA(key, "ITEMSELLWNDINFO.X", 0, REG_DWORD, base + 0x70C, 4);
    RegSetValueExA(key, "ITEMSELLWNDINFO.Y", 0, REG_DWORD, base + 0x710, 4);
    RegSetValueExA(key, "ITEMSELLWNDINFO.W", 0, REG_DWORD, base + 0x714, 4);
    RegSetValueExA(key, "ITEMSELLWNDINFO.H", 0, REG_DWORD, base + 0x718, 4);
    RegSetValueExA(key, "ITEMPURCHASEWNDINFO.X", 0, REG_DWORD, base + 0x71C, 4);
    RegSetValueExA(key, "ITEMPURCHASEWNDINFO.Y", 0, REG_DWORD, base + 0x720, 4);
    RegSetValueExA(key, "ITEMPURCHASEWNDINFO.W", 0, REG_DWORD, base + 0x724, 4);
    RegSetValueExA(key, "ITEMPURCHASEWNDINFO.H", 0, REG_DWORD, base + 0x728, 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.X", 0, REG_DWORD, base + 0x72C, 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.Y", 0, REG_DWORD, base + 0x730, 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.W", 0, REG_DWORD, base + 0x734, 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.H", 0, REG_DWORD, base + 0x738, 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_073C), 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.RADIO", 0, REG_DWORD, base + 0x740, 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.SIZEMODE", 0, REG_DWORD, base + 0x744, 4);
    RegSetValueExA(key, "EXCHANGEACCEPTWNDINFO.X", 0, REG_DWORD, base + 0x75C, 4);
    RegSetValueExA(key, "EXCHANGEACCEPTWNDINFO.Y", 0, REG_DWORD, base + 0x760, 4);
    RegSetValueExA(key, "EXCHANGEACCEPTWNDINFO.W", 0, REG_DWORD, base + 0x764, 4);
    RegSetValueExA(key, "EXCHANGEACCEPTWNDINFO.H", 0, REG_DWORD, base + 0x768, 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.X", 0, REG_DWORD, base + 0x76C, 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.Y", 0, REG_DWORD, base + 0x770, 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.W", 0, REG_DWORD, base + 0x774, 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.H", 0, REG_DWORD, base + 0x778, 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_077C), 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.X", 0, REG_DWORD, base + 0x780, 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.Y", 0, REG_DWORD, base + 0x784, 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.W", 0, REG_DWORD, base + 0x788, 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.H", 0, REG_DWORD, base + 0x78C, 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.SHOW", 0, REG_DWORD, base + 0x790, 4);
    RegSetValueExA(key, "MERCHANTMIRRORITEMWNDINFO.X", 0, REG_DWORD, base + 0x794, 4);
    RegSetValueExA(key, "MERCHANTMIRRORITEMWNDINFO.Y", 0, REG_DWORD, base + 0x798, 4);
    RegSetValueExA(key, "MERCHANTMIRRORITEMWNDINFO.W", 0, REG_DWORD, base + 0x79C, 4);
    RegSetValueExA(key, "MERCHANTMIRRORITEMWNDINFO.H", 0, REG_DWORD, base + 0x7A0, 4);
    RegSetValueExA(key, "MERCHANTSHOPMAKEWNDINFO.X", 0, REG_DWORD, base + 0x7A4, 4);
    RegSetValueExA(key, "MERCHANTSHOPMAKEWNDINFO.Y", 0, REG_DWORD, base + 0x7A8, 4);
    RegSetValueExA(key, "MERCHANTSHOPMAKEWNDINFO.W", 0, REG_DWORD, base + 0x7AC, 4);
    RegSetValueExA(key, "MERCHANTSHOPMAKEWNDINFO.H", 0, REG_DWORD, base + 0x7B0, 4);
    RegSetValueExA(key, "MERCHANTITEMSHOPWNDINFO.X", 0, REG_DWORD, base + 0x7B4, 4);
    RegSetValueExA(key, "MERCHANTITEMSHOPWNDINFO.Y", 0, REG_DWORD, base + 0x7B8, 4);
    RegSetValueExA(key, "MERCHANTITEMSHOPWNDINFO.W", 0, REG_DWORD, base + 0x7BC, 4);
    RegSetValueExA(key, "MERCHANTITEMSHOPWNDINFO.H", 0, REG_DWORD, base + 0x7C0, 4);
    RegSetValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.X", 0, REG_DWORD, base + 0x7C4, 4);
    RegSetValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.Y", 0, REG_DWORD, base + 0x7C8, 4);
    RegSetValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.W", 0, REG_DWORD, base + 0x7CC, 4);
    RegSetValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.H", 0, REG_DWORD, base + 0x7D0, 4);
    RegSetValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.X", 0, REG_DWORD, base + 0x7D4, 4);
    RegSetValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.Y", 0, REG_DWORD, base + 0x7D8, 4);
    RegSetValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.W", 0, REG_DWORD, base + 0x7DC, 4);
    RegSetValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.H", 0, REG_DWORD, base + 0x7E0, 4);
    RegSetValueExA(key, "ITEMCOLLECTIONWNDINFO.X", 0, REG_DWORD, base + 0x7E4, 4);
    RegSetValueExA(key, "ITEMCOLLECTIONWNDINFO.Y", 0, REG_DWORD, base + 0x7E8, 4);
    RegSetValueExA(key, "ITEMCOLLECTIONWNDINFO.W", 0, REG_DWORD, base + 0x7EC, 4);
    RegSetValueExA(key, "ITEMCOLLECTIONWNDINFO.H", 0, REG_DWORD, base + 0x7F0, 4);
    RegSetValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.X", 0, REG_DWORD, base + 0x7F4, 4);
    RegSetValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.Y", 0, REG_DWORD, base + 0x7F8, 4);
    RegSetValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.W", 0, REG_DWORD, base + 0x7FC, 4);
    RegSetValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.H", 0, REG_DWORD, base + 0x800, 4);
    RegSetValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.X", 0, REG_DWORD, base + 0x804, 4);
    RegSetValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.Y", 0, REG_DWORD, base + 0x808, 4);
    RegSetValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.W", 0, REG_DWORD, base + 0x80C, 4);
    RegSetValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.H", 0, REG_DWORD, base + 0x810, 4);
    RegSetValueExA(key, "PARTYSETTINGWNDINFO.X", 0, REG_DWORD, base + 0x814, 4);
    RegSetValueExA(key, "PARTYSETTINGWNDINFO.Y", 0, REG_DWORD, base + 0x818, 4);
    RegSetValueExA(key, "PARTYSETTINGWNDINFO.W", 0, REG_DWORD, base + 0x81C, 4);
    RegSetValueExA(key, "PARTYSETTINGWNDINFO.H", 0, REG_DWORD, base + 0x820, 4);
    RegSetValueExA(key, "DETAILLEVELWNDINFO.X", 0, REG_DWORD, base + 0x824, 4);
    RegSetValueExA(key, "DETAILLEVELWNDINFO.Y", 0, REG_DWORD, base + 0x828, 4);
    RegSetValueExA(key, "DETAILLEVELWNDINFO.W", 0, REG_DWORD, base + 0x82C, 4);
    RegSetValueExA(key, "DETAILLEVELWNDINFO.H", 0, REG_DWORD, base + 0x830, 4);
    RegSetValueExA(key, "WHISPERWNDINFO.X", 0, REG_DWORD, base + 0x848, 4);
    RegSetValueExA(key, "WHISPERWNDINFO.Y", 0, REG_DWORD, base + 0x84C, 4);
    RegSetValueExA(key, "WHISPERWNDINFO.W", 0, REG_DWORD, base + 0x850, 4);
    RegSetValueExA(key, "WHISPERWNDINFO.H", 0, REG_DWORD, base + 0x854, 4);
    RegSetValueExA(key, "FRIENDOPTIONWNDINFO.X", 0, REG_DWORD, base + 0x86C, 4);
    RegSetValueExA(key, "FRIENDOPTIONWNDINFO.Y", 0, REG_DWORD, base + 0x870, 4);
    RegSetValueExA(key, "FRIENDOPTIONWNDINFO.W", 0, REG_DWORD, base + 0x874, 4);
    RegSetValueExA(key, "FRIENDOPTIONWNDINFO.H", 0, REG_DWORD, base + 0x878, 4);
    RegSetValueExA(key, "ITEMCOMPOSITIONWNDINFO.X", 0, REG_DWORD, base + 0x87C, 4);
    RegSetValueExA(key, "ITEMCOMPOSITIONWNDINFO.Y", 0, REG_DWORD, base + 0x880, 4);
    RegSetValueExA(key, "ITEMCOMPOSITIONWNDINFO.W", 0, REG_DWORD, base + 0x884, 4);
    RegSetValueExA(key, "ITEMCOMPOSITIONWNDINFO.H", 0, REG_DWORD, base + 0x888, 4);
    RegSetValueExA(key, "ITEMIDENTIFYWNDINFO.X", 0, REG_DWORD, base + 0x88C, 4);
    RegSetValueExA(key, "ITEMIDENTIFYWNDINFO.Y", 0, REG_DWORD, base + 0x890, 4);
    RegSetValueExA(key, "ITEMIDENTIFYWNDINFO.W", 0, REG_DWORD, base + 0x894, 4);
    RegSetValueExA(key, "ITEMIDENTIFYWNDINFO.H", 0, REG_DWORD, base + 0x898, 4);
    RegSetValueExA(key, "SUBCHATWNDST.X", 0, REG_DWORD, base + 0x914, 4);
    RegSetValueExA(key, "SUBCHATWNDST.Y", 0, REG_DWORD, base + 0x918, 4);
    RegSetValueExA(key, "SUBCHATWNDST.W", 0, REG_DWORD, base + 0x91C, 4);
    RegSetValueExA(key, "SUBCHATWNDST.H", 0, REG_DWORD, base + 0x920, 4);
    RegSetValueExA(key, "SUBCHATWNDBT.X", 0, REG_DWORD, base + 0x924, 4);
    RegSetValueExA(key, "SUBCHATWNDBT.Y", 0, REG_DWORD, base + 0x928, 4);
    RegSetValueExA(key, "SUBCHATWNDBT.W", 0, REG_DWORD, base + 0x92C, 4);
    RegSetValueExA(key, "SUBCHATWNDBT.H", 0, REG_DWORD, base + 0x930, 4);
    RegSetValueExA(key, "CHATWNDSTICKON", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_03F4), 4);
    RegSetValueExA(key, "ONSTSUBCHAT", 0, REG_DWORD, base + 0x3F5, 4);
    RegSetValueExA(key, "ONBTSUBCHAT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_03F6), 4);
    RegSetValueExA(key, "MEMORIALDUNWND.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&g_memorialDunWndX), 4);
    RegSetValueExA(key, "MEMORIALDUNWND.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&g_memorialDunWndY), 4);
    RegSetValueExA(key, "QUICKSLOTWNDINFO.X", 0, REG_DWORD, base + 0x934, 4);
    RegSetValueExA(key, "QUICKSLOTWNDINFO.Y", 0, REG_DWORD, base + 0x938, 4);
    RegSetValueExA(key, "QUICKSLOTWNDINFO.W", 0, REG_DWORD, base + 0x93C, 4);
    RegSetValueExA(key, "QUICKSLOTWNDINFO.H", 0, REG_DWORD, base + 0x940, 4);
    RegSetValueExA(key, "BATTLEFIELDWNDINFO.X", 0, REG_DWORD, base + 0x944, 4);
    RegSetValueExA(key, "BATTLEFIELDWNDINFO.Y", 0, REG_DWORD, base + 0x948, 4);
    RegSetValueExA(key, "BATTLEFIELDWNDINFO.W", 0, REG_DWORD, base + 0x94C, 4);
    RegSetValueExA(key, "BATTLEFIELDWNDINFO.H", 0, REG_DWORD, base + 0x950, 4);
    RegSetValueExA(key, "GUILDHELPERWNDINFO.X", 0, REG_DWORD, base + 0x9D4, 4);
    RegSetValueExA(key, "GUILDHELPERWNDINFO.Y", 0, REG_DWORD, base + 0x9D8, 4);
    RegSetValueExA(key, "GUILDHELPERWNDINFO.W", 0, REG_DWORD, base + 0x9DC, 4);
    RegSetValueExA(key, "GUILDHELPERWNDINFO.H", 0, REG_DWORD, base + 0x9E0, 4);
    RegSetValueExA(key, "BUYINGSTOREMAKEWNDINFO.X", 0, REG_DWORD, base + 0x954, 4);
    RegSetValueExA(key, "BUYINGSTOREMAKEWNDINFO.Y", 0, REG_DWORD, base + 0x958, 4);
    RegSetValueExA(key, "BUYINGSTOREMAKEWNDINFO.W", 0, REG_DWORD, base + 0x95C, 4);
    RegSetValueExA(key, "BUYINGSTOREMAKEWNDINFO.H", 0, REG_DWORD, base + 0x960, 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.X", 0, REG_DWORD, base + 0x964, 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.Y", 0, REG_DWORD, base + 0x968, 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.W", 0, REG_DWORD, base + 0x96C, 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.H", 0, REG_DWORD, base + 0x970, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.X", 0, REG_DWORD, base + 0x974, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.Y", 0, REG_DWORD, base + 0x978, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.W", 0, REG_DWORD, base + 0x97C, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.H", 0, REG_DWORD, base + 0x980, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.X", 0, REG_DWORD, base + 0x984, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.Y", 0, REG_DWORD, base + 0x988, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.W", 0, REG_DWORD, base + 0x98C, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.H", 0, REG_DWORD, base + 0x990, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.X", 0, REG_DWORD, base + 0x994, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.Y", 0, REG_DWORD, base + 0x998, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.W", 0, REG_DWORD, base + 0x99C, 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.H", 0, REG_DWORD, base + 0x9A0, 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.X", 0, REG_DWORD, base + 0x9A4, 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.Y", 0, REG_DWORD, base + 0x9A8, 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.W", 0, REG_DWORD, base + 0x9AC, 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.H", 0, REG_DWORD, base + 0x9B0, 4);
    RegSetValueExA(key, "REPLAYRECCONTROLNWNDINFO.X", 0, REG_DWORD, base + 0x4F154, 4);
    RegSetValueExA(key, "REPLAYRECCONTROLNWNDINFO.Y", 0, REG_DWORD, base + 0x4F158, 4);
    RegSetValueExA(key, "REPLAYRECCONTROLNWNDINFO.W", 0, REG_DWORD, base + 0x4F15C, 4);
    RegSetValueExA(key, "REPLAYRECCONTROLNWNDINFO.H", 0, REG_DWORD, base + 0x4F160, 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.X", 0, REG_DWORD, base + 0x4F178, 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.Y", 0, REG_DWORD, base + 0x4F17C, 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.W", 0, REG_DWORD, base + 0x4F180, 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.H", 0, REG_DWORD, base + 0x4F184, 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.ISSCREEN", 0, REG_DWORD, base + 0x4F188, 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.X", 0, REG_DWORD, base + 0x4F1B0, 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.Y", 0, REG_DWORD, base + 0x4F1B4, 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.SHOWQUEST", 0, REG_DWORD, base + 0x4F1C0, 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.WHOWNPC", 0, REG_DWORD, base + 0x4F1C4, 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.WHOWMEMBER", 0, REG_DWORD, base + 0x4F1C8, 4);
    RegSetValueExA(key, "ACHTRACINGWNDINFO.X", 0, REG_DWORD, base + 0x4F1D8, 4);
    RegSetValueExA(key, "ACHTRACINGWNDINFO.Y", 0, REG_DWORD, base + 0x4F1DC, 4);
    RegSetValueExA(key, "PREVIEWEQUIPWNDINFO.X", 0, REG_DWORD, base + 0x4F220, 4);
    RegSetValueExA(key, "PREVIEWEQUIPWNDINFO.Y", 0, REG_DWORD, base + 0x4F224, 4);
    RegSetValueExA(key, "SWAPEQUIPMENTWNDINFO.X", 0, REG_DWORD, base + 0x4F228, 4);
    RegSetValueExA(key, "SWAPEQUIPMENTWNDINFO.Y", 0, REG_DWORD, base + 0x4F22C, 4);
    RegSetValueExA(key, "PETINFOWNDINFO.X", 0, REG_DWORD, base + 0x8BC, 4);
    RegSetValueExA(key, "PETINFOWNDINFO.Y", 0, REG_DWORD, base + 0x8C0, 4);
    RegSetValueExA(key, "PETINFOWNDINFO.W", 0, REG_DWORD, base + 0x8C4, 4);
    RegSetValueExA(key, "PETINFOWNDINFO.H", 0, REG_DWORD, base + 0x8C8, 4);
    RegSetValueExA(key, "HOMUNINFOWNDINFO.X", 0, REG_DWORD, base + 0x8CC, 4);
    RegSetValueExA(key, "HOMUNINFOWNDINFO.Y", 0, REG_DWORD, base + 0x8D0, 4);
    RegSetValueExA(key, "HOMUNINFOWNDINFO.W", 0, REG_DWORD, base + 0x8D4, 4);
    RegSetValueExA(key, "HOMUNINFOWNDINFO.H", 0, REG_DWORD, base + 0x8D8, 4);
    RegSetValueExA(key, "MERINFOWNDINFO.X", 0, REG_DWORD, base + 0x8F0, 4);
    RegSetValueExA(key, "MERINFOWNDINFO.Y", 0, REG_DWORD, base + 0x8F4, 4);
    RegSetValueExA(key, "MERINFOWNDINFO.W", 0, REG_DWORD, base + 0x8F8, 4);
    RegSetValueExA(key, "MERINFOWNDINFO.H", 0, REG_DWORD, base + 0x8FC, 4);
    RegSetValueExA(key, "ADVEN_REGISTERWNDINFO.X", 0, REG_DWORD, base + 0xA00, 4);
    RegSetValueExA(key, "ADVEN_REGISTERWNDINFO.Y", 0, REG_DWORD, base + 0xA04, 4);
    RegSetValueExA(key, "ADVEN_REGISTERWNDINFO.W", 0, REG_DWORD, base + 0xA08, 4);
    RegSetValueExA(key, "ADVEN_REGISTERWNDINFO.H", 0, REG_DWORD, base + 0xA0C, 4);
    RegSetValueExA(key, "ADVEN_JOINWNDINFO.X", 0, REG_DWORD, base + 0xA10, 4);
    RegSetValueExA(key, "ADVEN_JOINWNDINFO.Y", 0, REG_DWORD, base + 0xA14, 4);
    RegSetValueExA(key, "ADVEN_JOINWNDINFO.W", 0, REG_DWORD, base + 0xA18, 4);
    RegSetValueExA(key, "ADVEN_JOINWNDINFO.H", 0, REG_DWORD, base + 0xA1C, 4);
    RegSetValueExA(key, "ADVEN_PARTYBOARDWNDINFO.X", 0, REG_DWORD, base + 0xA20, 4);
    RegSetValueExA(key, "ADVEN_PARTYBOARDWNDINFO.Y", 0, REG_DWORD, base + 0xA24, 4);
    RegSetValueExA(key, "ADVEN_PARTYBOARDWNDINFO.W", 0, REG_DWORD, base + 0xA28, 4);
    RegSetValueExA(key, "ADVEN_PARTYBOARDWNDINFO.H", 0, REG_DWORD, base + 0xA2C, 4);
    RegCloseKey(key);
  }
}

// OFFSET: 0x00AA9F70
// FUNCTION: fn_17264
void UIWindowMgr::RefreshWindow01C8Edits() {
  if (this->window_01C8) {
    UIEditCtrl* const* edits = reinterpret_cast<UIEditCtrl* const*>(reinterpret_cast<const char*>(this->window_01C8) + 0xBC);
    if (this->hoverWindow == edits[0]) {
      edits[0]->SetText("");
    }

    edits = reinterpret_cast<UIEditCtrl* const*>(reinterpret_cast<const char*>(this->window_01C8) + 0xBC);
    UIEditCtrl* edit = edits[1];
    if (this->hoverWindow == edit) {
      edit->SetText(edit->Text());
    }
  }
}

// OFFSET: 0x00AA9FD0
// FUNCTION: fn_17265
void UIWindowMgr::SetSurface0478(CBitmapRes* bitmap) {
  if (!bitmap) {
    if (this->unknown_0478) {
      delete this->unknown_0478;
      this->unknown_0478 = nullptr;
    }
    return;
  }

  if (!this->unknown_0478) {
    this->unknown_0478 = reinterpret_cast<UIWindow*>(GetRenderSystemHolder()->CreateSurfaceOrTexture(bitmap->image.width, bitmap->image.height));
    if (!this->unknown_0478) {
      return;
    }
  }
  reinterpret_cast<CSurface*>(this->unknown_0478)->Upload(0, 0, bitmap->image.width, bitmap->image.height, bitmap->image.pixels, 0, 0);
}

// OFFSET: 0x00AAA050
// FUNCTION: fn_17266
void UIWindowMgr::SendFlagEffect(int message) {
  CMode* mode = g_modeMgr.gameMode;
  if (!mode) {
    return;
  }

  switch (message) {
  case 210:
    mode->SendMsg(329, reinterpret_cast<int>("ET_FLAG"), 0, 0, 0);
    break;
  case 211:
    mode->SendMsg(329, reinterpret_cast<int>("ET_INDONESIA_FLAG"), 0, 0, 0);
    break;
  case 212:
    mode->SendMsg(329, reinterpret_cast<int>("ET_PH_FLAG"), 0, 0, 0);
    break;
  case 213:
    mode->SendMsg(329, reinterpret_cast<int>("ET_MY_FLAG"), 0, 0, 0);
    break;
  case 214:
    mode->SendMsg(329, reinterpret_cast<int>("ET_SI_FLAG"), 0, 0, 0);
    break;
  case 215:
    mode->SendMsg(329, reinterpret_cast<int>("ET_BR_FLAG"), 0, 0, 0);
    break;
  case 216:
    mode->SendMsg(329, reinterpret_cast<int>("ET_INDIA_FLAG"), 0, 0, 0);
    break;
  case 217:
    mode->SendMsg(329, reinterpret_cast<int>("ET_FLAG8"), 0, 0, 0);
    break;
  case 218:
    mode->SendMsg(329, reinterpret_cast<int>("ET_FLAG9"), 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00A8FCA0
// FUNCTION: fn_17149
bool UIWindowMgr::DropAt(int x, int y, ModeDragState* drag) {
  UIWindow* window = reinterpret_cast<WindowMouseHit*>(this)->HitTest(x, y);
  if (!window) {
    return false;
  }

  int offsetX = 0;
  int offsetY = 0;
  for (UIWindow* node = window; node; node = node->parent) {
    offsetX += node->geometry.x;
    offsetY += node->geometry.y;
  }

  const int localX = x - offsetX;
  const int localY = y - offsetY;
  reinterpret_cast<WindowReplayRecorder*>(window)->RecordDrop(localX, localY, *drag, -1);
  drag->unknown_038C = reinterpret_cast<std::uint32_t>(window);
  if (this->window_01EC != window->Root()) {
    window->Slot10(localX, localY, reinterpret_cast<std::intptr_t>(drag));
  }

  return true;
}

// OFFSET: 0x00A8FD30
// FUNCTION: fn_17150
void UIWindowMgr::FocusWindow01C8Edit() {
  if (!this->window_01C8) {
    return;
  }

  UIWindow* edit = *reinterpret_cast<UIWindow* const*>(reinterpret_cast<const char*>(this->window_01C8) + 0xBC);
  if (this->hoverWindow) {
    this->hoverWindow->Slot24(edit && edit->Slot06() == this->hoverWindow);
  }

  if (edit) {
    this->hoverWindow = edit->Slot06();
    if (this->hoverWindow) {
      this->hoverWindow->Slot23();
    }
  } else {
    this->hoverWindow = nullptr;
  }
}

// OFFSET: 0x00A90120
// FUNCTION: fn_17152
void __stdcall Nullsub369(int, int, int, int) {
}

// OFFSET: 0x00A91DB0
// FUNCTION: fn_17173
void UIWindowMgr::RestoreHiddenWindows() {
  for (UIWindow* window : this->rendered) {
    if (std::find(this->hiddenWindows.begin(), this->hiddenWindows.end(), window) != this->hiddenWindows.end()) {
      window->Show(1);
    }
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode && mode->unknown_0254 == 1) {
    this->EnsureWindow(14);
  }

  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->RefreshEffect164();
  this->unknown_0508 = 1;

  for (std::map<int, UIWindow*>::iterator it = this->overlays.begin(); it != this->overlays.end(); ++it) {
    this->CloseWindow(it->first + 216);
  }
}

// OFFSET: 0x00A91E80
// FUNCTION: fn_17174
void UIWindowMgr::CycleWindowVisibilityTwice() {
  this->CycleWindowVisibility(4);
  this->CycleWindowVisibility(4);
}
