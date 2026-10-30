#include "UI/Window/UIBasicInfoWnd.h"

#include "Client/Session.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>

// OFFSET: 0x00998190
// FUNCTION: fn_13489
bool UIBasicInfoWnd::Slot03() {
  return true;
}

// OFFSET: 0x00999BC0
// FUNCTION: fn_13499
void UIBasicInfoWnd::Slot11() {
  if (this->saved) {
    this->saved->height = this->geometry.height;
    this->saved->width = this->geometry.width;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->unknown_0010 = this->mode;
  }
}

// OFFSET: 0x00999360
// FUNCTION: fn_13494
void UIBasicInfoWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x00999330
// FUNCTION: fn_13493
void UIBasicInfoWnd::Slot26(int x, int y) {
  if (y < 17) {
    this->SendMsg(this, 6, 202, 0, 0, 0);
  }
}

// OFFSET: 0x00999430
// FUNCTION: fn_13496
void UIBasicInfoWnd::Slot27(int x, int y) {
  UIFrameWnd::Slot27(x, y);
  if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);
    window->Move(screenX, this->geometry.height + screenY);
  }
}

// OFFSET: 0x00999380
// FUNCTION: fn_13495
void UIBasicInfoWnd::Slot28(int x, int y) {
  if (this->geometry.height != this->expandedHeight) {
    return;
  }

  int top = this->geometry.height - 20;
  if (x < 0 || x >= 100 || y < top || y >= this->geometry.height) {
    return;
  }

  char text[64];
  std::sprintf(text, "Weight %d%%", g_session.m_maxWeight ? g_session.m_weight * 100 / g_session.m_maxWeight : 0);

  int screenX = 0;
  int screenY = top - 19;
  this->ToScreen(screenX, screenY);
  this->m_controls->services.Tooltip(text, screenX, screenY, -1, false, true);
}
