#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Text/UIStaticText.h"

#include <cstdio>

// OFFSET: 0x00850550
// FUNCTION: fn_10678
UIStaticText::UIStaticText(UIWindowContext& windows, UIButtonContext& controls)
    : UIWindow(windows, 0), red(0), green(0), blue(0), shadow(0), outline(0), background(0xFFFF00FFu), outlineColor(0x969696), fontHeight(12), fontFamily(0),
      bold(0), italic(0), tooltipEnabled(0), m_controls(&controls) {
}

// OFFSET: 0x00853050
// OFFSET: 0x00851E10
// FUNCTION: fn_10699
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

// OFFSET: 0x008685B0
// FUNCTION: fn_10961
void UIStaticText::SetColor(int redValue, int greenValue, int blueValue) {
  this->red = redValue;
  this->green = greenValue;
  this->blue = blueValue;
  this->Invalidate();
}

// OFFSET: 0x00868880
// FUNCTION: fn_10965
void UIStaticText::SetTextFitted(const char* value) {
  this->tooltip = value;
  this->tooltipEnabled = 0;

  if (this->geometry.surface == nullptr) {
    this->text = value;
    this->Invalidate();
    return;
  }

  int width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, value, 0, this->fontFamily, this->fontHeight, this->bold, 0);
  this->TextWidth(this->m_controls->fonts, this->m_controls->locale, "...", 0, this->fontFamily, this->fontHeight, this->bold, 0);

  if (this->geometry.width >= width) {
    this->text = value;
    this->Invalidate();
    return;
  }

  this->tooltipEnabled = 1;
  this->text = value;
  std::string truncated = value;
  while (this->geometry.width < width) {
    if (truncated.size() <= 1) {
      this->text = truncated + "...";
      break;
    }

    if (truncated.size() >= 2) {
      const std::size_t size = truncated.size();
      truncated.resize(truncated[size - 2] < 0 && truncated[size - 1] < 0 ? size - 2 : size - 1);
    }

    this->text = truncated + "...";
    width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->bold, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x00868C00
// FUNCTION: fn_10966
void UIStaticText::SetTextFittedCodePage(const char* value) {
  this->tooltip = value;
  this->tooltipEnabled = 0;

  if (this->geometry.surface == nullptr) {
    this->text = value;
    this->Invalidate();
    return;
  }

  int width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, value, 0, this->fontFamily, this->fontHeight, 0, 0);
  this->TextWidth(this->m_controls->fonts, this->m_controls->locale, "...", 0, this->fontFamily, this->fontHeight, 0, 0);
  if (this->geometry.width >= width) {
    this->text = value;
    this->Invalidate();
    return;
  }

  this->tooltipEnabled = 1;
  const int wideLength = MultiByteToWideChar(3, 0, value, -1, nullptr, 0);
  std::wstring truncated;
  if (wideLength > 0) {
    truncated.resize(wideLength);
    MultiByteToWideChar(3, 0, value, -1, truncated.data(), wideLength);
    truncated.resize(wideLength - 1);
  }

  while (this->geometry.width < width) {
    if (truncated.size() < 2) {
      const int byteLength = WideCharToMultiByte(3, 0, truncated.c_str(), -1, nullptr, 0, nullptr, nullptr);
      std::string fitted;
      if (byteLength > 0) {
        fitted.resize(byteLength);
        WideCharToMultiByte(3, 0, truncated.c_str(), -1, fitted.data(), byteLength, nullptr, nullptr);
        fitted.resize(byteLength - 1);
      }
      this->text = fitted + "...";
      break;
    }

    const std::size_t size = truncated.size();
    const bool removePair = (static_cast<unsigned char>(truncated[size - 2]) & 0x80) != 0 &&
                            (static_cast<unsigned char>(truncated[size - 1]) & 0x80) != 0;
    truncated.resize(size - (removePair ? 2 : 1));

    const int byteLength = WideCharToMultiByte(3, 0, truncated.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string fitted;
    if (byteLength > 0) {
      fitted.resize(byteLength);
      WideCharToMultiByte(3, 0, truncated.c_str(), -1, fitted.data(), byteLength, nullptr, nullptr);
      fitted.resize(byteLength - 1);
    }
    this->text = fitted + "...";
    width = this->TextWidth(
      this->m_controls->fonts, this->m_controls->locale, this->text.c_str(), 0, this->fontFamily, this->fontHeight, 0, 0
    );
  }

  this->Invalidate();
}
// OFFSET: 0x0085DE70
// FUNCTION: fn_10820
void UIStaticText::Paint() {
  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, this->background);
  const auto draw = [&](int x, int y, std::uint32_t color, unsigned char boldValue, unsigned char italicValue) {
    this->DrawTextAt(
      this->m_controls->fonts, this->m_controls->locale, x, y, this->text.c_str(), 0, this->fontFamily, this->fontHeight, color, boldValue, italicValue
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

  const unsigned color =
    static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16);
  draw(0, 0, color, this->bold, this->italic);
}

// OFFSET: 0x0085EE40
// FUNCTION: fn_10850
void UIStaticText::Slot25(int x, int y) {
  this->parent->Slot25((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085EA60
// FUNCTION: fn_10837
void UIStaticText::Slot26(int x, int y) {
  this->parent->Slot26((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F8D0
// FUNCTION: fn_10873
void UIStaticText::Slot27(int x, int y) {
  this->Slot28(x, y);
  this->parent->Slot27((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F580
// FUNCTION: fn_10866
void UIStaticText::Slot28(int, int) {
  if (this->tooltipEnabled == 0) {
    return;
  }

  int x = -6;
  int y = -4;

  for (const UIWindow* node = this; node; node = node->parent) {
    x = (x + node->geometry.x);
    y = (y + node->geometry.y);
  }

  this->m_controls->services.Tooltip(this->tooltip.c_str(), x, y, -1, false, true);
}

// OFFSET: 0x0085FAB0
// FUNCTION: fn_10881
void UIStaticText::ResetCursor(int x, int y) {
  this->parent->ResetCursor((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F030
// FUNCTION: fn_10855
void UIStaticText::Slot31(int x, int y) {
  this->parent->Slot31((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085FED0
// FUNCTION: fn_10889
void UIStaticText::Slot32(int x, int y) {
  this->parent->Slot32((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085FF30
// FUNCTION: fn_10891
void UIStaticText::Slot33(int x, int y) {
  this->parent->Slot33((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x00860020
// FUNCTION: fn_10897
void UIStaticText::Slot35(int value) {
  this->parent->Slot35(value);
}

// OFFSET: 0x0085DAF0
// FUNCTION: fn_10818
void UIStaticText2::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->ClearSurface(this->background);

  if (this->shadow != 0) {
    this->DrawTextAt(controls.fonts, controls.locale, 2, 2, this->text.c_str(), 0, this->fontFamily, this->fontHeight, 0xFFFFFF, this->bold, this->italic);
  }

  if (this->outline != 0) {
    this->DrawTextAt(controls.fonts, controls.locale, 0, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic);
    this->DrawTextAt(controls.fonts, controls.locale, 1, 0, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic);
    this->DrawTextAt(controls.fonts, controls.locale, 2, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic);
    this->DrawTextAt(controls.fonts, controls.locale, 1, 2, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic);
  }

  const std::uint32_t color =
    static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16);
  this->DrawTextAt(controls.fonts, controls.locale, 1, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, color, this->bold, this->italic);
}

// OFFSET: 0x0085DCA0
// FUNCTION: fn_10819
void UIStaticText3::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->ClearSurface(this->background);

  if (this->shadow != 0) {
    this->DrawTextAt(
      controls.fonts, controls.locale, this->textX + 2, 2, this->text.c_str(), 0, this->fontFamily, this->fontHeight, 0xFFFFFF, this->bold, this->italic
    );
  }

  if (this->outline != 0) {
    this->DrawTextAt(
      controls.fonts, controls.locale, this->textX, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic
    );
    this->DrawTextAt(
      controls.fonts, controls.locale, this->textX + 1, 0, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold,
      this->italic
    );
    this->DrawTextAt(
      controls.fonts, controls.locale, this->textX + 2, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold,
      this->italic
    );
    this->DrawTextAt(
      controls.fonts, controls.locale, this->textX + 1, 2, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold,
      this->italic
    );
  }

  const std::uint32_t color =
    static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16);
  this->DrawTextAt(controls.fonts, controls.locale, this->textX + 1, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, color, this->bold, this->italic);
}

// OFFSET: 0x00865CE0
// FUNCTION: fn_10936
void UIStaticText::SetFont(int family, int height, unsigned char bold, unsigned char italic) {
  this->fontFamily = family;
  this->fontHeight = height;
  this->bold = bold;
  this->italic = italic;
}

// OFFSET: 0x008685E0
// FUNCTION: fn_10962
void UIBmpTextButton::AddTextStyle(int state, int height, std::uint32_t color, unsigned char bold, unsigned char shadow, int x, int y) {
  if (x == -1000) {
    const int width = this->TextWidth(
      this->m_buttonContext->fonts, this->m_buttonContext->locale, this->text.c_str(), this->text.size(), 0, height, bold, 0
    );
    x = static_cast<int>((this->geometry.width - width) * 0.5);
  }

  if (y == -1000) {
    const int textHeight = this->TextHeight(
      this->m_buttonContext->fonts, this->m_buttonContext->locale, this->text.c_str(), this->text.size(), 0, height, bold, 0
    );
    y = static_cast<int>((this->geometry.height - textHeight) * 0.5);
  }

  for (const BitmapButtonTextStyle& style : this->styles) {
    if (style.state == state) {
      return;
    }
  }

  this->styles.push_back(BitmapButtonTextStyle{state, x, y, height, bold, color, shadow});
}

// OFFSET: 0x00868750
// FUNCTION: fn_10963
void UIStaticText::SetTextAutoWidth(const char* value) {
  this->tooltipEnabled = 0;
  this->tooltip = value;
  this->text = value;
  this->Invalidate();

  const int height = this->geometry.height;
  const int width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, value, 0, this->fontFamily, this->fontHeight, this->bold, 0);
  this->Resize(width + 2, height);
}

// OFFSET: 0x008687F0
// FUNCTION: fn_10964
void UIStaticText::SetNumber(int value) {
  char buffer[40];
  std::sprintf(buffer, "%d", value);
  this->text = buffer;
  this->tooltip = buffer;
  this->tooltipEnabled = 0;
  this->Invalidate();
}

// OFFSET: 0x00867CA0
// FUNCTION: fn_10948
void UIStaticText::SetTextWithinWidth(const char* value, int outlineValue) {
  this->text = value;
  this->tooltip = value;
  this->outline = outlineValue;
  this->tooltipEnabled = 0;

  const int width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, value, 0, 0, this->fontHeight, this->bold, 0);
  if (this->geometry.width < width) {
    this->SetTextFitted(value);
  } else {
    const int height = this->geometry.height;
    const int textWidth = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, value, 0, 0, this->fontHeight, this->bold, 0);
    this->Resize(textWidth, height);
  }
  this->Invalidate();
}
