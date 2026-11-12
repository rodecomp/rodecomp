#include "UI/Login/UINewSelectCharWnd.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Text/UIStaticText.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"

#include <atlconv.h>
#include <cstdio>

template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);

// OFFSET: 0x00850550
// FUNCTION: fn_10678
__declspec(noinline) UIStaticText::UIStaticText() : UIWindow(0) {
  this->fontHeight = 12;
  this->fontFamily = 0;
  this->bold = 0;
  this->background = 0xFFFF00FFu;
  reinterpret_cast<UIWindow*>(this)->Invalidate();

  this->tooltipEnabled = 0;
  this->shadow = 0;
  this->outline = 0;
  this->red = 0;
  this->green = 0;
  this->blue = 0;
  this->text.clear();
  this->outlineColor = 0x969696;
  this->italic = 0;
}

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

  if (this->geometry.surface != nullptr) {
    int width = this->TextWidth(value, 0, this->fontFamily, this->fontHeight, this->bold, 0);
    this->TextWidth("...", 0, this->fontFamily, this->fontHeight, this->bold, 0);

    if (this->geometry.width >= width) {
      this->text = value;
    } else {
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
        width = this->TextWidth(this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->bold, 0);
      }
    }
  } else {
    this->text = value;
  }

  this->Invalidate();
}

// OFFSET: 0x00868C00
// FUNCTION: fn_10966
void UIStaticText::SetTextFittedCodePage(const char* value) {
  this->tooltip = value;
  this->tooltipEnabled = 0;

  if (this->geometry.surface != nullptr) {
    int width = this->TextWidth(value, 0, this->fontFamily, this->fontHeight, 0, 0);
    this->TextWidth("...", 0, this->fontFamily, this->fontHeight, 0, 0);
    if (this->geometry.width >= width) {
      this->text = value;
    } else {
      this->tooltipEnabled = 1;
      this->text = value;
      std::wstring truncated(ATL::CA2WEX<128>(value, CP_THREAD_ACP));
      while (this->geometry.width < width) {
        if (truncated.size() <= 1) {
          this->text = std::string(ATL::CW2AEX<128>(truncated.c_str(), CP_THREAD_ACP)) + "...";
          break;
        }

        if (truncated.size() >= 2) {
          const std::size_t size = truncated.size();
          truncated.resize((truncated[size - 2] & 0x80) != 0 && (truncated[size - 1] & 0x80) != 0 ? size - 2 : size - 1);
        }

        this->text = std::string(ATL::CW2AEX<128>(truncated.c_str(), CP_THREAD_ACP)) + "...";
        width = this->TextWidth(this->text.c_str(), 0, this->fontFamily, this->fontHeight, 0, 0);
      }
    }
  } else {
    this->text = value;
  }

  this->Invalidate();
}
// OFFSET: 0x0085DE70
// FUNCTION: fn_10820
void UIStaticText::Paint() {
  this->ClearSurface(this->background);

  if (this->shadow != 0) {
    this->DrawTextAt(1, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, 0xFFFFFF, 0, 0);
  }

  if (this->outline != 0) {
    this->DrawTextAt(-1, 0, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, 0, 0);
    this->DrawTextAt(1, 0, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, 0, 0);
    this->DrawTextAt(0, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, 0, 0);
  }

  const unsigned color =
    static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16);
  this->DrawTextAt(0, 0, this->text.c_str(), 0, this->fontFamily, this->fontHeight, color, this->bold, this->italic);
}

// OFFSET: 0x0085EE40
// FUNCTION: fn_10850
void UIStaticText::OnLBtnDown(int x, int y) {
  this->parent->OnLBtnDown((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085EA60
// FUNCTION: fn_10837
void UIStaticText::OnLBtnDblClk(int x, int y) {
  this->parent->OnLBtnDblClk((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F8D0
// FUNCTION: fn_10873
void UIStaticText::OnMouseMove(int x, int y) {
  this->OnMouseHover(x, y);
  this->parent->OnMouseMove((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F580
// FUNCTION: fn_10866
void UIStaticText::OnMouseHover(int, int) {
  if (this->tooltipEnabled == 0) {
    return;
  }

  int x = -6;
  int y = -4;
  this->ToScreen(x, y);

  g_modeMgr.gameMode->ShowTooltip(this->tooltip.c_str(), x, y, -1, false, true);
}

// OFFSET: 0x0085FAB0
// FUNCTION: fn_10881
void UIStaticText::ResetCursor(int x, int y) {
  this->parent->ResetCursor((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085F030
// FUNCTION: fn_10855
void UIStaticText::OnLBtnUp(int x, int y) {
  this->parent->OnLBtnUp((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085FED0
// FUNCTION: fn_10889
void UIStaticText::OnRBtnDown(int x, int y) {
  this->parent->OnRBtnDown((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x0085FF30
// FUNCTION: fn_10891
void UIStaticText::OnRBtnUp(int x, int y) {
  this->parent->OnRBtnUp((x + this->geometry.x), (y + this->geometry.y));
}

// OFFSET: 0x00860020
// FUNCTION: fn_10897
void UIStaticText::OnWheel(int value) {
  this->parent->OnWheel(value);
}

// OFFSET: 0x0085DAF0
// FUNCTION: fn_10818
void UIStaticText2::Paint() {
  this->ClearSurface(this->background);

  if (this->shadow != 0) {
    this->DrawTextAt(2, 2, this->text.c_str(), 0, this->fontFamily, this->fontHeight, 0xFFFFFF, this->bold, this->italic);
  }

  if (this->outline != 0) {
    this->DrawTextAt(0, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic);
    this->DrawTextAt(1, 0, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic);
    this->DrawTextAt(2, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic);
    this->DrawTextAt(1, 2, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic);
  }

  const std::uint32_t color =
    static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16);
  this->DrawTextAt(1, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, color, this->bold, this->italic);
}

// OFFSET: 0x0085DCA0
// FUNCTION: fn_10819
void UIStaticText3::Paint() {
  this->ClearSurface(this->background);

  if (this->shadow != 0) {
    this->DrawTextAt(this->textX + 2, 2, this->text.c_str(), 0, this->fontFamily, this->fontHeight, 0xFFFFFF, this->bold, this->italic
    );
  }

  if (this->outline != 0) {
    this->DrawTextAt(this->textX, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold, this->italic
    );
    this->DrawTextAt(this->textX + 1, 0, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold,
      this->italic
    );
    this->DrawTextAt(this->textX + 2, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold,
      this->italic
    );
    this->DrawTextAt(this->textX + 1, 2, this->text.c_str(), 0, this->fontFamily, this->fontHeight, this->outlineColor, this->bold,
      this->italic
    );
  }

  const std::uint32_t color =
    static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16);
  this->DrawTextAt(this->textX + 1, 1, this->text.c_str(), 0, this->fontFamily, this->fontHeight, color, this->bold, this->italic);
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
  BitmapButtonTextStyle style;
  style.state = state;
  style.x = x;
  style.y = y;
  style.height = height;
  style.color = color;
  style.bold = bold;
  style.shadow = shadow;

  if (x == -1000) {
    const int width = this->TextWidth(this->text.c_str(), this->text.size(), 0, height, bold, 0
    );
    style.x = static_cast<int>((this->geometry.width - width) * 0.5);
  }

  if (y == -1000) {
    const int textHeight = this->TextHeight(this->text.c_str(), this->text.size(), 0, height, bold, 0
    );
    style.y = static_cast<int>((this->geometry.height - textHeight) * 0.5);
  }

  for (const BitmapButtonTextStyle existing : this->styles) {
    if (state == existing.state) {
      return;
    }
  }

  this->styles.push_back(style);
}

// OFFSET: 0x00868750
// FUNCTION: fn_10963
void UIStaticText::SetTextAutoWidth(const char* value) {
  this->tooltipEnabled = 0;
  this->tooltip = value;
  this->text = value;
  this->Invalidate();

  this->Resize(this->TextWidth(value, 0, this->fontFamily, this->fontHeight, this->bold, 0) + 2, this->geometry.height);
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

  const int width = this->TextWidth(value, 0, 0, this->fontHeight, this->bold, 0);
  if (this->geometry.width < width) {
    this->SetTextFitted(value);
  } else {
    this->Resize(this->TextWidth(value, 0, 0, this->fontHeight, this->bold, 0), this->geometry.height);
  }
  this->Invalidate();
}

// OFFSET: 0x008504F0
// FUNCTION: fn_10677
UIStaticText3::UIStaticText3() : UIStaticText(), textX(0) {
}
