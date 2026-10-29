#include "UI/StylingShop/UIStylingShopWnd.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00829B50
// FUNCTION: fn_10341
void UIStylingshopTabParentWnd::Slot15(int, int) {
}

// OFFSET: 0x00829B60
// FUNCTION: fn_10342
void UI2ndCostumeWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  for (int y = 230; y < 280; y += 25) {
    CBitmapRes* checkbox = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\checkbox.bmp")));
    if (checkbox) {
      this->DrawBitmap(13, y, &checkbox->image, false);
    }
  }

  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 0, &line->image, false);
  }
}

// OFFSET: 0x00829BE0
// FUNCTION: fn_10343
void UIAccessoryCusWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 5, &line->image, false);
  }
}

// OFFSET: 0x00829C20
// FUNCTION: fn_10344
void UIAccessoryShowCase::Paint() {
  this->ClearSurface(0xFFFFFFFF);
}

// OFFSET: 0x00829C30
// FUNCTION: fn_10345
void UIBasicStylingWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(249, 5, &line->image, false);
  }

  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\bg_styling_color.bmp")));
  if (background) {
    this->DrawBitmap(71, 233, &background->image, false);
  }
}

// OFFSET: 0x00829F30
// FUNCTION: fn_10348
void UIMyStyleWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* line = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\styleshop\\line_dotted_2px.bmp")));
  if (line) {
    this->DrawBitmap(-line->image.width - static_cast<int>(static_cast<float>(this->geometry.width) * -0.5f), 0, &line->image, false);
  }
}

// OFFSET: 0x0082A020
// FUNCTION: fn_10350
void UISlotOfMyStyleWnd::Paint() {
}

// OFFSET: 0x0082A090
// FUNCTION: fn_10352
void UIStylingshopTabParentWnd::Paint() {
}

// OFFSET: 0x0082A0A0
// FUNCTION: fn_10353
void UISlotForSelectWnd::Slot26(int, int) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, 121, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0082A0C0
// FUNCTION: fn_10354
void UI2ndCostumeCheckBox::Slot25(int x, int y) {
  if (this->m_disabled == 0 && this->geometry.unknown_0030 != 1) {
    UICheckBox::Slot25(x, y);
  }
}

// OFFSET: 0x0082A0E0
// FUNCTION: fn_10355
void UISlotForSelectWnd::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, 6, this->geometry.id, x, y, 0);
  }
}

// OFFSET: 0x0082A210
// FUNCTION: fn_10358
void UIStylingShopButton::Slot27(int x, int y) {
  if (this->m_disabled == 0 && g_windowMgr.Capture() == this && x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height &&
      this->geometry.unknown_0030 != 2) {
    this->geometry.unknown_0030 = 2;
    this->Invalidate();
  }
}

// OFFSET: 0x0082A330
// FUNCTION: fn_10360
void UIAccBitmapButton::Slot33(int, int) {
  this->parent->SendMsg(this, 101, this->geometry.id, this->m_argument, 0, 0);
}
