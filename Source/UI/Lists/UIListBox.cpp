#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/ColoredText.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <cstring>

namespace {}

// OFFSET: 0x0086D940
// FUNCTION: fn_11055
UIListBox::UIListBox(UIWindowContext& windows, UIListBoxContext& list)
    : UIWindow(windows, 0), backgroundRed(255), backgroundGreen(255), backgroundBlue(255), selected(0), topLine(0), horizontalOffset(0), contentWidth(0),
      verticalOverflow(0), bottomMargin(0), verticalBar(nullptr), horizontalBar(nullptr), transparent(0), lineHeight(16), selectable(1), textColor(0),
      centerVertically(0), textInset(3), m_listContext(&list) {
  constexpr char path[] = "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\colorchip.bmp";
  const auto* chip = static_cast<CBitmapRes*>(list.resources.Load(path));
  this->selectionColor = 0;

  if (chip) {
    const auto& image = chip->image;
    const std::uint32_t pixel = image.width > 14 && image.height > 10 ? image.pixels[14 + 10 * image.width] : 0x00FF0000u;
    this->selectionColor = ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  }
}

// OFFSET: 0x00873690
// FUNCTION: fn_11156
UIListBox::~UIListBox() = default;

// OFFSET: 0x0087B640
// FUNCTION: fn_11245
void UIListBox::Configure(int x, int y, int width, int height, unsigned char value) {
  this->transparent = value;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x00886840
// FUNCTION: fn_11326
void UIListBox::Slot15(int width, int height) {
  auto& owner = *this->m_listContext;
  auto* vertical = new UIScrollBar(*this->m_context, owner.capture, owner.resources);
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move((width - 13), 0);
  vertical->Slot15(verticalWidth, height);
  vertical->Slot22(verticalWidth, height);
  this->AddChild(vertical);

  auto* horizontal = new UIScrollBar(*this->m_context, owner.capture, owner.resources);
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize((width - 13), horizontalHeight);
  horizontal->Move(0, (height - 13));
  horizontal->Slot15((width - 13), horizontalHeight);
  horizontal->Slot22((width - 13), horizontalHeight);
  this->AddChild(horizontal);
}

// OFFSET: 0x008976E0
// FUNCTION: fn_11532
void UIListBox::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->verticalBar->Resize(12, height);
  this->verticalBar->Move((width - 13), 0);
  this->horizontalBar->Resize((width - 13), 12);
  this->horizontalBar->Move(0, (height - 13));
  this->Refresh();
  this->topLine = 0;
}

// OFFSET: 0x00875A70
// FUNCTION: fn_11201
void UIListBox::AddLine(const char* text) {
  this->lines.emplace_back(text);
  auto& owner = *this->m_listContext;
  const int width = this->TextWidth(owner.fonts, owner.locale, text, 0, 0, 12, 0, 0);
  this->contentWidth = std::max(this->contentWidth, width);
  this->Refresh();
}

// OFFSET: 0x00879F80
// FUNCTION: fn_11223
void UIListBox::Clear() {
  this->selected = -1;
  this->contentWidth = 0;
  this->horizontalOffset = 0;
  this->topLine = 0;
  this->verticalOverflow = 0;
  this->bottomMargin = 0;
  this->lines.clear();
  this->Refresh();
  this->Invalidate();
}

// OFFSET: 0x00895ED0
// FUNCTION: fn_11512
void UIListBox::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int count = static_cast<int>(this->lines.size());
  const int overflow = std::max((count - visible), 0);

  this->verticalOverflow = overflow != 0;
  this->verticalBar->maximum = overflow != 0 ? (this->bottomMargin + overflow) : 0;
  this->verticalBar->total = count;
  this->verticalBar->visible = visible;
  this->verticalBar->Invalidate();

  if (this->verticalBar->position > this->verticalBar->maximum) {
    this->verticalBar->position = this->verticalBar->maximum;
  }

  this->verticalBar->Invalidate();
}

// OFFSET: 0x008805F0
// FUNCTION: fn_11277
int UIListBox::LineAt(int x, int y) const {
  if (x < 0 || y < 0 || x >= this->geometry.width || y >= this->geometry.height || this->selectable == 0) {
    return -1;
  }

  const int index = (this->topLine + y / this->lineHeight);
  return index >= 0 && static_cast<unsigned>(index) < this->lines.size() ? index : -1;
}

// OFFSET: 0x00890570
// FUNCTION: fn_11418
void UIListBox::Slot25(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
}

// OFFSET: 0x00890180
// FUNCTION: fn_11403
void UIListBox::Slot26(int, int) {
  if (this->selected != -1) {
    static_cast<UIWindow*>(this->parent)->SendMsg(this, 6, this->geometry.id, this->selected, 0, 0);
  }
}

// OFFSET: 0x005C72F0
// FUNCTION: fn_03608
void UIListBox::Slot32(int, int) {
}

// OFFSET: 0x00892E60
// FUNCTION: fn_11496
void UIListBox::Slot35(int value) {
  if (this->verticalOverflow != 0) {
    this->SendMsg(this->verticalBar, 7, value >= 0 ? -3 : 3, 0, 0, 0);
  }
}

// OFFSET: 0x0088B770
// FUNCTION: fn_11366
void UIListBox::Paint() {
  if (this->transparent != 0) {
    if (this->geometry.surface) {
      this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
    }

    if (this->parent) {
      static_cast<UIWindow*>(this->parent)->Invalidate();
    }
  } else if (this->geometry.surface) {
    const std::uint32_t color = (this->backgroundRed & 255) | ((this->backgroundGreen & 255) << 8) | ((this->backgroundBlue & 255) << 16);
    this->geometry.surface->Fill(nullptr, color);
  }

  const int first = this->topLine;
  const int limit = ((this->geometry.height / this->lineHeight - this->bottomMargin) + first);
  const int end = static_cast<int>(std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(limit)));
  std::uint32_t color = this->textColor;
  auto& owner = *this->m_listContext;

  for (int index = first; index < end; ++index) {
    const int row = (index - first);
    int y = (row * this->lineHeight);

    if (index == this->selected) {
      this->FillRegion(0, y, this->geometry.width, (this->lineHeight - 1), this->selectionColor);
    }

    const auto& line = this->lines[index];
    const int x = (this->textInset - this->horizontalOffset);

    if (this->centerVertically == 1) {
      int height = 0;

      if (!line.empty()) {
        TextContext context(*this->geometry.surface, owner.fonts, owner.locale);
        context.SetFont(0, 12, 0, 0);
        height = context.Measure(line.c_str(), static_cast<int>(std::strlen(line.c_str()))).cy;
      }

      y = static_cast<int>(static_cast<float>((this->lineHeight - height)) * 0.5f + static_cast<float>(y));
    }

    DrawColoredText(*this, owner.fonts, owner.locale, owner.codePage, x, y, line.c_str(), static_cast<unsigned>(line.size()), color, 0, 12, false);
  }
}

// OFFSET: 0x00898B00
// FUNCTION: fn_11552
int UIListBox::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  const int value = static_cast<int>(argument1);

  switch (message) {
  case 7:
  case 9:
  case 10: {
    const int previous = this->topLine;
    const int visible = this->geometry.height / this->lineHeight;
    const int delta = message == 9 ? (1 - visible) : message == 10 ? (visible - 1) : value;
    this->topLine = (previous + delta);

    if (this->topLine < 0) {
      this->topLine = 0;
    }

    const int maximum = ((static_cast<int>(this->lines.size()) - visible) + this->bottomMargin);

    if (static_cast<unsigned>(this->topLine) > static_cast<unsigned>(maximum)) {
      this->topLine = maximum;
    }

    this->verticalBar->position = this->topLine;
    this->verticalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent) {
      static_cast<UIWindow*>(this->parent)->Invalidate();
    }

    return (this->topLine - previous);
  }
  case 8:
  case 11:
  case 12: {
    const int previous = this->horizontalOffset;
    const int delta = message == 11 ? -15 : message == 12 ? 15 : value;
    this->horizontalOffset = (previous + delta);

    if (this->horizontalOffset < 0) {
      this->horizontalOffset = 0;
    }

    const int maximum = (((this->verticalOverflow * 16) - this->geometry.width) + this->contentWidth);

    if (this->horizontalOffset > maximum) {
      this->horizontalOffset = maximum;
    }

    this->horizontalBar->position = this->horizontalOffset;
    this->horizontalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent) {
      static_cast<UIWindow*>(this->parent)->Invalidate();
    }

    return (this->horizontalOffset - previous);
  }
  case 18:
    this->selected = (this->selected - 1);

    if (this->selected < 0) {
      this->selected = 0;
    }

    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
    }

    break;
  case 19:
  case 40: {
    this->selected = message == 19 ? (this->selected + 1) : value;

    if (message == 40 && this->selected < 0) {
      this->selected = 0;
    }

    const int last = (static_cast<int>(this->lines.size()) - 1);

    if (static_cast<unsigned>(this->selected) > static_cast<unsigned>(last)) {
      this->selected = last;
    }

    int top = this->topLine;

    if (message == 40 && this->selected < top) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
      top = this->topLine;
    }

    const int edge = message == 19 ? (top - 1) : top;
    const int rows = this->geometry.height / this->lineHeight;
    const int bottom = (edge + (message == 19 ? rows : (rows - 1)));

    if (this->selected > bottom) {
      this->SendMsg(nullptr, 7, 1, 0, 0, 0);
    }

    break;
  }
  case 98:
    if (argument3 > 0) {
      if (UIWindow* window = this->m_listContext->windows.LookupWindow(12)) {
        window->SendMsg(nullptr, 24, argument3, 0, 0, 0);
      }
    }

    return 0;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  this->Refresh();
  return 0;
}
