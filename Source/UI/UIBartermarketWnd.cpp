#include "UI/UIBartermarketWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00840B30
// FUNCTION: fn_10545
void UIBarterItemPurchaseWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 31) {
    if (g_session.IsEquipItemType(drag->type) == true || g_session.IsShadowItemType(drag->type) == true) {
      this->SendMsg(nullptr, 38, 0, 0, 0, 0);
      return;
    }

    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    if (UIWindow* window = g_windowMgr.LookupWindow(15)) {
      window->SendMsg(nullptr, 13, 24, 0, 0, 0);
    }
  } else {
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00840C00
// FUNCTION: fn_10546
void UIBartermarketWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 4) {
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(18, 0, 0, 0, 0);
    }
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  if (UIWindow* window = g_windowMgr.LookupWindow(15)) {
    window->SendMsg(nullptr, 13, 23, 0, 0, 0);
  }
}

// OFFSET: 0x00845940
// FUNCTION: fn_10576
void UIExpandedBarterItemPurchaseWnd::Slot10(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 31) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }
  if (g_session.IsEquipItemType(drag->type) == true || g_session.IsShadowItemType(drag->type) == true) {
    this->SendMsg(0, 38, 0, 0, 0, 0);
    return;
  }
  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  g_windowMgr.LookupWindow(15)->SendMsg(0, 13, 26, 0, 0, 0);
}

// OFFSET: 0x008459F0
// FUNCTION: fn_10577
void UIExpandedBartermarketWnd::Slot10(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 32) {
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }
  if (drag->quantity == 1) {
    this->SendMsg(0, 38, 0, 0, 0, 0);
    return;
  }
  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  g_windowMgr.LookupWindow(15)->SendMsg(0, 13, 25, 0, 0, 0);
}

// OFFSET: 0x00849030
// FUNCTION: fn_10594
void UIExpandedBarterItemPurchaseWnd::Slot35(int delta) {
  if (this->scrollEnabled == 1) {
    int step = this->rowCount - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, 7, delta > 0 ? -step : step, 0, 0, 0);
  }
}

// OFFSET: 0x00849080
// FUNCTION: fn_10595
void UIExpandedBartermarketWnd::Slot35(int delta) {
  if (this->scrollEnabled == 1) {
    int step = this->rowCount - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, 7, delta > 0 ? -step : step, 0, 0, 0);
  }
}

// OFFSET: 0x008490D0
// FUNCTION: fn_10596
void UIExpandedBarterItemPurchaseWnd::UpdateScrollBar() {
  if (!this->scrollBar) {
    return;
  }

  const int rows = this->rowCount;
  const int count = this->itemCount;
  const int overflow = std::max(count - rows, 0);

  if (overflow != 0) {
    this->scrollEnabled = 1;
    this->scrollBar->SetRange(overflow, rows, count);

    int position = this->unknown_00C4;
    if (position < 0) {
      this->unknown_00C4 = 0;
      position = 0;
    }

    const int limit = count - this->rowCount;
    if (limit < position) {
      this->unknown_00C4 = limit;
      position = limit;
    }

    this->scrollBar->SetPosition(position);
  } else {
    this->scrollEnabled = 0;
    this->unknown_00C4 = 0;
    this->scrollBar->SetRange(0, rows, count);
  }

  this->scrollBar->Invalidate();
}

// OFFSET: 0x00849170
// FUNCTION: fn_10597
void UIExpandedBartermarketWnd::UpdateScrollBar() {
  if (!this->scrollBar) {
    return;
  }

  const int rows = this->rowCount;
  const int count = this->itemCount;
  const int overflow = std::max(count - rows, 0);

  if (overflow != 0) {
    this->scrollEnabled = 1;
    this->scrollBar->SetRange(overflow, rows, count);

    int position = this->unknown_00C4;
    if (position < 0) {
      this->unknown_00C4 = 0;
      position = 0;
    }

    const int limit = count - this->rowCount;
    if (limit < position) {
      this->unknown_00C4 = limit;
      position = limit;
    }

    this->scrollBar->SetPosition(position);
  } else {
    this->scrollEnabled = 0;
    this->unknown_00C4 = 0;
    this->scrollBar->SetRange(0, rows, count);
  }

  this->scrollBar->Invalidate();
}
