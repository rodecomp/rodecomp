#include "Resource/TextureNames.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Buttons/UIButton.h"
#include "UI/UIStaticBitmap.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"

// OFFSET: 0x00850430
// FUNCTION: fn_10676
UIStaticBitmap::UIStaticBitmap() : UIWindow(0) {
  this->forwardHover = 0;
  this->tooltipX = 0;
  this->tooltipY = -20;
}

UIStaticBitmap::~UIStaticBitmap() = default;

// OFFSET: 0x00865740
// FUNCTION: fn_10930
void UIStaticBitmap::SetBitmap(const char* name) {
  this->bitmap = name;
  const auto* resource = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmap.c_str())));
  this->geometry.width = resource->image.width;
  this->geometry.height = resource->image.height;
}

// OFFSET: 0x00869790
// FUNCTION: fn_10969
void UIStaticBitmap::SetTooltip(const char* text) {
  if (text) {
    this->tooltip = text;
  }
}

// OFFSET: 0x0085DA90
// FUNCTION: fn_10817
void UIStaticBitmap::Paint() {
  if (this->bitmap.empty()) {
    return;
  }

  switch (this->geometry.unknown_0030) {
  case 0:
    this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmap.c_str())))->image, false);
    break;
  case 1:
    this->ClearSurface(0xFFFF00FFu);
    break;
  }
}

// OFFSET: 0x0085EE10
// FUNCTION: fn_10849
void UIStaticBitmap::Slot25(int x, int y) {
  this->parent->Slot25((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085EA30
// FUNCTION: fn_10836
void UIStaticBitmap::Slot26(int x, int y) {
  this->parent->Slot26((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F8A0
// FUNCTION: fn_10872
void UIStaticBitmap::Slot27(int x, int y) {
  this->parent->Slot27((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F500
// FUNCTION: fn_10865
void UIStaticBitmap::Slot28(int x, int y) {
  if (this->forwardHover != 0) {
    this->parent->Slot28(x, y);
  }

  if (!this->tooltip.empty()) {
    int screenX = this->tooltipX;
    int screenY = this->tooltipY;
    this->ToScreen(screenX, screenY);
    g_modeMgr.gameMode->ShowTooltip(this->tooltip.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x0085FA80
// FUNCTION: fn_10880
void UIStaticBitmap::ResetCursor(int x, int y) {
  this->parent->ResetCursor((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F000
// FUNCTION: fn_10854
void UIStaticBitmap::Slot31(int x, int y) {
  this->parent->Slot31((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085FEA0
// FUNCTION: fn_10888
void UIStaticBitmap::Slot32(int x, int y) {
  this->parent->Slot32((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085FF00
// FUNCTION: fn_10890
void UIStaticBitmap::Slot33(int x, int y) {
  this->parent->Slot33((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x00860010
// FUNCTION: fn_10896
void UIStaticBitmap::Slot35(int value) {
  this->parent->Slot35(value);
}
