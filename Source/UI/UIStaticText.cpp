#include "Render/DeviceContext.h"
#include "UI/UIButton.h"
#include "UI/UIStaticText.h"

#include <bit>

namespace {
  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }
}

// OFFSET: 0x00850550
// FUNCTION: fn_10678
UIStaticText::UIStaticText(UIWindowContext& windows, UIButtonContext& controls)
    : UIWindow(windows, 0), red(0), green(0), blue(0), shadow(0), outline(0), background(0xFFFF00FFu),
      outlineColor(0x969696), fontHeight(12), fontFamily(0), bold(0), italic(0), tooltipEnabled(0),
      m_controls(&controls) {
}

// OFFSET: 0x00853050
// FUNCTION: fn_10732
UIStaticText::~UIStaticText() = default;

// OFFSET: 0x008681C0
// FUNCTION: fn_10955
void UIStaticText::SetText(const char* value, int outlineValue) {
  this->text = value;
  this->tooltip = value;
  this->outline = outlineValue;
  this->tooltipEnabled = 0;
  this->Invalidate();
}

// OFFSET: 0x0085DE70
// FUNCTION: fn_10820
void UIStaticText::Paint() {
  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, this->background);
  const auto draw = [&](int x, int y, std::uint32_t color, unsigned char boldValue, unsigned char italicValue) {
    this->DrawTextAt(
      this->m_controls->fonts, this->m_controls->locale, x, y, this->text.c_str(), 0, this->fontFamily,
      this->fontHeight, color, boldValue, italicValue
    );
  };

  if (this->shadow != 0) {
    draw(1, 1, 0xFFFFFF, 0, 0);
  }

  if (this->outline != 0) {
    draw(-1, 0, this->outlineColor, 0, 0);
    draw(1, 0, this->outlineColor, 0, 0);
    draw(0, 1, this->outlineColor, 0, 0);
  }

  const unsigned color = static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) |
                         (static_cast<unsigned char>(this->blue) << 16);
  draw(0, 0, color, this->bold, this->italic);
}

// OFFSET: 0x0085EE40
// FUNCTION: fn_10850
void UIStaticText::Slot25(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot25(Add(x, this->geometry.x), Add(y, this->geometry.y));
}

// OFFSET: 0x0085EA60
// FUNCTION: fn_10837
void UIStaticText::Slot26(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot26(Add(x, this->geometry.x), Add(y, this->geometry.y));
}

// OFFSET: 0x0085F8D0
// FUNCTION: fn_10873
void UIStaticText::Slot27(int x, int y) {
  this->Slot28(x, y);
  static_cast<UIWindow*>(this->parent)->Slot27(Add(x, this->geometry.x), Add(y, this->geometry.y));
}

// OFFSET: 0x0085F580
// FUNCTION: fn_10866
void UIStaticText::Slot28(int, int) {
  if (this->tooltipEnabled == 0) {
    return;
  }

  int x = -6;
  int y = -4;

  for (const UIWindow* node = this; node; node = static_cast<const UIWindow*>(node->parent)) {
    x = Add(x, node->geometry.x);
    y = Add(y, node->geometry.y);
  }

  this->m_controls->services.Tooltip(this->tooltip.c_str(), x, y, -1, false, true);
}

// OFFSET: 0x0085FAB0
// FUNCTION: fn_10881
void UIStaticText::ResetCursor(int x, int y) {
  static_cast<UIWindow*>(this->parent)->ResetCursor(Add(x, this->geometry.x), Add(y, this->geometry.y));
}

// OFFSET: 0x0085F030
// FUNCTION: fn_10855
void UIStaticText::Slot31(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot31(Add(x, this->geometry.x), Add(y, this->geometry.y));
}

// OFFSET: 0x0085FED0
// FUNCTION: fn_10889
void UIStaticText::Slot32(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot32(Add(x, this->geometry.x), Add(y, this->geometry.y));
}

// OFFSET: 0x0085FF30
// FUNCTION: fn_10891
void UIStaticText::Slot33(int x, int y) {
  static_cast<UIWindow*>(this->parent)->Slot33(Add(x, this->geometry.x), Add(y, this->geometry.y));
}

// OFFSET: 0x00860020
// FUNCTION: fn_10897
void UIStaticText::Slot35(int value) {
  static_cast<UIWindow*>(this->parent)->Slot35(value);
}
