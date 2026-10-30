#include "UI/Window/UIWindowMgr.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIReplayButton.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UICntWnd.h"
#include "UI/Window/UIMonsterInfoWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeEvents.h"
#include "UI/Lists/UIReplayListBox.h"
#include "UI/Window/UINoticeWnd.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Layout/WindowSnapping.h"

#include <cstdlib>
#include <string>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24747
std::uint32_t g_monsterInfoActorId;

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

  for (const UIWindow* node = this->parent; node; node = node->parent) {
    x += node->geometry.x;
    y += node->geometry.y;
  }
}

// OFFSET: 0x008BA1E0
// FUNCTION: fn_11779
void UIFrameWnd::Slot25(int x, int y) {
  if (g_dragWindowId != this->geometry.id) {
    g_windowMgr.BringToFront(this->geometry.id);
    g_dragWindowId = this->geometry.id;
  }

  g_windowMgr.SetCapture(this);
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
  if (g_windowMgr.Capture() != this || x <= -this->m_frameContext->dragLimitX || y <= -this->m_frameContext->dragLimitY) {
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
  if (g_windowMgr.Capture() != this) {
    return;
  }

  g_dragWindowId = -1;
  this->geometry.targetAlpha = 255;
  this->geometry.alphaStart = timeGetTime();
  g_windowMgr.ReleaseCapture();
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

// OFFSET: 0x008CEF10

// OFFSET: 0x008CEFD0

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

// OFFSET: 0x008CF2A0

// OFFSET: 0x008CF390

// OFFSET: 0x008CF870

// OFFSET: 0x008CFE50

// OFFSET: 0x008CFF40

// OFFSET: 0x008BA280
// FUNCTION: fn_11780
void UIReplayListBox::Slot25(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
  this->parent->SendMsg(nullptr, 80, 0, 0, 0, 0);
}

// OFFSET: 0x008BA500
// FUNCTION: fn_11785
void UINoticeWnd::Slot28(int, int) {
}

// OFFSET: 0x008BA310
// FUNCTION: fn_11782
void UIReplayButton::Slot31(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;
    if (this->parent) {
      this->parent->SendMsg(this, 6, this->geometry.id, this->m_argument, 0, 0);
      this->parent->SendMsg(nullptr, 35, 0, 0, 0, 0);
    } else {
      this->m_context->mode->SendModeMessage(0, this->geometry.id, this->m_argument, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x008BA3D0
// FUNCTION: fn_11784
void UIMonsterInfoWnd::Slot28(int x, int y) {
  if (x < 161 || x > 259 || y < 31 || y > 41) {
    return;
  }

  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  std::string name = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->nameCard.Name(g_monsterInfoActorId);
  const std::size_t mark = name.find("#", 0);
  if (mark != std::string::npos) {
    name.erase(mark, name.size());
  }

  if (this->TextWidth(this->m_controls->fonts, this->m_controls->locale, name.c_str(), 0, 0, 12, 0, 0) > 100) {
    this->m_controls->services.Tooltip(name.c_str(), screenX + x, screenY + y, -1, false, true);
  }
}

// OFFSET: 0x008BB410
// FUNCTION: fn_11798
int UIAllyGuildAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 184) {
      this->m_frameContext->windows.CloseWindow(72);
      this->m_context->mode->SendModeMessage(120, this->identity, 1, 0, 0);
    } else if (a == 185) {
      this->m_frameContext->windows.CloseWindow(72);
      this->m_context->mode->SendModeMessage(120, this->identity, 0, 0, 0);
    }

    return 0;
  }

  if (message != 74) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  this->identity = static_cast<std::uint32_t>(a);
  std::string text = "(";
  text += reinterpret_cast<const char*>(b);
  text += ")";
  text += this->m_acceptance->text.buttons.services.MessageText(393);
  this->viewer->Clear();
  this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
  return 0;
}

// OFFSET: 0x008BDB00
// FUNCTION: fn_11801
int UICntWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    if (a == 184) {
      this->m_context->mode->SendModeMessage(this->m_event, std::atoi(this->m_edit->Text()), 0, 0, 0);
    }

    g_windowMgr.Schedule(this);
    return 0;
  case 29:
    this->m_text->SetText(reinterpret_cast<const char*>(a), 0);
    return 0;
  case 80:
    this->m_event = static_cast<int>(a);
    return 0;
  case 81: {
    char buffer[16];
    this->m_edit->SetText(_itoa(static_cast<int>(a), buffer, 10));
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}
