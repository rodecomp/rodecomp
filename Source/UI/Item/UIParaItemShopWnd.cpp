#include "UI/Item/UIParaItemShopWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Dialogs/UIPasswordWnd.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdlib>

// OFFSET: 0x009A80F0
// FUNCTION: fn_13745
void UIParaResultWnd::AddItem(const RawItemBasic& item, int quantity) {
  ITEM_INFO info;
  info.AssignBasic(item);
  info.quantity = quantity;
  info.unknown_0060 = 0;
  info.unknown_005D = 0;
  info.SetItemId(atoi(info.itemId.c_str()));
  this->items.push_back(info);
}

// OFFSET: 0x009A81E0
// FUNCTION: fn_13746
void UIParaResultWnd::AddItem(const ITEM_INFO& item) {
  this->items.push_back(item);
}

// OFFSET: 0x009A81F0
// FUNCTION: fn_13747
void UIParaItemPurchaseWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 3) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  switch (drag->type) {
  case 4:
  case 5:
  case 8:
  case 9:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  default:
    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    dynamic_cast<UIItemDropCntWnd*>(g_windowMgr.LookupWindow(15))->SendMsg(nullptr, 13, 21, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x009A82E0
// FUNCTION: fn_13748
void UIParaItemShopWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 4) {
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  dynamic_cast<UIItemDropCntWnd*>(g_windowMgr.LookupWindow(15))->SendMsg(nullptr, 13, 22, 0, 0, 0);
}

// OFFSET: 0x009A83B0
// FUNCTION: fn_13749
void UIParaResultWnd::Slot10(int x, int y, std::intptr_t value) {
}

// OFFSET: 0x009A96C0
// FUNCTION: fn_13754
void UIParaItemPurchaseWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  int previousHovered = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);
  if (previousHovered != this->hovered) {
    this->Invalidate();
  }
}

// OFFSET: 0x009A99C0
// FUNCTION: fn_13756
void UIParaItemShopWnd::Slot32(int x, int y) {
  const int previousHovered = this->unknown_00F8;
  this->unknown_00F8 = this->GetItemIndexAt(x, y);
  if (previousHovered != this->unknown_00F8) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->unknown_00F8);
  if (item.unknown_0000[1] != 0) {
    g_modeMgr.GetGameMode();
    UIWindow* window = g_windowMgr.EnsureWindow(0xC);
    window->SendMsg(nullptr, 0x18, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x009A96B0
// FUNCTION: fn_13753
void __stdcall Nullsub344(int, int) {
}
