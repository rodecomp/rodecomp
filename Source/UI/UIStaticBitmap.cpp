#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Buttons/UIButton.h"
#include "UI/UIStaticBitmap.h"

// OFFSET: 0x00850430
// FUNCTION: fn_10676
UIStaticBitmap::UIStaticBitmap(UIWindowContext& windows, UIButtonContext& controls)
    : UIWindow(windows, 0), tooltipX(0), tooltipY(-20), forwardHover(0), m_controls(&controls) {
}

// OFFSET: 0x00852F20
UIStaticBitmap::~UIStaticBitmap() = default;

// OFFSET: 0x00865740
// FUNCTION: fn_10930
void UIStaticBitmap::SetBitmap(const char* name) {
  this->bitmap = name;
  const auto* resource = static_cast<CBitmapRes*>(this->m_controls->resources.Load(this->bitmap.c_str()));
  this->geometry.width = resource->image.width;
  this->geometry.height = resource->image.height;
}

// OFFSET: 0x0085DA90
// FUNCTION: fn_10817
void UIStaticBitmap::Paint() {
  if (this->bitmap.empty()) {
    return;
  }

  if (this->geometry.unknown_0030 == 0) {
    const auto* resource = static_cast<CBitmapRes*>(this->m_controls->resources.Load(this->bitmap.c_str()));
    this->DrawBitmap(0, 0, resource ? &resource->image : nullptr, false);
  } else if (this->geometry.unknown_0030 == 1 && this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
  }
}

// OFFSET: 0x0085EE10
// FUNCTION: fn_10849
void UIStaticBitmap::Slot25(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot25((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085EA30
// FUNCTION: fn_10836
void UIStaticBitmap::Slot26(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot26((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F8A0
// FUNCTION: fn_10872
void UIStaticBitmap::Slot27(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot27((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F500
// FUNCTION: fn_10865
void UIStaticBitmap::Slot28(int x, int y) {
  if (this->forwardHover != 0) {
    static_cast<UIWindow*>(this->parent)->Slot28(x, y);
  }

  if (!this->tooltip.empty()) {
    int screenX = this->tooltipX;
    int screenY = this->tooltipY;

    for (const UIWindow* window = this; window; window = static_cast<const UIWindow*>(window->parent)) {
      screenX = (screenX + window->geometry.x);
      screenY = (screenY + window->geometry.y);
    }

    this->m_controls->services.Tooltip(this->tooltip.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x0085FA80
// FUNCTION: fn_10880
void UIStaticBitmap::ResetCursor(int x, int y) {
  static_cast<UIWindow*>(this->parent)->ResetCursor((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F000
// FUNCTION: fn_10854
void UIStaticBitmap::Slot31(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot31((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085FEA0
// FUNCTION: fn_10888
void UIStaticBitmap::Slot32(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot32((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085FF00
// FUNCTION: fn_10890
void UIStaticBitmap::Slot33(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot33((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x00860010
// FUNCTION: fn_10896
void UIStaticBitmap::Slot35(int value) {
  static_cast<UIWindow*>(this->parent)->Slot35(value);
}
