#include "UI/Window/UIWindowMgr.h"
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
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/Surface.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Edit/UIEditCtrl.h"
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
