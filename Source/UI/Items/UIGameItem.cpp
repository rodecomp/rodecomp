#include "UI/Items/UIGameItem.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Text/ColoredText.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include "Client/Mode/CMode.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdlib>
#include <format>
#include <string>

// OFFSET: 0x009E72A0
// FUNCTION: fn_14476
void CUIGameItem::SetMode(int mode) {
  this->mode = mode;
  this->SetSize(mode != 1 ? 24 : 75, mode != 1 ? 24 : 100);
}

// OFFSET: 0x009E72E0
// FUNCTION: fn_14477
void CUIGameItemWithNum::SetMode(int mode) {
  this->mode = mode;
  this->SetSize(mode != 1 ? 38 : 75, mode != 1 ? 25 : 100);
}

// OFFSET: 0x009E7320
// FUNCTION: fn_14478
ITEM_INFO* CUIGameItem::GetItemInfo() {
  return &this->itemInfo;
}

// OFFSET: 0x009E7330
// FUNCTION: fn_14479
void CUIGameItem::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->image == nullptr) {
    return;
  }

  if (100 != this->scale) {
    if (this->transparent) {
      this->DrawAuxiliaryScaled(1, 0, 0, this->geometry.width, this->geometry.height, this->image, 1, false);
    } else {
      this->DrawBitmapScaled(0, 0, this->geometry.width, this->geometry.height, this->image, 0);
    }
  } else if (this->transparent) {
    this->DrawAuxiliary(1, 0, 0, this->image, true, false);
  } else {
    this->DrawBitmap(0, 0, this->image, false);
  }

  if (this->showOverlay && this->overlay) {
    this->DrawBitmap(0, 12, this->overlay, true);
  }
}

// OFFSET: 0x009E73E0
// FUNCTION: fn_14480
void CUIGameItemWithNum::Paint() {
  CUIGameItem::Paint();
  if (this->itemInfo.quantity > 0 && this->mode == 0) {
    std::string text = std::to_string(this->itemInfo.quantity);
    this->DrawOutlinedText(14, 15, text.c_str(), 0, 0, 0xFFFFFF, 0, 11, 0, 0);
  }
}

// OFFSET: 0x009E75E0
// FUNCTION: fn_14481
void CUIGameItem::OnLBtnDown(int x, int y) {
  if (this->onClick) {
    this->onClick(this);
    return;
  }
  CUIControl::OnLBtnDown(x, y);
}

// OFFSET: 0x009E7610
// FUNCTION: fn_14482
void CUIGameItem::OnMouseHover(int x, int y) {
  if (this->showNameTooltip == 0 || this->name.empty()) {
    return;
  }

  int screenX = x;
  int screenY = y - 20;
  this->ToScreen(screenX, screenY);
  if (this->itemInfo.optionCount > 0) {
    CMode* tooltip = g_modeMgr.currentMode;
    tooltip->ShowTooltip(
      FormatOwnedString(LookupMessage(MSI_EA6), this->name.c_str(), this->itemInfo.optionCount).c_str(),
      screenX, screenY, g_session.GetItemNameColor(this->itemInfo), 0, 1
    );
  } else {
    CMode* tooltip = g_modeMgr.currentMode;
    tooltip->ShowTooltip(this->name.c_str(), screenX, screenY, g_session.GetItemNameColor(this->itemInfo), 0, 1);
  }
}

// OFFSET: 0x009E7750
// FUNCTION: fn_14483
void CUIGameItem::OnRBtnDown(int, int) {
  if (this->allowItemInfo == 0 || std::atoi(this->itemInfo.itemId.c_str()) <= 0) {
    return;
  }

  WindowManagerWindows& windows = g_windowMgr;
  UIWindow* window = windows.EnsureWindow(WID_ITEMCOLLECTIONWND);
  if (window != nullptr) {
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->itemInfo), 0, 0, 0);
  }
}

// OFFSET: 0x009E77B0
// FUNCTION: fn_45731
__forceinline void CUIGameItem::ClearItem() {
  this->itemInfo.Reset();
  this->name = "";
  this->image = nullptr;
  this->overlay = nullptr;
  this->Invalidate();
}

// OFFSET: 0x009E7870
// FUNCTION: fn_45732
void CUIGameItem::SetItemById(int itemId) {
  if (itemId == 0) {
    this->ClearItem();
    return;
  }

  const ITEM_INFO* item = g_session.FindInventoryItemByItemId(itemId, false);
  if (item && item->identity[1] > 0) {
    this->SetItem(*item);
    return;
  }

  ITEM_INFO fallback;
  fallback.SetItemId(itemId);
  this->SetItem(fallback);
}

// OFFSET: 0x009E79D0
// FUNCTION: fn_14484
void CUIGameItem::SetItem(const ITEM_INFO& item) {
  if (std::atoi(item.itemId.c_str()) <= 0) {
    return;
  }

  this->itemInfo = item;
  this->name = this->itemInfo.DisplayName(0);

  std::string path;
  if (this->mode == 0) {
    path = std::format("유저인터페이스\\item\\{}.bmp", this->itemInfo.ResourcePath());
  } else if (this->mode == 1) {
    path = std::format("유저인터페이스\\collection\\{}.bmp", this->itemInfo.ResourcePath());
  } else {
    return;
  }

  if (path.empty()) {
    return;
  }

  this->image = CResPtr<CBitmapRes>(static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(path)));

  if (0 < item.grade) {
    this->overlay = CResPtr<CBitmapRes>(static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(std::format("유저인터페이스\\grade_enchant\\grade_icon{}.bmp", item.grade))));
  }

  this->Invalidate();
}

// OFFSET: 0x009E7CE0
// FUNCTION: fn_14485
void CUIGameItem::SetScale(short scale) {
  if (scale == this->scale) {
    return;
  }

  float ratio = static_cast<float>(scale) / 100.0f;
  this->scale = scale;
  if (this->mode == 1) {
    this->Resize(static_cast<int>(ratio * 75.0f), static_cast<int>(ratio * 100.0f));
  } else {
    this->Resize(static_cast<int>(ratio * 24.0f), static_cast<int>(ratio * 24.0f));
  }
}
