#include "UI/Window/UIBasicInfoWnd.h"

#include "Client/Session.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>

// OFFSET: 0x00997250
// FUNCTION: fn_13484
UIBasicInfoWnd::UIBasicInfoWnd(UIWindowContext& windowContext, UIFrameContext& frameContext) : UIFrameWnd(windowContext, frameContext, 0) {
  this->saved = nullptr;
  this->mode = 0;
  this->layoutOffset = 13;
  this->collapsedHeight = 55;
  this->expandedHeight = 134;
  this->extendedOffset = 28;
  this->extendedCollapsedHeight = 71;
  this->extendedExpandedHeight = 149;
  this->normalOffset = 13;
  this->normalCollapsedHeight = 55;
  this->normalExpandedHeight = 134;
  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true) {
    this->layoutOffset = this->extendedOffset;
    this->collapsedHeight = this->extendedCollapsedHeight;
    this->expandedHeight = this->extendedExpandedHeight;
  }
}

// OFFSET: 0x00998190
// FUNCTION: fn_13489
bool UIBasicInfoWnd::Slot03() {
  return true;
}

// OFFSET: 0x009981A0
// FUNCTION: fn_13490
void UIBasicInfoWnd::SetHeight(int height) {
  if (height == this->collapsedHeight) {
    this->Resize(this->geometry.width, this->collapsedHeight);
    this->nameText->Resize(this->geometry.width - 30, 15);
    this->nameText->SetTextFitted(g_session.DecodePlayerName());
    this->nameText->Move(17, 3);
    this->hpBar->Move(-100, -100);
    this->spBar->Move(-100, -100);
    this->apBar->Move(-100, -100);
    this->baseExpBar->Move(-100, -100);
    this->jobExpBar->Move(-100, -100);
  } else if (height == this->expandedHeight) {
    this->Resize(this->geometry.width, this->expandedHeight);
    this->hpBar->Move(35, 53);
    this->spBar->Move(35, 68);
    if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true) {
      this->apBar->Move(35, 83);
    }
    this->nameText->Resize(this->geometry.width - 20, 15);
    this->nameText->SetTextFitted(g_session.DecodePlayerName());
    this->nameText->Move(9, 22);

    if (g_session.GetMaxBaseLevel(g_session.GetDisplayJob()) == g_session.m_level) {
      this->baseExpBar->Move(-200, 78);
    } else {
      this->baseExpBar->Move(85, this->layoutOffset + 76);
    }

    if (g_session.m_jobLevel < g_session.GetMaxJobLevel(g_session.GetDisplayJob())) {
      this->jobExpBar->Move(85, this->layoutOffset + 88);
    } else {
      this->jobExpBar->Move(-200, 88);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);
    window->Move(screenX, this->geometry.height + screenY);
  }
}

// OFFSET: 0x00999490
// FUNCTION: fn_13497
void UIBasicInfoWnd::RefreshLayout() {
  const int height = this->geometry.height;
  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob())) {
    if (height == this->collapsedHeight) {
      this->layoutOffset = this->extendedOffset;
      this->collapsedHeight = this->extendedCollapsedHeight;
      this->expandedHeight = this->extendedExpandedHeight;
      this->SetHeight(this->extendedCollapsedHeight);
    } else if (height == this->expandedHeight) {
      this->layoutOffset = this->extendedOffset;
      this->collapsedHeight = this->extendedCollapsedHeight;
      this->expandedHeight = this->extendedExpandedHeight;
      this->SetHeight(this->extendedExpandedHeight);
    }
  } else {
    if (height == this->collapsedHeight) {
      this->layoutOffset = this->normalOffset;
      this->collapsedHeight = this->normalCollapsedHeight;
      this->expandedHeight = this->normalExpandedHeight;
      this->SetHeight(this->normalCollapsedHeight);
    } else if (height == this->expandedHeight) {
      this->layoutOffset = this->normalOffset;
      this->collapsedHeight = this->normalCollapsedHeight;
      this->expandedHeight = this->normalExpandedHeight;
      this->SetHeight(this->normalExpandedHeight);
    }
    this->apBar->Move(-100, -100);
  }

  this->Invalidate();
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
