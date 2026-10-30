#include "UI/Items/UIGameItem.h"
#include "UI/Text/ColoredText.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include <cstdlib>
#include <format>
#include <string>

// OFFSET: 0x009F71F0
// FUNCTION: fn_14769
CUIGameItem::CUIGameItem(UIWindowContext& context)
    : CUIControl(context), mode(0), unknown_0084(1), unknown_0085(1), transparent(0), scale(100), itemInfo(), name(), image(nullptr), showOverlay(0),
      overlay(nullptr), onClick() {
}

// OFFSET: 0x009E72A0
// FUNCTION: fn_14476
void CUIGameItem::SetMode(int mode) {
  this->mode = mode;
  this->SetSize(mode == 1 ? 75 : 24, mode == 1 ? 100 : 24);
}

// OFFSET: 0x009E72E0
// FUNCTION: fn_14477
void CUIGameItemWithNum::SetMode(int mode) {
  this->mode = mode;
  this->SetSize(mode == 1 ? 75 : 38, mode == 1 ? 100 : 25);
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

  if (this->scale != 100) {
    if (this->transparent) {
      this->DrawAuxiliaryScaled(1, 0, 0, this->geometry.width, this->geometry.height, &this->image->image, 1, false);
    } else {
      this->DrawBitmapScaled(0, 0, this->geometry.width, this->geometry.height, &this->image->image, 0);
    }
  } else if (this->transparent) {
    this->DrawAuxiliary(1, 0, 0, &this->image->image, true, false);
  } else {
    this->DrawBitmap(0, 0, &this->image->image, false);
  }

  if (this->showOverlay && this->overlay) {
    this->DrawBitmap(0, 12, &this->overlay->image, true);
  }
}

// OFFSET: 0x009E73E0
// FUNCTION: fn_14480
void CUIGameItemWithNum::Paint() {
  CUIGameItem::Paint();
  if (this->itemInfo.quantity > 0 && this->mode == 0) {
    std::string text = std::to_string(this->itemInfo.quantity);
    DrawOutlinedText(*this, 14, 15, text.c_str(), 0, 0, 0xFFFFFF, 0, 11, 0, 0);
  }
}

// OFFSET: 0x009E75E0
// FUNCTION: fn_14481
void CUIGameItem::Slot25(int x, int y) {
  if (this->onClick) {
    this->onClick(this);
    return;
  }
  CUIControl::Slot25(x, y);
}

// OFFSET: 0x009E7610
// FUNCTION: fn_14482
void CUIGameItem::Slot28(int x, int y) {
  if (this->unknown_0084 == 0 || this->name.empty()) {
    return;
  }

  int screenX = x;
  int screenY = y - 20;
  this->ToScreen(screenX, screenY);
  const std::uint32_t color = g_session.GetItemNameColor(this->itemInfo);
  ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_charViewMode);
  if (this->itemInfo.unknown_0098 > 0) {
    const std::string text = FormatOwnedString(LookupMessage(0xA28), this->name.c_str(), this->itemInfo.unknown_0098);
    tooltip->Show(text.c_str(), screenX, screenY, color, 0, 1);
  } else {
    tooltip->Show(this->name.c_str(), screenX, screenY, color, 0, 1);
  }
}

// OFFSET: 0x009E7750
// FUNCTION: fn_14483
void CUIGameItem::Slot26(int, int) {
  if (this->unknown_0085 == 0 || std::atoi(this->itemInfo.itemId.c_str()) <= 0) {
    return;
  }

  WindowManagerWindows& windows = reinterpret_cast<WindowManagerWindows&>(g_windowMgr);
  UIWindow* window = windows.EnsureWindow(12);
  if (window != nullptr) {
    window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&this->itemInfo), 0, 0, 0);
  }
}

// OFFSET: 0x009E79D0
// FUNCTION: fn_14484
void CUIGameItem::SetItem(const ITEM_INFO& item) {
  if (std::atoi(item.itemId.c_str()) <= 0) {
    return;
  }

  this->itemInfo = item;
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  this->name = naming.DisplayName(this->itemInfo, 0);

  std::string path;
  if (this->mode == 0) {
    path = std::format("유저인터페이스\\item\\{}.bmp", ItemResourcePath(this->itemInfo));
  } else if (this->mode == 1) {
    path = std::format("유저인터페이스\\collection\\{}.bmp", ItemResourcePath(this->itemInfo));
  } else {
    return;
  }

  if (path.empty()) {
    return;
  }

  CBitmapRes* image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(path));
  if (image) {
    image->Lock();
  }
  CBitmapRes* oldImage = this->image;
  this->image = image;
  if (oldImage) {
    oldImage->Unlock();
  }

  if (item.unknown_0088 > 0) {
    CBitmapRes* overlay = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(std::format("유저인터페이스\\grade_enchant\\grade_icon{}.bmp", item.unknown_0088)));
    if (overlay) {
      overlay->Lock();
    }
    CBitmapRes* oldOverlay = this->overlay;
    this->overlay = overlay;
    if (oldOverlay) {
      oldOverlay->Unlock();
    }
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
