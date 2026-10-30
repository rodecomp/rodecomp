#include "UI/Guild/UIGuildHelperWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <mbstring.h>
#include <string>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24878
static const std::string s_itemLinkOpening = "<ITEML>";

// GLOBAL: gl_24879
static const std::string s_itemLinkClosing = "</ITEML>";

// OFFSET: 0x0095DB20
// FUNCTION: fn_13071
void UIGuildHelperWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2074), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(controls.fonts, controls.locale, 15, 32, LookupMessage(2075), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0095FCB0
// FUNCTION: fn_13084
void UIGuildHelperWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
    if (UIWindow* window = g_windowMgr.unknown_0438) {
      window->geometry.targetAlpha = 128;
      window->geometry.alphaStart = timeGetTime();
    }
  }
}

// OFFSET: 0x00960130
// FUNCTION: fn_13095
void UIGuildHelperWnd::Slot31(int x, int y) {
  UIFrameWnd::Slot31(x, y);
  if (UIWindow* window = g_windowMgr.unknown_0438) {
    window->geometry.targetAlpha = 255;
    window->geometry.alphaStart = timeGetTime();
  }
}

// OFFSET: 0x009601D0
// FUNCTION: fn_13098
void UIGuildHelperWnd::Slot27(int x, int y) {
  UIFrameWnd::Slot27(x, y);
  if (g_windowMgr.unknown_0438) {
    x = 0;
    y = 0;
    this->ToScreen(x, y);
    g_windowMgr.unknown_0438->Move(x + 2 + this->geometry.width, y);
  }
}

// OFFSET: 0x0095D5C0
// FUNCTION: fn_13069
void UICreateGuildWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
  int text;
  int title;
  if (this->m_mode == 0) {
    text = 2077;
    title = 2076;
  } else {
    text = 2089;
    title = 2088;
  }
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(title), 0);
  this->DrawTextAt(controls.fonts, controls.locale, 16, 23, LookupMessage(text), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0095FBE0
// FUNCTION: fn_13082
void UICreateGuildWnd::Slot25(int, int) {
}

// OFFSET: 0x009601C0
// FUNCTION: fn_13097
void UICreateGuildWnd::Slot27(int, int) {
}

// OFFSET: 0x00960120
// FUNCTION: fn_13094
void UICreateGuildWnd::Slot31(int, int) {
}

// OFFSET: 0x0095F420
// FUNCTION: fn_13078
void UIGuildTipWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2078), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x0095E310
// FUNCTION: fn_13073
void UIGuildLeaveReasonDescWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x00965650
// FUNCTION: fn_13138
void UICreateGuildWnd::Slot11() {
}

// OFFSET: 0x00965660
// FUNCTION: fn_13139
void UIGuildHelperWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}

// OFFSET: 0x009612E0
// FUNCTION: fn_13123
int UICreateGuildWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }

  switch (a1) {
  case 184: {
    CMode* mode = g_modeMgr.GetGameMode();
    std::string name = this->m_nameEdit->Text();
    this->m_nameEdit->SelectAll();

    if (name.empty()) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2080)), 0xFF, 0, 0, 0);
    } else if (_mbsstr(reinterpret_cast<const unsigned char*>(this->m_nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_itemLinkOpening.c_str())) ||
               _mbsstr(reinterpret_cast<const unsigned char*>(this->m_nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_itemLinkClosing.c_str()))) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2812)), 0xFF, 0, 0, 0);
    } else if (this->m_mode == 0) {
      mode->SendMsg(112, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
    } else {
      mode->SendMsg(106, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
      this->m_frameContext->windows.CloseWindow(215);
    }
    break;
  }
  case 185:
    if (this->m_mode == 0) {
      this->m_frameContext->windows.CloseWindow(213);
    } else {
      this->m_frameContext->windows.CloseWindow(215);
    }
    break;
  }

  return 0;
}

// OFFSET: 0x00961540
// FUNCTION: fn_13124
int UIGuildBanReasonDescWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message == 6) {
    if (a1 == 184) {
      this->m_frameContext->windows.CloseWindow(76);
      g_modeMgr.gameMode->SendMsg(111, reinterpret_cast<int>(&this->m_target), reinterpret_cast<int>(this->m_reasonEdit->Text()), 0, 0);
    } else if (a1 == 185) {
      this->m_frameContext->windows.CloseWindow(76);
    }
    return 0;
  }

  if (message == 34) {
    this->m_target = *reinterpret_cast<const GuildBanTarget*>(a1);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
}

// OFFSET: 0x00961710
// FUNCTION: fn_13126
int UIGuildHelperWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message == 6) {
    switch (a1) {
    case 184:
      this->m_frameContext->windows.CloseWindow(214);
      this->m_frameContext->windows.CloseWindow(213);
      this->m_frameContext->windows.CloseWindow(212);
      break;
    case 343:
      if (!this->m_frameContext->windows.CloseWindow(214)) {
        this->m_frameContext->windows.EnsureWindow(214);
      }
      break;
    case 448:
      if (!this->m_frameContext->windows.CloseWindow(213)) {
        this->m_frameContext->windows.EnsureWindow(213);
        if (g_windowMgr.unknown_0438) {
          int x = 0;
          int y = 0;
          this->ToScreen(x, y);
          g_windowMgr.unknown_0438->Move(x + 2 + this->geometry.width, y);
        }
      }
      break;
    }
    return 0;
  }

  if (message == 34) {
    this->m_saved = reinterpret_cast<GuildHelperWndInfo*>(a1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    this->Move(this->m_saved->x, this->m_saved->y);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
}

// OFFSET: 0x00964C60
// FUNCTION: fn_13133
int UIGuildTipWnd::SendMsg(UIWindow*, int message, std::intptr_t a1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6 && a1 == 201) {
    this->m_frameContext->windows.CloseWindow(214);
  }

  return 0;
}
