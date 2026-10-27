#include "UI/Window/FrameFocus.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Layout/WindowSnapping.h"

#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x008A3700
// FUNCTION: fn_11657
UIFrameWnd::UIFrameWnd(UIWindowContext& windowContext, UIFrameContext& frameContext, unsigned char cached)
    : UIWindow(windowContext, cached), m_dragX(0), m_dragY(0), m_message0(600), m_message1(600), m_frameContext(&frameContext) {
  this->m_textCache.max_load_factor(1.0f);
  this->m_textCache.rehash(8);
}

// OFFSET: 0x008A4E90
// OFFSET: 0x008A60E0
// FUNCTION: fn_11678
UIFrameWnd::~UIFrameWnd() = default;

// OFFSET: 0x005C4640
// FUNCTION: fn_03570
bool UIFrameWnd::Slot02() {
  return true;
}

// OFFSET: 0x008AD150
// FUNCTION: fn_11735
void UIFrameWnd::Move(int x, int y) {
  this->geometry.Move(x, y);
  this->Slot36();
}

// OFFSET: 0x008BB3F0
// FUNCTION: fn_11797
void UIFrameWnd::Slot12() {
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
}

// OFFSET: 0x008AD170
// FUNCTION: fn_11736
void UIFrameWnd::Slot13(int dx, int dy) {
  this->geometry.x = this->m_originX + dx;
  this->geometry.y = this->m_originY + dy;
}

// OFFSET: 0x00A7D870
// FUNCTION: fn_16956
void UIFrameWnd::ToScreen(int& x, int& y) const {
  x += this->geometry.x;
  y += this->geometry.y;

  for (const WindowAncestry* node = this->parent; node; node = node->parent) {
    const UIWindow* window = static_cast<const UIWindow*>(node);
    x += window->geometry.x;
    y += window->geometry.y;
  }
}

// OFFSET: 0x008BA1E0
// FUNCTION: fn_11779
void UIFrameWnd::Slot25(int x, int y) {
  if (g_dragWindowId != this->geometry.id) {
    this->m_frameContext->focus.BringToFront(this->geometry.id);
    g_dragWindowId = this->geometry.id;
  }

  this->m_frameContext->focus.Set(this);
  this->ToScreen(x, y);
  this->m_dragX = x;
  this->m_dragY = y;
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
  this->geometry.targetAlpha = 128;
  this->geometry.alphaStart = timeGetTime();
  this->m_frameContext->snapping.graph.CaptureOrigins(this);
}

// OFFSET: 0x008BA510
// FUNCTION: fn_11786
void UIFrameWnd::Slot27(int x, int y) {
  if (this->m_frameContext->focus.Get() != this || x <= -this->m_frameContext->dragLimitX || y <= -this->m_frameContext->dragLimitY) {
    return;
  }

  this->ToScreen(x, y);
  const int dx = x - this->m_dragX;
  const int dy = y - this->m_dragY;
  WindowSnapping& snapping = this->m_frameContext->snapping;

  if (!snapping.graph.Contains(this)) {
    this->Move(this->m_originX + dx, this->m_originY + dy);
    return;
  }

  this->Slot13(dx, dy);
  int ownX, ownY;
  snapping.Constrain(this, ownX, ownY, 0);
  int groupX = dx, groupY = dy;
  snapping.graph.ConstrainGroup(this, groupX, groupY, snapping);

  if (std::abs(groupX) < std::abs(ownX)) {
    groupX = ownX;
  }

  if (std::abs(groupY) < std::abs(ownY)) {
    groupY = ownY;
  }

  this->Slot13(dx + groupX, dy + groupY);
  snapping.Store(this);
  snapping.graph.MoveConnected(this, dx + groupX, dy + groupY, snapping);
}

// OFFSET: 0x008BA3C0
// FUNCTION: fn_11783
void UIFrameWnd::Slot28(int, int) {
}

// OFFSET: 0x008BA2C0
// FUNCTION: fn_11781
void UIFrameWnd::Slot31(int, int) {
  if (this->m_frameContext->focus.Get() != this) {
    return;
  }

  g_dragWindowId = -1;
  this->geometry.targetAlpha = 255;
  this->geometry.alphaStart = timeGetTime();
  this->m_frameContext->focus.Clear();
  this->m_frameContext->snapping.graph.Refresh(this);
}

// OFFSET: 0x008BB340
// FUNCTION: fn_11796
void UIFrameWnd::Slot36() {
  WindowSnapping& snapping = this->m_frameContext->snapping;
  int x, y;
  snapping.Constrain(this, x, y, 0);
  this->geometry.x += x;
  this->geometry.y += y;
  snapping.Store(this);
}

// OFFSET: 0x008BDC90
// FUNCTION: fn_11802
int UIFrameWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 0 || message == 1) {
    const int id = message == 0 ? this->m_message0 : this->m_message1;
    if (id) {
      this->SendMsg(this, 6, id, 0, 0, 0);
    }
  } else if (message == 98) {
    if (c > 0) {
      UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
      if (window) {
        window->SendMsg(nullptr, 24, c, 0, 0, 0);
      }
    }
  } else {
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x005C4420
// FUNCTION: fn_03559
std::vector<std::uint32_t> UIFrameWnd::Slot44() {
  return {};
}

// OFFSET: 0x005C44F0
// FUNCTION: fn_03562
int UIFrameWnd::Slot46() {
  return 1;
}

// OFFSET: 0x008CF0C0
// FUNCTION: fn_12076
UIBitmapButton* UIFrameWnd::AddCloseButton(UIButtonContext& controls, const char* tooltip, int width) {
  if (width == 0) {
    width = this->geometry.width;
  }

  UIBitmapButton* button = new UIBitmapButton(*this->m_context, controls);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - 14, 3);
  button->SetId(201);
  button->SetTooltip(tooltip);
  this->AddChild(button);

  return button;
}
