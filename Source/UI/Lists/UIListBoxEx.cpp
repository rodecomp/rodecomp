#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/ColoredText.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIListBoxEx.h"
#include "UI/Lists/UIScrollBar.h"

#include <algorithm>

// OFFSET: 0x0086DA90
// FUNCTION: fn_11056
UIListBoxEx::UIListBoxEx(UIWindowContext& windows, UIListBoxContext& list)
    : UIWindow(windows, 0), backgroundRed(255), backgroundGreen(255), backgroundBlue(255), selected(0), topLine(0), horizontalOffset(0), contentWidth(0),
      verticalOverflow(0), bottomMargin(0), verticalBar(nullptr), horizontalBar(nullptr), transparent(0), lineHeight(16), selectionColor(0),
      m_listContext(&list) {
  constexpr char path[] = "유저인터페이스\\colorchip.bmp";
  const auto* chip = static_cast<CBitmapRes*>(list.resources.Load(path));

  if (chip) {
    const auto& image = chip->image;
    const std::uint32_t pixel = image.width > 10 && image.height > 10 ? image.pixels[10 + 10 * image.width] : 0x00FF0000u;
    this->selectionColor = ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  }
}

// OFFSET: 0x00873710
// FUNCTION: fn_11157
UIListBoxEx::~UIListBoxEx() = default;

// OFFSET: 0x0087B690
// FUNCTION: fn_11246
void UIListBoxEx::Configure(int x, int y, int width, int height, unsigned char value) {
  this->transparent = value;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x00886990
// FUNCTION: fn_11327
void UIListBoxEx::Slot15(int width, int height) {
  auto& owner = *this->m_listContext;
  auto* vertical = new UIScrollBar(*this->m_context, owner.capture, owner.resources);
  this->verticalBar = vertical;
  const int thickness = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(thickness, height);
  vertical->Move((width - 13), 0);
  vertical->Slot15(thickness, height);
  vertical->Slot22(thickness, height);
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

// OFFSET: 0x00897750
// FUNCTION: fn_11533
void UIListBoxEx::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->verticalBar->Resize(12, height);
  this->verticalBar->Move((width - 13), 0);
  this->horizontalBar->Resize((width - 13), 12);
  this->horizontalBar->Move(0, (height - 13));
  this->Refresh();
}

// OFFSET: 0x00875BE0
// FUNCTION: fn_11202
void UIListBoxEx::AddLine(const char* text, std::uint32_t color) {
  this->lines.emplace(static_cast<int>(this->lines.size()), ExtendedListLine{text, color});
  auto& owner = *this->m_listContext;
  const int width = this->TextWidth(owner.fonts, owner.locale, text, 0, 0, 12, 0, 0);
  this->contentWidth = std::max(this->contentWidth, width);
  this->Refresh();
}

// OFFSET: 0x0087A010
// FUNCTION: fn_11224
void UIListBoxEx::Clear() {
  this->selected = -1;
  this->contentWidth = 0;
  this->horizontalOffset = 0;
  this->topLine = 0;
  this->verticalOverflow = 0;
  this->bottomMargin = 0;
  this->lines.clear();
}

// OFFSET: 0x00895FC0
// FUNCTION: fn_11513
void UIListBoxEx::Refresh() {
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

// INLINED
int UIListBoxEx::LineAt(int x, int y) const {
  if (x < 0 || y < 0 || x >= this->geometry.width || y >= this->geometry.height) {
    return -1;
  }

  const int index = (this->topLine + y / this->lineHeight);
  return index >= 0 && static_cast<unsigned>(index) < this->lines.size() ? index : -1;
}

// OFFSET: 0x008905A0
// FUNCTION: fn_11419
void UIListBoxEx::Slot25(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
}

// OFFSET: 0x008901B0
// FUNCTION: fn_11404
void UIListBoxEx::Slot26(int, int) {
  if (this->selected != -1) {
    this->parent->SendMsg(this, 6, this->geometry.id, this->selected, 0, 0);
  }
}

// OFFSET: 0x00892EA0
// FUNCTION: fn_11497
void UIListBoxEx::Slot35(int value) {
  if (this->verticalOverflow != 0) {
    this->SendMsg(this->verticalBar, 7, value >= 0 ? -3 : 3, 0, 0, 0);
  }
}

// OFFSET: 0x0088B920
// FUNCTION: fn_11367
void UIListBoxEx::Paint() {
  if (this->transparent != 0) {
    this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);

    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    const unsigned color = static_cast<unsigned char>(this->backgroundRed) | (static_cast<unsigned char>(this->backgroundGreen) << 8) |
                           (static_cast<unsigned char>(this->backgroundBlue) << 16);
    this->geometry.surface->Fill(nullptr, color);
  }

  const unsigned end = std::min(
    static_cast<unsigned>(this->lines.size()), static_cast<unsigned>((this->topLine + (this->geometry.height / this->lineHeight - this->bottomMargin)))
  );
  auto& owner = *this->m_listContext;

  for (int index = this->topLine; index < static_cast<int>(end); index = (index + 1)) {
    const int y = (this->lineHeight * (index - this->topLine));

    if (index == this->selected) {
      this->FillRegion(0, y, this->geometry.width, (this->lineHeight - 1), this->selectionColor);
    }

    auto& line = this->lines[index];
    auto color = line.color;
    DrawColoredText(
      *this, owner.fonts, owner.locale, owner.codePage, (3 - this->horizontalOffset), y, line.text.c_str(), static_cast<unsigned>(line.text.size()), color, 0,
      12, false
    );
  }
}

// OFFSET: 0x00898EB0
// FUNCTION: fn_11553
int UIListBoxEx::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case 7:
  case 9:
  case 10: {
    const int previous = this->topLine;
    const int visible = this->geometry.height / this->lineHeight;
    const int delta = message == 9 ? (1 - visible) : message == 10 ? (visible - 1) : value;
    this->topLine = std::max((previous + delta), 0);
    const int maximum = ((static_cast<int>(this->lines.size()) - visible) + this->bottomMargin);

    if (static_cast<unsigned>(this->topLine) > static_cast<unsigned>(maximum)) {
      this->topLine = maximum;
    }

    this->verticalBar->position = this->topLine;
    this->verticalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }

    UIWindow* root = this;

    while (root->parent) {
      root = root->parent;
    }

    root->Invalidate();
    return (this->topLine - previous);
  }
  case 8:
  case 11:
  case 12: {
    const int previous = this->horizontalOffset;
    const int delta = message == 11 ? -15 : message == 12 ? 15 : value;
    this->horizontalOffset = std::max((previous + delta), 0);
    const int maximum = (((16 * this->verticalOverflow) - this->geometry.width) + this->contentWidth);

    if (this->horizontalOffset > maximum) {
      this->horizontalOffset = maximum;
    }

    this->horizontalBar->position = this->horizontalOffset;
    this->horizontalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }

    return (this->horizontalOffset - previous);
  }
  case 18:
    this->selected = std::max((this->selected - 1), 0);

    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
    }

    break;
  case 19:
  case 40: {
    this->selected = message == 19 ? (this->selected + 1) : std::max(value, 0);
    const int last = (static_cast<int>(this->lines.size()) - 1);

    if (static_cast<unsigned>(this->selected) > static_cast<unsigned>(last)) {
      this->selected = last;
    }

    int top = this->topLine;

    if (message == 40 && this->selected < top) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
      top = this->topLine;
    }

    const int bottom = ((top + this->geometry.height / this->lineHeight) - 1);

    if (this->selected > bottom) {
      this->SendMsg(nullptr, 7, 1, 0, 0, 0);
    }

    break;
  }
  default:
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  this->Invalidate();
  this->Refresh();
  return 0;
}
