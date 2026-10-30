#include "UI/Guild/UIGuildHelperWnd.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

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
