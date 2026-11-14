struct RenderSystemHolder;
__declspec(noinline) RenderSystemHolder* GetRenderSystemHolder();

#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Hotkeys/InterfaceHotkey.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/Registry.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/MercenaryAI.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Textures/Surface.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Login/UIEmailAddressWnd.h"
#include "UI/Navigation/UIMemorialDunWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"
#include "UI/Dialogs/UIMessageBox.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Events/UIRoulletteWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/UIWindowMgr.h"
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

// OFFSET: 0x00A911E0
// FUNCTION: fn_17156
void UIWindowMgr::NotifyHoverWindow() {
  if (this->hoverWindow) {
    this->hoverWindow->OnFinishEdit(0);
  }
}

// OFFSET: 0x00A91400
// FUNCTION: fn_17159
UIWindow* UIWindowMgr::GetGuildWindow() {
  if (this->guildInfoManageWnd) {
    return this->guildInfoManageWnd;
  }
  if (this->guildMemberManageWnd) {
    return this->guildMemberManageWnd;
  }
  if (this->guildPositionManageWnd) {
    return this->guildPositionManageWnd;
  }
  if (this->guildSkillWnd) {
    return this->guildSkillWnd;
  }
  if (this->guildBanishedMemberWnd) {
    return this->guildBanishedMemberWnd;
  }
  if (this->guildNoticeWnd) {
    return this->guildNoticeWnd;
  }
  return this->guildTotalInfoWnd;
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
void UIWindowMgr::HideChatWnd() {
  if (this->chatWnd) {
    this->chatWnd->Show(0);
  }
}

// OFFSET: 0x00A91D90
// FUNCTION: fn_17172
bool CGameActor::HasActiveHomunculusAI() {
  return this->homunAI && this->homunAI->state != nullptr;
}

// OFFSET: 0x00A91F60
// FUNCTION: fn_17175
void UIWindowMgr::RestoreMiniPartyWindows() {
  const int count = static_cast<int>(g_session.miniPartyRecords.size());
  for (int index = 0; index < count; ++index) {
    MessengerMember member = g_session.PartyAt(index);
    if (member.valid == 0) {
      continue;
    }

    const SessionRecord0734 record = g_session.FindMiniPartyRecord(member.accountId);
    UIMiniPartyWnd* window = new UIMiniPartyWnd();
    window->SetSize(174, 52);
    window->m_cell = record.cell;
    this->AddMiniPartyWindow(window);
    window->SendMsg(nullptr, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(&member), 0, 0, 0);
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
      g_session.ResetHotkeyDescriptions();
      g_session.ClearHotkeySkillLevels();
      for (int index = 0; index < 36; ++index) {
        DWORD type = REG_SZ;
        DWORD size = 32;
        char name[36];
        std::sprintf(name, "SITEM%d%d%d", index, g_serviceType, g_session.selectedCharacterSlot);
        char description[33] = {};
        LSTATUS result = RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(description), &size);
        int hasSkillLevel = 0;
        g_session.m_hotkeyPage = static_cast<unsigned char>(index / 9);
        if (result == ERROR_SUCCESS) {
          if (description[0] != '#') {
            g_session.SetHotkeyDescription(description, index);
          } else {
            hasSkillLevel = 1;
          }
        } else {
          g_session.SetHotkeyDescription("nothing", index);
        }

        type = REG_DWORD;
        size = 4;
        std::sprintf(name, "S_SKILL_USE_LEVEL%d%d%d", index, g_serviceType, g_session.selectedCharacterSlot);
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
  char path[512];
  std::strcpy(path, g_subKey);
  std::strcat(path, g_uiRectInfoKeySuffix);

  HKEY probe;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &probe) == ERROR_SUCCESS) {
    RegCloseKey(probe);

    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
      DWORD type = REG_DWORD;
      DWORD size = 4;
      char name[256];

      RegQueryValueExA(key, "m_minimapZoomWnd", nullptr, &type, reinterpret_cast<BYTE*>(&this->minimapZoom), &size);
      if (RegQueryValueExA(key, "M_ISDRAGALL", nullptr, &type, reinterpret_cast<BYTE*>(&this->dragAllItems), &size) != ERROR_SUCCESS) {
        this->dragAllItems = 1;
      }
      if (this->dragAllItems != 0) {
        this->dragAllItems = 1;
      }
      if (RegQueryValueExA(key, "M_ISDRAWCOMPASS", nullptr, &type, reinterpret_cast<BYTE*>(&this->drawCompass), &size) != ERROR_SUCCESS) {
        this->drawCompass = 0;
      }
      if (RegQueryValueExA(key, "M_MINIMAPARGB", nullptr, &type, reinterpret_cast<BYTE*>(&this->minimapColor), &size) != ERROR_SUCCESS) {
        this->minimapColor = 0xAAFFFFFF;
      }
      if (this->minimapColor != 0xFFFFFFFF) {
        this->minimapColor = 0xAAFFFFFF;
      }
      RegQueryValueExA(key, "M_CHATWNDSTATUS", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatWndStatus), &size);
      RegQueryValueExA(key, "M_CHATWNDHEIGHT", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatWndHeight), &size);
      RegQueryValueExA(key, "M_CHATWNDX", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatWndX), &size);
      RegQueryValueExA(key, "M_CHATWNDY", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatWndY), &size);
      RegQueryValueExA(key, "M_CHATWNDSHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatWndVisible), &size);
      RegQueryValueExA(key, "SAYDIALOGWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->sayDialogWndInfo.x), &size);
      RegQueryValueExA(key, "SAYDIALOGWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->sayDialogWndInfo.y), &size);
      RegQueryValueExA(key, "SAYDIALOGWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->sayDialogWndInfo.width), &size);
      RegQueryValueExA(key, "SAYDIALOGWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->sayDialogWndInfo.height), &size);
      RegQueryValueExA(key, "CHOOSEWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->chooseWndInfo.x), &size);
      RegQueryValueExA(key, "CHOOSEWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->chooseWndInfo.y), &size);
      RegQueryValueExA(key, "CHOOSEWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->chooseWndInfo.width), &size);
      RegQueryValueExA(key, "CHOOSEWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->chooseWndInfo.height), &size);
      RegQueryValueExA(key, "BASICINFOWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->basicInfoWndInfo.x), &size);
      RegQueryValueExA(key, "BASICINFOWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->basicInfoWndInfo.y), &size);
      RegQueryValueExA(key, "BASICINFOWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->basicInfoWndInfo.width), &size);
      RegQueryValueExA(key, "BASICINFOWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->basicInfoWndInfo.height), &size);
      RegQueryValueExA(key, "BASICINFOWNDINFO.ISSHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->basicInfoWndInfo.mode), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.ORGHEIGHT", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.expandedHeight), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.CURTAB", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.selectedTab), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.CURRADIO1", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.usableRows), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.CURRADIO2", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.equipmentRows), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.CURRADIO3", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.etcRows), &size);
      RegQueryValueExA(key, "ITEMWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemWndInfo.visible), &size);
      RegQueryValueExA(key, "QUESTWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->questWndPlacement[0]), &size);
      RegQueryValueExA(key, "QUESTWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->questWndPlacement[1]), &size);
      RegQueryValueExA(key, "QUESTWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->questWndPlacement[2]), &size);
      RegQueryValueExA(key, "QUESTWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->questWndPlacement[3]), &size);
      RegQueryValueExA(key, "QUESTWNDINFO.ORGHEIGHT", nullptr, &type, reinterpret_cast<BYTE*>(&this->questWndPlacement[4]), &size);
      RegQueryValueExA(key, "QUESTWNDINFO.CURTAB", nullptr, &type, reinterpret_cast<BYTE*>(&this->questWndPlacement[5]), &size);
      RegQueryValueExA(key, "QUESTWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->questWndVisible), &size);
      RegQueryValueExA(key, "QUESTDISPWNDINFO.QUESTDISPLAY", nullptr, &type, reinterpret_cast<BYTE*>(&this->questDisplayInfo.visible), &size);
      RegQueryValueExA(key, "STATUSWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->statusWndInfo.x), &size);
      RegQueryValueExA(key, "STATUSWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->statusWndInfo.y), &size);
      RegQueryValueExA(key, "STATUSWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->statusWndInfo.width), &size);
      RegQueryValueExA(key, "STATUSWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->statusWndInfo.height), &size);
      RegQueryValueExA(key, "STATUSWNDINFO.ORGHEIGHT", nullptr, &type, reinterpret_cast<BYTE*>(&this->statusWndInfo.expandedHeight), &size);
      if (RegQueryValueExA(key, "STATUSWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->statusWndInfo.visible), &size) != ERROR_SUCCESS) {
        this->statusWndInfo.visible = 0;
      }
      RegQueryValueExA(key, "EQUIPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->equipWndInfo.x), &size);
      RegQueryValueExA(key, "EQUIPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->equipWndInfo.y), &size);
      RegQueryValueExA(key, "EQUIPWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->equipWndInfo.width), &size);
      RegQueryValueExA(key, "EQUIPWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->equipWndInfo.height), &size);
      RegQueryValueExA(key, "EQUIPWNDINFO.ORGHEIGHT", nullptr, &type, reinterpret_cast<BYTE*>(&this->equipWndInfo.contentHeight), &size);
      if (RegQueryValueExA(key, "EQUIPWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->equipWndInfo.visible), &size) != ERROR_SUCCESS) {
        this->equipWndInfo.visible = 0;
      }
      RegQueryValueExA(key, "OPTIONWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->optionWndInfo.x), &size);
      RegQueryValueExA(key, "OPTIONWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->optionWndInfo.y), &size);
      RegQueryValueExA(key, "OPTIONWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->optionWndInfo.width), &size);
      RegQueryValueExA(key, "OPTIONWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->optionWndInfo.height), &size);
      RegQueryValueExA(key, "OPTIONWNDINFO.ORGHEIGHT", nullptr, &type, reinterpret_cast<BYTE*>(&this->optionWndInfo.defaultHeight), &size);
      if (RegQueryValueExA(key, "OPTIONWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->optionWndInfo.visible), &size) != ERROR_SUCCESS) {
        this->optionWndInfo.visible = 0;
      }
      RegQueryValueExA(key, "SHORTENITEMWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->shortenItemWndInfo.x), &size);
      RegQueryValueExA(key, "SHORTENITEMWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->shortenItemWndInfo.y), &size);
      RegQueryValueExA(key, "SHORTENITEMWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->shortenItemWndInfo.width), &size);
      RegQueryValueExA(key, "SHORTENITEMWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->shortenItemWndInfo.height), &size);
      if (RegQueryValueExA(key, "SHORTENITEMWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->shortenItemWndInfo.visible), &size) != ERROR_SUCCESS) {
        this->shortenItemWndInfo.visible = 0;
      }
      RegQueryValueExA(key, "CHATROOMWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatRoomWndInfo.x), &size);
      RegQueryValueExA(key, "CHATROOMWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatRoomWndInfo.y), &size);
      RegQueryValueExA(key, "CHATROOMWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatRoomWndInfo.width), &size);
      RegQueryValueExA(key, "CHATROOMWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatRoomWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMSTOREWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindListInfo.x), &size);
      RegQueryValueExA(key, "ITEMSTOREWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindListInfo.y), &size);
      RegQueryValueExA(key, "ITEMSTOREWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindListInfo.width), &size);
      RegQueryValueExA(key, "ITEMSTOREWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindListInfo.height), &size);
      RegQueryValueExA(key, "ITEMSTOREWNDINFO.subWnd", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindListInfo.openTabs), &size);

      std::memset(name, 0, sizeof(name) - 1);
      RegQueryValueExA(key, "ITEMSTOREWNDNUM", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreWindowCount), &size);
      if (this->itemStoreWindowCount != 8) {
        for (int index = 0; index < 8; ++index) {
          this->itemStoreFindSlots[index].Reset(index);
        }
        this->itemStoreWindowCount = 8;
        this->itemStoreFindListInfo.Reset();
      } else {
        for (int index = 0; index < 8; ++index) {
          std::sprintf(name, "ITEMSTOREWNDINFO_%d.X", index);
          RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindSlots[0].x) + index * 0x10, &size);
          std::sprintf(name, "ITEMSTOREWNDINFO_%d.Y", index);
          RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindSlots[0].y) + index * 0x10, &size);
          std::sprintf(name, "ITEMSTOREWNDINFO_%d.W", index);
          RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindSlots[0].width) + index * 0x10, &size);
          std::sprintf(name, "ITEMSTOREWNDINFO_%d.H", index);
          RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindSlots[0].height) + index * 0x10, &size);
        }
      }
      RegQueryValueExA(key, "ITEMSTOREWNDINFO.findWnd", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindWndCount), &size);
      RegQueryValueExA(key, "ITEMSTOREFINDWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMSTOREFINDWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMSTOREFINDWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMSTOREFINDWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemStoreFindWndInfo.height), &size);
      RegQueryValueExA(key, "BOOKINGLISTWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyListWndInfo.x), &size);
      RegQueryValueExA(key, "BOOKINGLISTWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyListWndInfo.y), &size);
      RegQueryValueExA(key, "BOOKINGLISTWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyListWndInfo.width), &size);
      RegQueryValueExA(key, "BOOKINGLISTWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyListWndInfo.height), &size);
      RegQueryValueExA(key, "BOOKINGMBWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyMBWndInfo.x), &size);
      RegQueryValueExA(key, "BOOKINGMBWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyMBWndInfo.y), &size);
      RegQueryValueExA(key, "BOOKINGMBWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyMBWndInfo.width), &size);
      RegQueryValueExA(key, "BOOKINGMBWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyMBWndInfo.height), &size);
      RegQueryValueExA(key, "BOOKINGWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyWndInfo.x), &size);
      RegQueryValueExA(key, "BOOKINGWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyWndInfo.y), &size);
      RegQueryValueExA(key, "BOOKINGWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyWndInfo.width), &size);
      RegQueryValueExA(key, "BOOKINGWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->seekPartyWndInfo.height), &size);
      RegQueryValueExA(key, "BOOKINGMBCHECKWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->jobListWndInfo.x), &size);
      RegQueryValueExA(key, "BOOKINGMBCHECKWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->jobListWndInfo.y), &size);
      RegQueryValueExA(key, "BOOKINGMBCHECKWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->jobListWndInfo.width), &size);
      RegQueryValueExA(key, "BOOKINGMBCHECKWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->jobListWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMSHOPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemShopWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMSHOPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemShopWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMSHOPWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemShopWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMSHOPWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemShopWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMSELLWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemSellWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMSELLWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemSellWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMSELLWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemSellWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMSELLWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemSellWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMPURCHASEWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemPurchaseWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMPURCHASEWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemPurchaseWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMPURCHASEWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemPurchaseWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMPURCHASEWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemPurchaseWndInfo.height), &size);
      RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->messengerGroupWndInfo.x), &size);
      RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->messengerGroupWndInfo.y), &size);
      RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->messengerGroupWndInfo.width), &size);
      RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->messengerGroupWndInfo.height), &size);
      if (RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->messengerGroupWndInfo.visible), &size) != ERROR_SUCCESS) {
        this->messengerGroupWndInfo.visible = 0;
      }
      if (RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.RADIO", nullptr, &type, reinterpret_cast<BYTE*>(&this->messengerGroupWndInfo.mode), &size) != ERROR_SUCCESS) {
        this->messengerGroupWndInfo.mode = 1;
      }
      if (RegQueryValueExA(key, "MESSENGERGROUPWNDINFO.SIZEMODE", nullptr, &type, reinterpret_cast<BYTE*>(&this->messengerGroupWndInfo.iconIndex), &size) != ERROR_SUCCESS) {
        this->messengerGroupWndInfo.iconIndex = 0;
      }
      RegQueryValueExA(key, "EXCHANGEACCEPTWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->exchangeAcceptWndInfo.x), &size);
      RegQueryValueExA(key, "EXCHANGEACCEPTWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->exchangeAcceptWndInfo.y), &size);
      RegQueryValueExA(key, "EXCHANGEACCEPTWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->exchangeAcceptWndInfo.width), &size);
      RegQueryValueExA(key, "EXCHANGEACCEPTWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->exchangeAcceptWndInfo.height), &size);
      RegQueryValueExA(key, "SKILLLISTWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->skillListWndInfo.x), &size);
      RegQueryValueExA(key, "SKILLLISTWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->skillListWndInfo.y), &size);
      RegQueryValueExA(key, "SKILLLISTWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->skillListWndInfo.width), &size);
      RegQueryValueExA(key, "SKILLLISTWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->skillListWndInfo.height), &size);
      if (RegQueryValueExA(key, "SKILLLISTWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->skillListWndVisible), &size) != ERROR_SUCCESS) {
        this->skillListWndVisible = 0;
      }
      RegQueryValueExA(key, "MERCHANTITEMWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemWndInfo.x), &size);
      RegQueryValueExA(key, "MERCHANTITEMWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemWndInfo.y), &size);
      RegQueryValueExA(key, "MERCHANTITEMWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemWndInfo.width), &size);
      RegQueryValueExA(key, "MERCHANTITEMWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemWndInfo.height), &size);
      if (RegQueryValueExA(key, "MERCHANTITEMWNDINFO.SHOW", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemWndVisible), &size) != ERROR_SUCCESS) {
        this->merchantItemWndVisible = 0;
      }
      RegQueryValueExA(key, "MERCHANTMIRRORITEMWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantMirrorItemWndInfo.x), &size);
      RegQueryValueExA(key, "MERCHANTMIRRORITEMWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantMirrorItemWndInfo.y), &size);
      RegQueryValueExA(key, "MERCHANTMIRRORITEMWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantMirrorItemWndInfo.width), &size);
      RegQueryValueExA(key, "MERCHANTMIRRORITEMWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantMirrorItemWndInfo.height), &size);
      RegQueryValueExA(key, "MERCHANTSHOPMAKEWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantShopMakeWndInfo.x), &size);
      RegQueryValueExA(key, "MERCHANTSHOPMAKEWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantShopMakeWndInfo.y), &size);
      RegQueryValueExA(key, "MERCHANTSHOPMAKEWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantShopMakeWndInfo.width), &size);
      RegQueryValueExA(key, "MERCHANTSHOPMAKEWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantShopMakeWndInfo.height), &size);
      RegQueryValueExA(key, "MERCHANTITEMSHOPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemShopWndInfo.x), &size);
      RegQueryValueExA(key, "MERCHANTITEMSHOPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemShopWndInfo.y), &size);
      RegQueryValueExA(key, "MERCHANTITEMSHOPWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemShopWndInfo.width), &size);
      RegQueryValueExA(key, "MERCHANTITEMSHOPWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemShopWndInfo.height), &size);
      RegQueryValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemMyShopWndInfo.x), &size);
      RegQueryValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemMyShopWndInfo.y), &size);
      RegQueryValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemMyShopWndInfo.width), &size);
      RegQueryValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemMyShopWndInfo.height), &size);
      RegQueryValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemPurchaseWndInfo.x), &size);
      RegQueryValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemPurchaseWndInfo.y), &size);
      RegQueryValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemPurchaseWndInfo.width), &size);
      RegQueryValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->merchantItemPurchaseWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMCOLLECTIONWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemCollectionWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMCOLLECTIONWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemCollectionWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMCOLLECTIONWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemCollectionWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMCOLLECTIONWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemCollectionWndInfo.height), &size);
      RegQueryValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->combinedCardItemCollectionWndInfo.x), &size);
      RegQueryValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->combinedCardItemCollectionWndInfo.y), &size);
      RegQueryValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->combinedCardItemCollectionWndInfo.width), &size);
      RegQueryValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->combinedCardItemCollectionWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemParamChangeDisplayWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemParamChangeDisplayWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemParamChangeDisplayWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemParamChangeDisplayWndInfo.height), &size);
      RegQueryValueExA(key, "PARTYSETTINGWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->partySettingWndInfo.x), &size);
      RegQueryValueExA(key, "PARTYSETTINGWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->partySettingWndInfo.y), &size);
      RegQueryValueExA(key, "PARTYSETTINGWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->partySettingWndInfo.width), &size);
      RegQueryValueExA(key, "PARTYSETTINGWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->partySettingWndInfo.height), &size);
      RegQueryValueExA(key, "DETAILLEVELWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->unusedWndInfoA.x), &size);
      RegQueryValueExA(key, "DETAILLEVELWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->unusedWndInfoA.y), &size);
      RegQueryValueExA(key, "DETAILLEVELWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->unusedWndInfoA.width), &size);
      RegQueryValueExA(key, "DETAILLEVELWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->unusedWndInfoA.height), &size);
      RegQueryValueExA(key, "WHISPERWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->m_whisperPlacement.x), &size);
      RegQueryValueExA(key, "WHISPERWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->m_whisperPlacement.y), &size);
      RegQueryValueExA(key, "WHISPERWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->m_whisperPlacement.width), &size);
      RegQueryValueExA(key, "WHISPERWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->m_whisperPlacement.height), &size);
      RegQueryValueExA(key, "FRIENDOPTIONWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->friendOptionWndInfo.x), &size);
      RegQueryValueExA(key, "FRIENDOPTIONWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->friendOptionWndInfo.y), &size);
      RegQueryValueExA(key, "FRIENDOPTIONWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->friendOptionWndInfo.width), &size);
      RegQueryValueExA(key, "FRIENDOPTIONWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->friendOptionWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMCOMPOSITIONWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemCompositionWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMCOMPOSITIONWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemCompositionWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMCOMPOSITIONWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemCompositionWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMCOMPOSITIONWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemCompositionWndInfo.height), &size);
      RegQueryValueExA(key, "ITEMIDENTIFYWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemIdentifyWndInfo.x), &size);
      RegQueryValueExA(key, "ITEMIDENTIFYWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemIdentifyWndInfo.y), &size);
      RegQueryValueExA(key, "ITEMIDENTIFYWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemIdentifyWndInfo.width), &size);
      RegQueryValueExA(key, "ITEMIDENTIFYWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->itemIdentifyWndInfo.height), &size);
      RegQueryValueExA(key, "SUBCHATWNDST.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->subChatHisWndInfo[0].x), &size);
      RegQueryValueExA(key, "SUBCHATWNDST.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->subChatHisWndInfo[0].y), &size);
      RegQueryValueExA(key, "SUBCHATWNDST.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->subChatHisWndInfo[0].width), &size);
      RegQueryValueExA(key, "SUBCHATWNDST.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->subChatHisWndInfo[0].height), &size);
      RegQueryValueExA(key, "SUBCHATWNDBT.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->subChatHisWndInfo[1].x), &size);
      RegQueryValueExA(key, "SUBCHATWNDBT.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->subChatHisWndInfo[1].y), &size);
      RegQueryValueExA(key, "SUBCHATWNDBT.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->subChatHisWndInfo[1].width), &size);
      RegQueryValueExA(key, "SUBCHATWNDBT.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->subChatHisWndInfo[1].height), &size);
      RegQueryValueExA(key, "CHATWNDSTICKON", nullptr, &type, reinterpret_cast<BYTE*>(&this->chatWndStickOn), &size);
      RegQueryValueExA(key, "ONSTSUBCHAT", nullptr, &type, reinterpret_cast<BYTE*>(&this->onStSubChat), &size);
      RegQueryValueExA(key, "ONBTSUBCHAT", nullptr, &type, reinterpret_cast<BYTE*>(&this->battleSubChatEnabled), &size);
      RegQueryValueExA(key, "MEMORIALDUNWND.X", nullptr, &type, reinterpret_cast<BYTE*>(&g_session.memorialWindowX), &size);
      RegQueryValueExA(key, "MEMORIALDUNWND.Y", nullptr, &type, reinterpret_cast<BYTE*>(&g_session.memorialWindowY), &size);
      RegQueryValueExA(key, "QUICKSLOTWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->quickSlotWndInfo.x), &size);
      RegQueryValueExA(key, "QUICKSLOTWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->quickSlotWndInfo.y), &size);
      RegQueryValueExA(key, "QUICKSLOTWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->quickSlotWndInfo.width), &size);
      RegQueryValueExA(key, "QUICKSLOTWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->quickSlotWndInfo.height), &size);
      RegQueryValueExA(key, "BATTLEFIELDWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->entryQueueWndInfo.x), &size);
      RegQueryValueExA(key, "BATTLEFIELDWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->entryQueueWndInfo.y), &size);
      RegQueryValueExA(key, "BATTLEFIELDWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->entryQueueWndInfo.width), &size);
      RegQueryValueExA(key, "BATTLEFIELDWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->entryQueueWndInfo.height), &size);
      RegQueryValueExA(key, "GUILDHELPERWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->entryQueueStandbyWndInfo.x), &size);
      RegQueryValueExA(key, "GUILDHELPERWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->entryQueueStandbyWndInfo.y), &size);
      RegQueryValueExA(key, "GUILDHELPERWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->entryQueueStandbyWndInfo.width), &size);
      RegQueryValueExA(key, "GUILDHELPERWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->entryQueueStandbyWndInfo.height), &size);
      RegQueryValueExA(key, "BUYINGSTOREMAKEWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingShopMakeWndInfo.x), &size);
      RegQueryValueExA(key, "BUYINGSTOREMAKEWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingShopMakeWndInfo.y), &size);
      RegQueryValueExA(key, "BUYINGSTOREMAKEWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingShopMakeWndInfo.width), &size);
      RegQueryValueExA(key, "BUYINGSTOREMAKEWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingShopMakeWndInfo.height), &size);
      RegQueryValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingMirrorItemWndInfo.x), &size);
      RegQueryValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingMirrorItemWndInfo.y), &size);
      RegQueryValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingMirrorItemWndInfo.width), &size);
      RegQueryValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingMirrorItemWndInfo.height), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemMyShopWndInfo.x), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemMyShopWndInfo.y), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemMyShopWndInfo.width), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemMyShopWndInfo.height), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemShopWndInfo.x), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemShopWndInfo.y), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemShopWndInfo.width), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemShopWndInfo.height), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemPurchaseWndInfo.x), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemPurchaseWndInfo.y), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemPurchaseWndInfo.width), &size);
      RegQueryValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingItemPurchaseWndInfo.height), &size);
      RegQueryValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingMirrorItemWndInfo2.x), &size);
      RegQueryValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingMirrorItemWndInfo2.y), &size);
      RegQueryValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingMirrorItemWndInfo2.width), &size);
      RegQueryValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->buyingMirrorItemWndInfo2.height), &size);
      RegQueryValueExA(key, "REPLAYRECCONTROLWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->replayRecControlWndInfo.x), &size);
      RegQueryValueExA(key, "REPLAYRECCONTROLWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->replayRecControlWndInfo.y), &size);
      RegQueryValueExA(key, "REPLAYRECCONTROLWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->replayRecControlWndInfo.width), &size);
      RegQueryValueExA(key, "REPLAYRECCONTROLWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->replayRecControlWndInfo.height), &size);
      RegQueryValueExA(key, "NAVIGATIONWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->navigationWindowInfo.x), &size);
      RegQueryValueExA(key, "NAVIGATIONWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->navigationWindowInfo.y), &size);
      RegQueryValueExA(key, "NAVIGATIONWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->navigationWindowInfo.width), &size);
      RegQueryValueExA(key, "NAVIGATIONWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->navigationWindowInfo.height), &size);
      RegQueryValueExA(key, "NAVIGATIONWNDINFO.ISSCREEN", nullptr, &type, reinterpret_cast<BYTE*>(&this->navigationWindowInfo.expanded), &size);
      this->miniMapWndInfo.Reset();
      RegQueryValueExA(key, "MINIMAPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->miniMapWndInfo.width), &size);
      RegQueryValueExA(key, "MINIMAPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->miniMapWndInfo.height), &size);
      RegQueryValueExA(key, "MINIMAPWNDINFO.SHOWQUEST", nullptr, &type, reinterpret_cast<BYTE*>(&this->miniMapWndInfo.showQuests), &size);
      RegQueryValueExA(key, "MINIMAPWNDINFO.WHOWNPC", nullptr, &type, reinterpret_cast<BYTE*>(&this->miniMapWndInfo.showTowns), &size);
      RegQueryValueExA(key, "MINIMAPWNDINFO.WHOWMEMBER", nullptr, &type, reinterpret_cast<BYTE*>(&this->miniMapWndInfo.showMembers), &size);
      RegQueryValueExA(key, "ACHTRACINGWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->achTracingWndInfo.x), &size);
      RegQueryValueExA(key, "ACHTRACINGWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->achTracingWndInfo.y), &size);
      RegQueryValueExA(key, "PREVIEWEQUIPWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->previewEquipWndInfo.x), &size);
      RegQueryValueExA(key, "PREVIEWEQUIPWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->previewEquipWndInfo.y), &size);
      RegQueryValueExA(key, "SWAPEQUIPMENTWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->swapEquipmentPlacement[0]), &size);
      RegQueryValueExA(key, "SWAPEQUIPMENTWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->swapEquipmentPlacement[1]), &size);
      RegQueryValueExA(key, "PETINFOWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->petInfoWndInfo.x), &size);
      RegQueryValueExA(key, "PETINFOWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->petInfoWndInfo.y), &size);
      RegQueryValueExA(key, "PETINFOWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->petInfoWndInfo.width), &size);
      RegQueryValueExA(key, "PETINFOWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->petInfoWndInfo.height), &size);
      RegQueryValueExA(key, "HOMUNINFOWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->homunInfoWndInfo.x), &size);
      RegQueryValueExA(key, "HOMUNINFOWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->homunInfoWndInfo.y), &size);
      RegQueryValueExA(key, "HOMUNINFOWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->homunInfoWndInfo.width), &size);
      RegQueryValueExA(key, "HOMUNINFOWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->homunInfoWndInfo.height), &size);
      RegQueryValueExA(key, "MERINFOWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->merInfoWndInfo.x), &size);
      RegQueryValueExA(key, "MERINFOWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->merInfoWndInfo.y), &size);
      RegQueryValueExA(key, "MERINFOWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->merInfoWndInfo.width), &size);
      RegQueryValueExA(key, "MERINFOWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->merInfoWndInfo.height), &size);
      RegQueryValueExA(key, "ADVEN_REGISTERWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->registerPartyWndInfo.x), &size);
      RegQueryValueExA(key, "ADVEN_REGISTERWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->registerPartyWndInfo.y), &size);
      RegQueryValueExA(key, "ADVEN_REGISTERWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->registerPartyWndInfo.width), &size);
      RegQueryValueExA(key, "ADVEN_REGISTERWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->registerPartyWndInfo.height), &size);
      RegQueryValueExA(key, "ADVEN_JOINWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->applyForPartyWndInfo.x), &size);
      RegQueryValueExA(key, "ADVEN_JOINWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->applyForPartyWndInfo.y), &size);
      RegQueryValueExA(key, "ADVEN_JOINWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->applyForPartyWndInfo.width), &size);
      RegQueryValueExA(key, "ADVEN_JOINWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->applyForPartyWndInfo.height), &size);
      RegQueryValueExA(key, "ADVEN_PARTYBOARDWNDINFO.X", nullptr, &type, reinterpret_cast<BYTE*>(&this->advenPartyBoardWndInfo.x), &size);
      RegQueryValueExA(key, "ADVEN_PARTYBOARDWNDINFO.Y", nullptr, &type, reinterpret_cast<BYTE*>(&this->advenPartyBoardWndInfo.y), &size);
      RegQueryValueExA(key, "ADVEN_PARTYBOARDWNDINFO.W", nullptr, &type, reinterpret_cast<BYTE*>(&this->advenPartyBoardWndInfo.width), &size);
      RegQueryValueExA(key, "ADVEN_PARTYBOARDWNDINFO.H", nullptr, &type, reinterpret_cast<BYTE*>(&this->advenPartyBoardWndInfo.height), &size);
      RegCloseKey(key);
    }
  } else {
    this->ResetUIRectInfo();
  }
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
  UIWindow* window;
  if (this->choose3Wnd) {
    window = this->choose3Wnd;
  } else if (this->sayDialogWnd) {
    window = this->sayDialogWnd;
  } else if (this->sayDialogOverlayWnd) {
    window = this->sayDialogOverlayWnd;
  } else if (this->comboBoxWnd) {
    window = this->comboBoxWnd;
  } else if (this->comboBoxIDWnd) {
    window = this->comboBoxIDWnd;
  } else if (this->chooseWarpWnd) {
    window = this->chooseWarpWnd;
  } else if (this->makeTargetListWnd) {
    window = this->makeTargetListWnd;
  } else if (this->hoverWindow) {
    window = this->hoverWindow;
  } else {
    for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
      if ((*entry)->IsFrameWnd()) {
        (*entry)->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
      }
    }
    return;
  }

  window->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
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
  for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    int id = (*it)->geometry.id;
    if (id == 8) {
      if (this->itemWndInfo.visible) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 10) {
      if (this->equipWndInfo.visible) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 11) {
      if (this->statusWndInfo.visible) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 13) {
      if (this->optionWndInfo.visible) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 37) {
      if (this->skillListWndVisible) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 321) {
      if (this->questWndVisible) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 205) {
      if (this->questWndVisible) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 134) {
      if (this->questWndVisible) {
        this->savedWindowIds.push_back(id);
      }
    } else if ((id == 69 || id == 34) && this->messengerGroupWndInfo.visible) {
      this->savedWindowIds.push_back(id);
    } else if (id == 271) {
      if (g_session.HasTrackedAchievements() == true) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 286) {
      if (this->macroReporterVisible == true) {
        this->savedWindowIds.push_back(id);
      }
    } else if (id == 10014) {
      if (g_session.lowMemory && g_session.performanceWarningEnabled && g_effectDetailLevel == 2 && g_textureDetailLevel == 2) {
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
  if (this->navigationWnd && !g_navigationManager.expandedMap && static_cast<UINavigationV4Wnd*>(this->navigationWnd)->IsListShown()) {
    this->navigationWnd->SendMsg(nullptr, UM_COMMAND, ID_MINI, 0, 0, 0);
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
    std::sprintf(name, "SITEM%d%d%d", index, g_serviceType, g_session.selectedCharacterSlot);
    RegSetValueExA(key, name, 0, REG_SZ, reinterpret_cast<const BYTE*>(description), 32);

    DWORD level = g_session.GetHotkeySkillLevel(index);
    std::sprintf(name, "S_SKILL_USE_LEVEL%d%d%d", index, g_serviceType, g_session.selectedCharacterSlot);
    RegSetValueExA(key, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&level), 4);
  }

  RegCloseKey(key);
}

// OFFSET: 0x00AABB40
// FUNCTION: fn_17284
void UIWindowMgr::SaveUIRectInfo() {
  char path[512];
  std::strcpy(path, g_subKey);
  std::strcat(path, g_uiRectInfoKeySuffix);

  HKEY key;
  DWORD disposition = 0;
  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, path, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
    char name[256];
    RegSetValueExA(key, "m_minimapZoomWnd", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->minimapZoom), 4);
    RegSetValueExA(key, "M_ISDRAGALL", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->dragAllItems), 4);
    RegSetValueExA(key, "M_ISDRAWCOMPASS", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->drawCompass), 4);
    RegSetValueExA(key, "M_MINIMAPARGB", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->minimapColor), 4);
    RegSetValueExA(key, "M_CHATWNDSTATUS", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatWndStatus), 4);
    RegSetValueExA(key, "M_CHATWNDHEIGHT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatWndHeight), 4);
    RegSetValueExA(key, "M_CHATWNDX", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatWndX), 4);
    RegSetValueExA(key, "M_CHATWNDY", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatWndY), 4);
    RegSetValueExA(key, "M_CHATWNDSHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatWndVisible), 4);
    RegSetValueExA(key, "SAYDIALOGWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->sayDialogWndInfo.x), 4);
    RegSetValueExA(key, "SAYDIALOGWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->sayDialogWndInfo.y), 4);
    RegSetValueExA(key, "SAYDIALOGWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->sayDialogWndInfo.width), 4);
    RegSetValueExA(key, "SAYDIALOGWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->sayDialogWndInfo.height), 4);
    RegSetValueExA(key, "CHOOSEWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chooseWndInfo.x), 4);
    RegSetValueExA(key, "CHOOSEWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chooseWndInfo.y), 4);
    RegSetValueExA(key, "CHOOSEWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chooseWndInfo.width), 4);
    RegSetValueExA(key, "CHOOSEWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chooseWndInfo.height), 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->basicInfoWndInfo.x), 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->basicInfoWndInfo.y), 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->basicInfoWndInfo.width), 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->basicInfoWndInfo.height), 4);
    RegSetValueExA(key, "BASICINFOWNDINFO.ISSHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->basicInfoWndInfo.mode), 4);
    RegSetValueExA(key, "ITEMWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.x), 4);
    RegSetValueExA(key, "ITEMWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.y), 4);
    RegSetValueExA(key, "ITEMWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.width), 4);
    RegSetValueExA(key, "ITEMWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.height), 4);
    RegSetValueExA(key, "ITEMWNDINFO.ORGHEIGHT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.expandedHeight), 4);
    RegSetValueExA(key, "ITEMWNDINFO.CURTAB", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.selectedTab), 4);
    RegSetValueExA(key, "ITEMWNDINFO.CURRADIO1", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.usableRows), 4);
    RegSetValueExA(key, "ITEMWNDINFO.CURRADIO2", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.equipmentRows), 4);
    RegSetValueExA(key, "ITEMWNDINFO.CURRADIO3", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.etcRows), 4);
    RegSetValueExA(key, "ITEMWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemWndInfo.visible), 4);
    RegSetValueExA(key, "QUESTWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->questWndPlacement[0]), 4);
    RegSetValueExA(key, "QUESTWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->questWndPlacement[1]), 4);
    RegSetValueExA(key, "QUESTWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->questWndPlacement[2]), 4);
    RegSetValueExA(key, "QUESTWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->questWndPlacement[3]), 4);
    RegSetValueExA(key, "QUESTWNDINFO.ORGHEIGHT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->questWndPlacement[4]), 4);
    RegSetValueExA(key, "QUESTWNDINFO.CURTAB", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->questWndPlacement[5]), 4);
    RegSetValueExA(key, "QUESTWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->questWndVisible), 4);
    RegSetValueExA(key, "QUESTDISPWNDINFO.QUESTDISPLAY", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->questDisplayInfo.visible), 4);
    RegSetValueExA(key, "STATUSWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->statusWndInfo.x), 4);
    RegSetValueExA(key, "STATUSWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->statusWndInfo.y), 4);
    RegSetValueExA(key, "STATUSWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->statusWndInfo.width), 4);
    RegSetValueExA(key, "STATUSWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->statusWndInfo.height), 4);
    RegSetValueExA(key, "STATUSWNDINFO.ORGHEIGHT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->statusWndInfo.expandedHeight), 4);
    RegSetValueExA(key, "STATUSWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->statusWndInfo.visible), 4);
    RegSetValueExA(key, "EQUIPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->equipWndInfo.x), 4);
    RegSetValueExA(key, "EQUIPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->equipWndInfo.y), 4);
    RegSetValueExA(key, "EQUIPWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->equipWndInfo.width), 4);
    RegSetValueExA(key, "EQUIPWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->equipWndInfo.height), 4);
    RegSetValueExA(key, "EQUIPWNDINFO.ORGHEIGHT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->equipWndInfo.contentHeight), 4);
    RegSetValueExA(key, "EQUIPWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->equipWndInfo.visible), 4);
    RegSetValueExA(key, "OPTIONWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->optionWndInfo.x), 4);
    RegSetValueExA(key, "OPTIONWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->optionWndInfo.y), 4);
    RegSetValueExA(key, "OPTIONWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->optionWndInfo.width), 4);
    RegSetValueExA(key, "OPTIONWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->optionWndInfo.height), 4);
    RegSetValueExA(key, "OPTIONWNDINFO.ORGHEIGHT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->optionWndInfo.defaultHeight), 4);
    RegSetValueExA(key, "OPTIONWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->optionWndInfo.visible), 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->shortenItemWndInfo.x), 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->shortenItemWndInfo.y), 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->shortenItemWndInfo.width), 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->shortenItemWndInfo.height), 4);
    RegSetValueExA(key, "SHORTENITEMWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->shortenItemWndInfo.visible), 4);
    RegSetValueExA(key, "CHATROOMWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatRoomWndInfo.x), 4);
    RegSetValueExA(key, "CHATROOMWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatRoomWndInfo.y), 4);
    RegSetValueExA(key, "CHATROOMWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatRoomWndInfo.width), 4);
    RegSetValueExA(key, "CHATROOMWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatRoomWndInfo.height), 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindListInfo.x), 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindListInfo.y), 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindListInfo.width), 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindListInfo.height), 4);
    RegSetValueExA(key, "ITEMSTOREWNDINFO.subWnd", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindListInfo.openTabs), 4);
    std::memset(name, 0, sizeof(name) - 1);
    RegSetValueExA(key, "ITEMSTOREWNDNUM", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreWindowCount), 4);
    for (int index = 0; index < 8; ++index) {
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.X", index);
      RegSetValueExA(key, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindSlots[0].x) + index * 0x10, 4);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.Y", index);
      RegSetValueExA(key, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindSlots[0].y) + index * 0x10, 4);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.W", index);
      RegSetValueExA(key, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindSlots[0].width) + index * 0x10, 4);
      std::sprintf(name, "ITEMSTOREWNDINFO_%d.H", index);
      RegSetValueExA(key, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindSlots[0].height) + index * 0x10, 4);
    }
    RegSetValueExA(key, "ITEMSTOREWNDINFO.findWnd", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindWndCount), 4);
    RegSetValueExA(key, "ITEMSTOREFINDWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindWndInfo.x), 4);
    RegSetValueExA(key, "ITEMSTOREFINDWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindWndInfo.y), 4);
    RegSetValueExA(key, "ITEMSTOREFINDWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindWndInfo.width), 4);
    RegSetValueExA(key, "ITEMSTOREFINDWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemStoreFindWndInfo.height), 4);
    RegSetValueExA(key, "BOOKINGLISTWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyListWndInfo.x), 4);
    RegSetValueExA(key, "BOOKINGLISTWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyListWndInfo.y), 4);
    RegSetValueExA(key, "BOOKINGLISTWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyListWndInfo.width), 4);
    RegSetValueExA(key, "BOOKINGLISTWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyListWndInfo.height), 4);
    RegSetValueExA(key, "BOOKINGMBWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyMBWndInfo.x), 4);
    RegSetValueExA(key, "BOOKINGMBWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyMBWndInfo.y), 4);
    RegSetValueExA(key, "BOOKINGMBWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyMBWndInfo.width), 4);
    RegSetValueExA(key, "BOOKINGMBWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyMBWndInfo.height), 4);
    RegSetValueExA(key, "BOOKINGWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyWndInfo.x), 4);
    RegSetValueExA(key, "BOOKINGWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyWndInfo.y), 4);
    RegSetValueExA(key, "BOOKINGWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyWndInfo.width), 4);
    RegSetValueExA(key, "BOOKINGWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->seekPartyWndInfo.height), 4);
    RegSetValueExA(key, "BOOKINGMBCHECKWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->jobListWndInfo.x), 4);
    RegSetValueExA(key, "BOOKINGMBCHECKWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->jobListWndInfo.y), 4);
    RegSetValueExA(key, "BOOKINGMBCHECKWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->jobListWndInfo.width), 4);
    RegSetValueExA(key, "BOOKINGMBCHECKWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->jobListWndInfo.height), 4);
    RegSetValueExA(key, "ITEMSHOPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemShopWndInfo.x), 4);
    RegSetValueExA(key, "ITEMSHOPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemShopWndInfo.y), 4);
    RegSetValueExA(key, "ITEMSHOPWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemShopWndInfo.width), 4);
    RegSetValueExA(key, "ITEMSHOPWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemShopWndInfo.height), 4);
    RegSetValueExA(key, "ITEMSELLWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemSellWndInfo.x), 4);
    RegSetValueExA(key, "ITEMSELLWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemSellWndInfo.y), 4);
    RegSetValueExA(key, "ITEMSELLWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemSellWndInfo.width), 4);
    RegSetValueExA(key, "ITEMSELLWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemSellWndInfo.height), 4);
    RegSetValueExA(key, "ITEMPURCHASEWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemPurchaseWndInfo.x), 4);
    RegSetValueExA(key, "ITEMPURCHASEWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemPurchaseWndInfo.y), 4);
    RegSetValueExA(key, "ITEMPURCHASEWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemPurchaseWndInfo.width), 4);
    RegSetValueExA(key, "ITEMPURCHASEWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemPurchaseWndInfo.height), 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->messengerGroupWndInfo.x), 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->messengerGroupWndInfo.y), 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->messengerGroupWndInfo.width), 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->messengerGroupWndInfo.height), 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->messengerGroupWndInfo.visible), 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.RADIO", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->messengerGroupWndInfo.mode), 4);
    RegSetValueExA(key, "MESSENGERGROUPWNDINFO.SIZEMODE", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->messengerGroupWndInfo.iconIndex), 4);
    RegSetValueExA(key, "EXCHANGEACCEPTWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->exchangeAcceptWndInfo.x), 4);
    RegSetValueExA(key, "EXCHANGEACCEPTWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->exchangeAcceptWndInfo.y), 4);
    RegSetValueExA(key, "EXCHANGEACCEPTWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->exchangeAcceptWndInfo.width), 4);
    RegSetValueExA(key, "EXCHANGEACCEPTWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->exchangeAcceptWndInfo.height), 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->skillListWndInfo.x), 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->skillListWndInfo.y), 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->skillListWndInfo.width), 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->skillListWndInfo.height), 4);
    RegSetValueExA(key, "SKILLLISTWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->skillListWndVisible), 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemWndInfo.x), 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemWndInfo.y), 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemWndInfo.width), 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemWndInfo.height), 4);
    RegSetValueExA(key, "MERCHANTITEMWNDINFO.SHOW", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemWndVisible), 4);
    RegSetValueExA(key, "MERCHANTMIRRORITEMWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantMirrorItemWndInfo.x), 4);
    RegSetValueExA(key, "MERCHANTMIRRORITEMWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantMirrorItemWndInfo.y), 4);
    RegSetValueExA(key, "MERCHANTMIRRORITEMWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantMirrorItemWndInfo.width), 4);
    RegSetValueExA(key, "MERCHANTMIRRORITEMWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantMirrorItemWndInfo.height), 4);
    RegSetValueExA(key, "MERCHANTSHOPMAKEWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantShopMakeWndInfo.x), 4);
    RegSetValueExA(key, "MERCHANTSHOPMAKEWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantShopMakeWndInfo.y), 4);
    RegSetValueExA(key, "MERCHANTSHOPMAKEWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantShopMakeWndInfo.width), 4);
    RegSetValueExA(key, "MERCHANTSHOPMAKEWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantShopMakeWndInfo.height), 4);
    RegSetValueExA(key, "MERCHANTITEMSHOPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemShopWndInfo.x), 4);
    RegSetValueExA(key, "MERCHANTITEMSHOPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemShopWndInfo.y), 4);
    RegSetValueExA(key, "MERCHANTITEMSHOPWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemShopWndInfo.width), 4);
    RegSetValueExA(key, "MERCHANTITEMSHOPWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemShopWndInfo.height), 4);
    RegSetValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemMyShopWndInfo.x), 4);
    RegSetValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemMyShopWndInfo.y), 4);
    RegSetValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemMyShopWndInfo.width), 4);
    RegSetValueExA(key, "MERCHANTITEMMYSHOPWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemMyShopWndInfo.height), 4);
    RegSetValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemPurchaseWndInfo.x), 4);
    RegSetValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemPurchaseWndInfo.y), 4);
    RegSetValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemPurchaseWndInfo.width), 4);
    RegSetValueExA(key, "MERCHANTITEMPURCHASEWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merchantItemPurchaseWndInfo.height), 4);
    RegSetValueExA(key, "ITEMCOLLECTIONWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemCollectionWndInfo.x), 4);
    RegSetValueExA(key, "ITEMCOLLECTIONWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemCollectionWndInfo.y), 4);
    RegSetValueExA(key, "ITEMCOLLECTIONWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemCollectionWndInfo.width), 4);
    RegSetValueExA(key, "ITEMCOLLECTIONWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemCollectionWndInfo.height), 4);
    RegSetValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->combinedCardItemCollectionWndInfo.x), 4);
    RegSetValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->combinedCardItemCollectionWndInfo.y), 4);
    RegSetValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->combinedCardItemCollectionWndInfo.width), 4);
    RegSetValueExA(key, "COMBINEDCARDITEMCOLLECTIONWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->combinedCardItemCollectionWndInfo.height), 4);
    RegSetValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemParamChangeDisplayWndInfo.x), 4);
    RegSetValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemParamChangeDisplayWndInfo.y), 4);
    RegSetValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemParamChangeDisplayWndInfo.width), 4);
    RegSetValueExA(key, "ITEMPARAMCHANGEDISPLAYWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemParamChangeDisplayWndInfo.height), 4);
    RegSetValueExA(key, "PARTYSETTINGWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->partySettingWndInfo.x), 4);
    RegSetValueExA(key, "PARTYSETTINGWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->partySettingWndInfo.y), 4);
    RegSetValueExA(key, "PARTYSETTINGWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->partySettingWndInfo.width), 4);
    RegSetValueExA(key, "PARTYSETTINGWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->partySettingWndInfo.height), 4);
    RegSetValueExA(key, "DETAILLEVELWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unusedWndInfoA.x), 4);
    RegSetValueExA(key, "DETAILLEVELWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unusedWndInfoA.y), 4);
    RegSetValueExA(key, "DETAILLEVELWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unusedWndInfoA.width), 4);
    RegSetValueExA(key, "DETAILLEVELWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unusedWndInfoA.height), 4);
    RegSetValueExA(key, "WHISPERWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_whisperPlacement.x), 4);
    RegSetValueExA(key, "WHISPERWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_whisperPlacement.y), 4);
    RegSetValueExA(key, "WHISPERWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_whisperPlacement.width), 4);
    RegSetValueExA(key, "WHISPERWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_whisperPlacement.height), 4);
    RegSetValueExA(key, "FRIENDOPTIONWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->friendOptionWndInfo.x), 4);
    RegSetValueExA(key, "FRIENDOPTIONWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->friendOptionWndInfo.y), 4);
    RegSetValueExA(key, "FRIENDOPTIONWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->friendOptionWndInfo.width), 4);
    RegSetValueExA(key, "FRIENDOPTIONWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->friendOptionWndInfo.height), 4);
    RegSetValueExA(key, "ITEMCOMPOSITIONWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemCompositionWndInfo.x), 4);
    RegSetValueExA(key, "ITEMCOMPOSITIONWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemCompositionWndInfo.y), 4);
    RegSetValueExA(key, "ITEMCOMPOSITIONWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemCompositionWndInfo.width), 4);
    RegSetValueExA(key, "ITEMCOMPOSITIONWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemCompositionWndInfo.height), 4);
    RegSetValueExA(key, "ITEMIDENTIFYWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemIdentifyWndInfo.x), 4);
    RegSetValueExA(key, "ITEMIDENTIFYWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemIdentifyWndInfo.y), 4);
    RegSetValueExA(key, "ITEMIDENTIFYWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemIdentifyWndInfo.width), 4);
    RegSetValueExA(key, "ITEMIDENTIFYWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->itemIdentifyWndInfo.height), 4);
    RegSetValueExA(key, "SUBCHATWNDST.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->subChatHisWndInfo[0].x), 4);
    RegSetValueExA(key, "SUBCHATWNDST.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->subChatHisWndInfo[0].y), 4);
    RegSetValueExA(key, "SUBCHATWNDST.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->subChatHisWndInfo[0].width), 4);
    RegSetValueExA(key, "SUBCHATWNDST.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->subChatHisWndInfo[0].height), 4);
    RegSetValueExA(key, "SUBCHATWNDBT.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->subChatHisWndInfo[1].x), 4);
    RegSetValueExA(key, "SUBCHATWNDBT.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->subChatHisWndInfo[1].y), 4);
    RegSetValueExA(key, "SUBCHATWNDBT.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->subChatHisWndInfo[1].width), 4);
    RegSetValueExA(key, "SUBCHATWNDBT.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->subChatHisWndInfo[1].height), 4);
    RegSetValueExA(key, "CHATWNDSTICKON", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->chatWndStickOn), 4);
    RegSetValueExA(key, "ONSTSUBCHAT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->onStSubChat), 4);
    RegSetValueExA(key, "ONBTSUBCHAT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->battleSubChatEnabled), 4);
    RegSetValueExA(key, "MEMORIALDUNWND.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&g_session.memorialWindowX), 4);
    RegSetValueExA(key, "MEMORIALDUNWND.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&g_session.memorialWindowY), 4);
    RegSetValueExA(key, "QUICKSLOTWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->quickSlotWndInfo.x), 4);
    RegSetValueExA(key, "QUICKSLOTWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->quickSlotWndInfo.y), 4);
    RegSetValueExA(key, "QUICKSLOTWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->quickSlotWndInfo.width), 4);
    RegSetValueExA(key, "QUICKSLOTWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->quickSlotWndInfo.height), 4);
    RegSetValueExA(key, "BATTLEFIELDWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->entryQueueWndInfo.x), 4);
    RegSetValueExA(key, "BATTLEFIELDWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->entryQueueWndInfo.y), 4);
    RegSetValueExA(key, "BATTLEFIELDWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->entryQueueWndInfo.width), 4);
    RegSetValueExA(key, "BATTLEFIELDWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->entryQueueWndInfo.height), 4);
    RegSetValueExA(key, "GUILDHELPERWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->entryQueueStandbyWndInfo.x), 4);
    RegSetValueExA(key, "GUILDHELPERWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->entryQueueStandbyWndInfo.y), 4);
    RegSetValueExA(key, "GUILDHELPERWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->entryQueueStandbyWndInfo.width), 4);
    RegSetValueExA(key, "GUILDHELPERWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->entryQueueStandbyWndInfo.height), 4);
    RegSetValueExA(key, "BUYINGSTOREMAKEWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingShopMakeWndInfo.x), 4);
    RegSetValueExA(key, "BUYINGSTOREMAKEWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingShopMakeWndInfo.y), 4);
    RegSetValueExA(key, "BUYINGSTOREMAKEWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingShopMakeWndInfo.width), 4);
    RegSetValueExA(key, "BUYINGSTOREMAKEWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingShopMakeWndInfo.height), 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingMirrorItemWndInfo.x), 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingMirrorItemWndInfo.y), 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingMirrorItemWndInfo.width), 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORITEMWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingMirrorItemWndInfo.height), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemMyShopWndInfo.x), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemMyShopWndInfo.y), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemMyShopWndInfo.width), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMMYSHOPWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemMyShopWndInfo.height), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemShopWndInfo.x), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemShopWndInfo.y), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemShopWndInfo.width), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSHOPWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemShopWndInfo.height), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemPurchaseWndInfo.x), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemPurchaseWndInfo.y), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemPurchaseWndInfo.width), 4);
    RegSetValueExA(key, "BUYINGSTOREITEMSELLWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingItemPurchaseWndInfo.height), 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingMirrorItemWndInfo2.x), 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingMirrorItemWndInfo2.y), 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingMirrorItemWndInfo2.width), 4);
    RegSetValueExA(key, "BUYINGSTOREMIRRORSELLLISTWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->buyingMirrorItemWndInfo2.height), 4);
    RegSetValueExA(key, "REPLAYRECCONTROLNWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->replayRecControlWndInfo.x), 4);
    RegSetValueExA(key, "REPLAYRECCONTROLNWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->replayRecControlWndInfo.y), 4);
    RegSetValueExA(key, "REPLAYRECCONTROLNWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->replayRecControlWndInfo.width), 4);
    RegSetValueExA(key, "REPLAYRECCONTROLNWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->replayRecControlWndInfo.height), 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->navigationWindowInfo.x), 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->navigationWindowInfo.y), 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->navigationWindowInfo.width), 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->navigationWindowInfo.height), 4);
    RegSetValueExA(key, "NAVIGATIONWNDINFO.ISSCREEN", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->navigationWindowInfo.expanded), 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->miniMapWndInfo.width), 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->miniMapWndInfo.height), 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.SHOWQUEST", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->miniMapWndInfo.showQuests), 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.WHOWNPC", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->miniMapWndInfo.showTowns), 4);
    RegSetValueExA(key, "MINIMAPWNDINFO.WHOWMEMBER", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->miniMapWndInfo.showMembers), 4);
    RegSetValueExA(key, "ACHTRACINGWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->achTracingWndInfo.x), 4);
    RegSetValueExA(key, "ACHTRACINGWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->achTracingWndInfo.y), 4);
    RegSetValueExA(key, "PREVIEWEQUIPWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->previewEquipWndInfo.x), 4);
    RegSetValueExA(key, "PREVIEWEQUIPWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->previewEquipWndInfo.y), 4);
    RegSetValueExA(key, "SWAPEQUIPMENTWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->swapEquipmentPlacement[0]), 4);
    RegSetValueExA(key, "SWAPEQUIPMENTWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->swapEquipmentPlacement[1]), 4);
    RegSetValueExA(key, "PETINFOWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->petInfoWndInfo.x), 4);
    RegSetValueExA(key, "PETINFOWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->petInfoWndInfo.y), 4);
    RegSetValueExA(key, "PETINFOWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->petInfoWndInfo.width), 4);
    RegSetValueExA(key, "PETINFOWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->petInfoWndInfo.height), 4);
    RegSetValueExA(key, "HOMUNINFOWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->homunInfoWndInfo.x), 4);
    RegSetValueExA(key, "HOMUNINFOWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->homunInfoWndInfo.y), 4);
    RegSetValueExA(key, "HOMUNINFOWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->homunInfoWndInfo.width), 4);
    RegSetValueExA(key, "HOMUNINFOWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->homunInfoWndInfo.height), 4);
    RegSetValueExA(key, "MERINFOWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merInfoWndInfo.x), 4);
    RegSetValueExA(key, "MERINFOWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merInfoWndInfo.y), 4);
    RegSetValueExA(key, "MERINFOWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merInfoWndInfo.width), 4);
    RegSetValueExA(key, "MERINFOWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->merInfoWndInfo.height), 4);
    RegSetValueExA(key, "ADVEN_REGISTERWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->registerPartyWndInfo.x), 4);
    RegSetValueExA(key, "ADVEN_REGISTERWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->registerPartyWndInfo.y), 4);
    RegSetValueExA(key, "ADVEN_REGISTERWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->registerPartyWndInfo.width), 4);
    RegSetValueExA(key, "ADVEN_REGISTERWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->registerPartyWndInfo.height), 4);
    RegSetValueExA(key, "ADVEN_JOINWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->applyForPartyWndInfo.x), 4);
    RegSetValueExA(key, "ADVEN_JOINWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->applyForPartyWndInfo.y), 4);
    RegSetValueExA(key, "ADVEN_JOINWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->applyForPartyWndInfo.width), 4);
    RegSetValueExA(key, "ADVEN_JOINWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->applyForPartyWndInfo.height), 4);
    RegSetValueExA(key, "ADVEN_PARTYBOARDWNDINFO.X", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->advenPartyBoardWndInfo.x), 4);
    RegSetValueExA(key, "ADVEN_PARTYBOARDWNDINFO.Y", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->advenPartyBoardWndInfo.y), 4);
    RegSetValueExA(key, "ADVEN_PARTYBOARDWNDINFO.W", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->advenPartyBoardWndInfo.width), 4);
    RegSetValueExA(key, "ADVEN_PARTYBOARDWNDINFO.H", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->advenPartyBoardWndInfo.height), 4);
    RegCloseKey(key);
  }
}

// OFFSET: 0x00AA9F70
// FUNCTION: fn_17264
void UIWindowMgr::RefreshWindow01C8Edits() {
  if (this->chatWnd) {
    if (this->hoverWindow == static_cast<UINewChatWnd*>(this->chatWnd)->m_chatEdit) {
      static_cast<UINewChatWnd*>(this->chatWnd)->m_chatEdit->SetText("");
    }

    UIEditCtrl* edit = static_cast<UIEditCtrl*>(static_cast<UINewChatWnd*>(this->chatWnd)->m_whisperEdit);
    if (this->hoverWindow == edit) {
      edit->SetText(edit->Text());
    }
  }
}

// OFFSET: 0x00AA9FD0
// FUNCTION: fn_17265
void UIWindowMgr::SetBackgroundSurface(CBitmapRes* bitmap) {
  if (!bitmap) {
    if (this->backgroundSurface) {
      delete this->backgroundSurface;
      this->backgroundSurface = nullptr;
    }
    return;
  }

  if (!this->backgroundSurface) {
    this->backgroundSurface = GetRenderSystemHolder()->CreateSurfaceOrTexture(bitmap->image.width, bitmap->image.height);
    if (!this->backgroundSurface) {
      return;
    }
  }
  this->backgroundSurface->Upload(0, 0, bitmap->image.width, bitmap->image.height, bitmap->image.pixels, 0, 0);
}

// OFFSET: 0x00AAA050
// FUNCTION: fn_17266
void UIWindowMgr::SendFlagEffect(int hotkey) {
  CMode* mode = g_modeMgr.gameMode;
  if (!mode) {
    return;
  }

  switch (hotkey) {
  case HK_FLAG_1 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_FLAG"), 0, 0, 0);
    break;
  case HK_FLAG_2 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_INDONESIA_FLAG"), 0, 0, 0);
    break;
  case HK_FLAG_3 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_PH_FLAG"), 0, 0, 0);
    break;
  case HK_FLAG_4 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_MY_FLAG"), 0, 0, 0);
    break;
  case HK_FLAG_5 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_SI_FLAG"), 0, 0, 0);
    break;
  case HK_FLAG_6 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_BR_FLAG"), 0, 0, 0);
    break;
  case HK_FLAG_7 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_INDIA_FLAG"), 0, 0, 0);
    break;
  case HK_FLAG_8 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_FLAG8"), 0, 0, 0);
    break;
  case HK_FLAG_9 + 200:
    mode->SendMsg(MM_CASH_EMOTION, reinterpret_cast<int>("ET_FLAG9"), 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00A8FCA0
// FUNCTION: fn_17149
bool UIWindowMgr::DropAt(int x, int y, ModeDragState* drag) {
  UIWindow* window = this->HitTestWindows(x, y);
  if (window) {
    int offsetX = 0;
    int offsetY = 0;
    for (UIWindow* node = window; node; node = node->parent) {
      offsetX += node->geometry.x;
      offsetY += node->geometry.y;
    }

    x -= offsetX;
    y -= offsetY;
    window->RecordDrop(x, y, *drag, -1);
    drag->sourceWindow = reinterpret_cast<std::uint32_t>(window);
    if (this->itemDropCntWnd != window->Root()) {
      window->DragAndDrop(x, y, reinterpret_cast<std::intptr_t>(drag));
    }

    return true;
  }

  return false;
}

// OFFSET: 0x00A8FD30
// FUNCTION: fn_17150
void UIWindowMgr::FocusWindow01C8Edit() {
  if (!this->chatWnd) {
    return;
  }

  UIWindow* edit = static_cast<UINewChatWnd*>(this->chatWnd)->m_chatEdit;
  if (this->hoverWindow) {
    this->hoverWindow->OnFinishEdit(!edit ? false : edit->GetEditWindow() == this->hoverWindow);
  }

  if (edit) {
    this->hoverWindow = edit->GetEditWindow();
    if (this->hoverWindow) {
      this->hoverWindow->OnBeginEdit();
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
  for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    if (this->hiddenWindows.end() != std::find(this->hiddenWindows.begin(), this->hiddenWindows.end(), *it)) {
      (*it)->Show(1);
    }
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode && mode->canRotateView == 1) {
    this->EnsureWindow(WID_MINIMAPZOOMWND);
  }

  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->RefreshEffect164();
  this->windowVisibilityState = 1;

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

// OFFSET: 0x00A8FDB0
// FUNCTION: fn_17151
int UIWindowMgr::RunEmailAddress() {
  if (!this->modalWindows.empty() && this->modalWindows.back()) {
    return 185;
  }

  UIWindow* window = new UIEmailAddressWnd();
  window->SetSize(280, 48);
  window->Move((g_renderer->viewport.width - 640) / 2 + 185, (g_renderer->viewport.height - 480) / 2 + 180);
  this->rendered.push_back(window);

  if (this->inputWindow) {
    this->persistentWindows.insert(this->inputWindow->Root());
  }

  this->inputWindow = nullptr;
  this->modalResponse = 0;
  this->modalWindows.push_back(window);
  int response;
  CMode* pump = g_modeMgr.gameMode;

  for (;;) {
    if (g_modalImmediate != 0) {
      response = 184;
      break;
    }

    if (this->activeWindowId != 0) {
      if (UIWindow* active = this->Top()) {
        active->Show(0);
        this->Schedule(active);
        this->modalWindows.pop_back();
      }

      response = this->activeWindowId;

      if (this->modalWindows.empty()) {
        this->activeWindowId = 0;
      }

      break;
    }

    if (this->modalResponse != 0) {
      if (UIWindow* active = this->Top()) {
        active->Show(0);
        this->Schedule(active);
        this->modalWindows.pop_back();
      }

      response = this->modalResponse;
      this->modalResponse = 0;
      break;
    }

    if (pump) {
      pump->Update();
      pump = g_modeMgr.gameMode;
    }

    if (g_session.GetOption(20) > 0) {
      this->activeWindowId = 0;
      this->modalResponse = 0;

      while (!this->modalWindows.empty()) {
        this->Schedule(this->modalWindows.back());
        this->modalWindows.pop_back();
      }

      response = 184;
      break;
    }
  }

  this->RestoreInput();
  return response;
}
