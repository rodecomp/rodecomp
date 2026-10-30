#include "UI/Items/UIGameItem.h"
#include "UI/Text/ColoredText.h"

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
