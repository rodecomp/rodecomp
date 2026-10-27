#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/ItemLinkDisplay.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"

#include <algorithm>

namespace {
  // INLINED
  unsigned Digit(unsigned char value) {
    if (value >= '0' && value <= '9') {
      return value - '0';
    }

    if (value >= 'a' && value <= 'f') {
      return value - 'a' + 10;
    }

    if (value >= 'A' && value <= 'F') {
      return value - 'A' + 10;
    }

    return 0;
  }

  // INLINED
  void ScanColors(const std::string& line, unsigned short codePage, std::uint32_t& color) {
    const char* current = line.c_str();
    const char* end = current + line.size();

    while (current < end) {
      bool marker = *current == '^' && end - current >= 6;

      for (int index = 1; marker && index <= 6; ++index) {
        const char value = current[index];
        marker = (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
      }

      if (marker) {
        color = (Digit(current[2]) + 16 * Digit(current[1])) | ((Digit(current[4]) + 16 * Digit(current[3])) << 8) |
                ((Digit(current[6]) + 16 * Digit(current[5])) << 16);
        current += 7;
      } else {
        current = CharNextExA(codePage, current, 0);
      }
    }
  }
}

// OFFSET: 0x0086F0A0
// FUNCTION: fn_11086
UITextViewer::UITextViewer(UIWindowContext& windows, UITextViewerContext& text)
    : UIListBox(windows, text.list), splitTipLinks(0), outline(0), bold(0), m_text(&text) {
  this->lineHeight = 18;
  constexpr char path[] = "유저인터페이스\\colorchip.bmp";
  const auto* chip = static_cast<CBitmapRes*>(text.list.resources.Load(path));
  const auto& image = chip->image;
  const std::uint32_t pixel = image.width > 18 && image.height > 2 ? image.pixels[18 + 2 * image.width] : 0x00FF0000u;
  this->backgroundColor = 0xFF000000u | ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  this->Invalidate();

  this->unknown_00BC = 0;
}

// OFFSET: 0x008747F0
UITextViewer::~UITextViewer() = default;

// OFFSET: 0x008872A0
// FUNCTION: fn_11334
void UITextViewer::Slot15(int width, int height) {
  UIListBox::Slot15(width, height);
}

// OFFSET: 0x0087B780
// FUNCTION: fn_11249
void UITextViewer::Configure(int x, int y, int width, int height, unsigned char value) {
  UIListBox::Configure(x, y, width, height, value);
}

// OFFSET: 0x008760C0
// FUNCTION: fn_11204
void UITextViewer::AddLine(const char* text) {
  UIListBox::AddLine(text);
}

// OFFSET: 0x0087A290
// FUNCTION: fn_11229
void UITextViewer::Clear() {
  UIListBox::Clear();

  for (UITextButton* link : this->links) {
    this->RemoveChild(link, 1);
  }

  this->links.clear();
  this->Invalidate();
}

// OFFSET: 0x008909F0
// FUNCTION: fn_11429
void UITextViewer::Slot25(int, int) {
}

// OFFSET: 0x007B78C0
// FUNCTION: fn_09227
void UITextViewer::Slot26(int, int) {
}

// OFFSET: 0x007B7A10
// FUNCTION: fn_09233
void UITextViewer::Slot31(int, int) {
}

// OFFSET: 0x0088FBB0
// FUNCTION: fn_11394
void UITextViewer::Paint() {
  if (this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, this->transparent != 0 ? 0xFFFF00FFu : this->backgroundColor);
  }

  if (this->transparent != 0 && this->parent) {
    static_cast<UIWindow*>(this->parent)->Invalidate();
  }

  int divisor = (this->lineHeight - this->bottomMargin);

  if (divisor == 0) {
    divisor = 1;
  }

  const int limit = (this->geometry.height / divisor + this->topLine);
  const int end = static_cast<int>(std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(limit)));
  auto& owner = this->m_text->list;
  std::uint32_t color = 0;

  for (int index = 0; index < this->topLine; ++index) {
    ScanColors(this->lines[index], owner.codePage, color);
  }

  for (int index = this->topLine; index < end; ++index) {
    const auto& line = this->lines[index];
    const int y = (index - this->topLine) * this->lineHeight + 4;
    const auto bytes = static_cast<unsigned>(line.size());

    if ((this->unknown_00BC & 255) != 0) {
      DrawOutlinedText(*this, owner.fonts, owner.locale, 3, y, line.c_str(), bytes, 0, 0xFFFFFF, 0, 12, 0, 0);
    } else if (this->outline != 0) {
      DrawOutlinedColoredText(*this, owner.fonts, owner.locale, owner.codePage, 3, y, line.c_str(), bytes, color, this->backgroundColor, 0, 12, this->bold);
    } else {
      DrawColoredText(*this, owner.fonts, owner.locale, owner.codePage, 3, y, line.c_str(), bytes, color, 0, 12, false);
    }
  }
}
