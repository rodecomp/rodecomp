#include "UI/Window/UIWindowMgr.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Contents/SkinMgr.h"
#include "Contents/Items/ItemLink.h"
#include "UI/Text/ColoredText.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIMemberListBox.h"
#include "Render/Text/FontCache.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/TextTags.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <mbstring.h>

namespace {}

// OFFSET: 0x0086D940
// FUNCTION: fn_11055
UIListBox::UIListBox() : UIWindow(0) {
  this->textColor = 0;
  this->textInset = 3;
  this->selected = 0;
  this->topLine = 0;
  this->horizontalOffset = 0;
  this->contentWidth = 0;
  this->verticalBar = nullptr;
  this->horizontalBar = nullptr;
  this->verticalOverflow = 0;
  this->bottomMargin = 0;
  this->transparent = 0;
  this->lineHeight = 16;
  this->backgroundRed = 255;
  this->backgroundGreen = 255;
  this->backgroundBlue = 255;
  const SkinPaletteCell cell = {3, 2};
  this->selectionColor = SkinPaletteColor(ResourceMgr::Instance(), cell, g_skinInfo.Selected());
  this->selectable = 1;
  this->centerVertically = 0;
}

// OFFSET: 0x0086E750
// FUNCTION: fn_11076
UIServerListBox::UIServerListBox() : UIListBox(), balloon(nullptr) {
}

// OFFSET: 0x00871040
// FUNCTION: fn_11105
UIListBox::~UIListBox() = default;

// OFFSET: 0x0087BB00
// FUNCTION: fn_11251
void UIScrollBar::Configure(int x, int y, int length, int vertical, int arrowLength, int thickness) {
  this->vertical = vertical;
  this->arrowLength = arrowLength;
  this->thickness = thickness;

  int width = 0;
  int height = 0;

  switch (vertical) {
  case 0:
    width = length;
    height = thickness;
    break;
  case 1:
    width = thickness;
    height = length;
    break;
  }

  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

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
  auto* vertical = new UIScrollBar();
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move((width - 13), 0);
  vertical->Slot15(verticalWidth, height);
  vertical->Slot22(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar();
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize((width - 13), horizontalHeight);
  horizontal->Move(0, (height - 13));
  horizontal->Slot15((width - 13), horizontalHeight);
  horizontal->Slot22((width - 13), horizontalHeight);
  this->AddChild(this->horizontalBar);
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

// OFFSET: 0x008750D0
// FUNCTION: fn_11197
void UIChatHisBox::AddTextWrapped(const char* text, int color, const char* style) {
  std::vector<std::string> lines;
  const int width = this->verticalBar ? (this->geometry.width - 16) : this->geometry.width;

  if (g_navigationLinkParser->HasLink(std::string(text))) {
    this->AddText(text, color, style);
    return;
  }

  this->WrapPixelText(text, lines, width);

  for (unsigned index = 0; index < lines.size(); ++index) {
    const std::size_t length = std::strlen(lines[index].c_str());
    char* copy = new char[length + 1];
    std::strcpy(copy, lines[index].c_str());
    copy[length] = 0;
    this->AddText(copy, color, style);
    delete[] copy;
  }
}

// OFFSET: 0x008752F0
// FUNCTION: fn_11198
void UIChatHisBox3::AddText(const char* text, int color) {
  const bool atBottom =
    this->verticalOverflow == 0 || this->topLine == ((static_cast<int>(this->lines.size()) - this->geometry.height / this->lineHeight) + this->bottomMargin);

  if (this->lines.size() >= this->maxLines) {
    this->ShiftNaviLinks();
    this->ShiftItemTags();
    this->DropOldestLines();
  }

  this->unknown_00E4.push_back(color);

  int mode = 3;
  const auto* bytes = reinterpret_cast<const unsigned char*>(text);

  if (_mbsstr(bytes, reinterpret_cast<const unsigned char*>(g_itemLinkTagBegin.c_str())) || _mbsstr(bytes, reinterpret_cast<const unsigned char*>(g_itemLinkTagEnd.c_str()))) {
    this->AddItemLinkLine(text, color, mode);
  } else if (g_navigationLinkParser->HasLink(std::string(text))) {
    this->AddNavigationLinkLine(text, color, mode);
  } else {
    this->UIListBox::AddLine(text);
  }

  if (this->verticalOverflow != 0 && atBottom && this->unknown_0130 != 0) {
    this->topLine = (static_cast<int>(this->lines.size()) - this->geometry.height / this->lineHeight) + this->bottomMargin;
    this->verticalBar->position = this->topLine;
    this->verticalBar->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x00875520
// FUNCTION: fn_11199
void UIChatHisBox::AddText(const char* text, int color, const char* style) {
  const bool atBottom =
    this->verticalOverflow == 0 || this->topLine == ((static_cast<int>(this->lines.size()) - this->geometry.height / this->lineHeight) + this->bottomMargin);

  if (this->lines.size() >= this->maxLines) {
    this->ShiftNaviLinks();
    this->ShiftItemTags();
    this->DropOldestLines();
  }

  this->history.push_back(style ? std::string(style) : std::string());
  this->unknown_00E4.push_back(color);

  int mode = 4;
  const auto* bytes = reinterpret_cast<const unsigned char*>(text);

  if (_mbsstr(bytes, reinterpret_cast<const unsigned char*>(g_itemLinkTagBegin.c_str())) || _mbsstr(bytes, reinterpret_cast<const unsigned char*>(g_itemLinkTagEnd.c_str()))) {
    this->AddItemLinkLine(text, color, mode);
  } else if (g_navigationLinkParser->HasLink(std::string(text))) {
    this->AddNavigationLinkLine(text, color, mode);
  } else {
    this->UIListBox::AddLine(text);
  }

  if (this->verticalOverflow != 0 && atBottom) {
    this->topLine = (static_cast<int>(this->lines.size()) - this->geometry.height / this->lineHeight) + this->bottomMargin;
    this->verticalBar->position = this->topLine;
    this->verticalBar->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x00875A70
// FUNCTION: fn_11201
void UIListBox::AddLine(const char* text) {
  this->lines.push_back(std::string(text));
  const int width = this->TextWidth(text, 0, 0, 12, 0, 0);
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
  const int overflow = std::max(0, (static_cast<int>(this->lines.size()) - visible));

  this->verticalOverflow = overflow != 0;
  UIScrollBar* bar = this->verticalBar;
  bar->maximum = overflow != 0 ? (this->bottomMargin + overflow) : 0;
  bar->total = static_cast<int>(this->lines.size());
  bar->visible = visible;
  bar->Invalidate();

  if (bar->position > bar->maximum) {
    bar->position = bar->maximum;
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
    this->parent->SendMsg(this, 6, this->geometry.id, this->selected, 0, 0);
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
    if (value < 0) {
      this->SendMsg(this->verticalBar, 7, 3, 0, 0, 0);
    } else {
      this->SendMsg(this->verticalBar, 7, -3, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0088B770
// FUNCTION: fn_11366
void UIListBox::Paint() {
  if (this->transparent != 0) {
    this->ClearSurface(0xFFFF00FFu);

    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface((this->backgroundRed & 255) | ((this->backgroundGreen & 255) << 8) | ((this->backgroundBlue & 255) << 16));
  }

  const int first = this->topLine;
  const int limit = ((this->geometry.height / this->lineHeight - this->bottomMargin) + first);
  const int end = static_cast<int>(std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(limit)));
  std::uint32_t color = this->textColor;

  for (int index = first; index < end; ++index) {
    const int row = (index - first);

    if (index == this->selected) {
      this->FillRegion(0, (row * this->lineHeight), this->geometry.width, (this->lineHeight - 1), this->selectionColor);
    }

    const int x = (this->textInset - this->horizontalOffset);
    int y = (row * this->lineHeight);

    if (this->centerVertically == 1) {
      const int height = this->TextHeight(this->lines[index].c_str(), 0, 0, 12, 0, 0);
      y = static_cast<int>(static_cast<float>((this->lineHeight - height)) * 0.5f + static_cast<float>((row * this->lineHeight)));
    }

    DrawColoredText(*this, x, y, this->lines[index].c_str(), static_cast<unsigned>(this->lines[index].size()), color, 0, 12, false);
  }
}

// OFFSET: 0x00898B00
// FUNCTION: fn_11552
int UIListBox::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  const int value = static_cast<int>(argument1);

  switch (message) {
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

  case 18:
    this->selected = (this->selected - 1);

    if (this->selected < 0) {
      this->selected = 0;
    }

    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
    }

    break;

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
      this->parent->Invalidate();
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
      this->parent->Invalidate();
    }

    return (this->horizontalOffset - previous);
  }

  case 98:
    if (argument3 > 0) {
      if (UIWindow* window = g_windowMgr.LookupWindow(12)) {
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

// OFFSET: 0x00880D90
// FUNCTION: fn_11291
bool UIListBox::CopyText(char* buffer, int length) {
  if (!buffer) {
    return false;
  }

  std::string text;
  const int count = static_cast<int>(this->lines.size());

  for (int index = 0; index < count; ++index) {
    text += this->lines[index].c_str();
  }

  if (length == 0) {
    length = this->TextLength();
  }

  std::memcpy(buffer, text.c_str(), length);
  return true;
}

// OFFSET: 0x00880F60
// FUNCTION: fn_11292
int UIListBox::TextLength() {
  int length = 0;
  const int count = static_cast<int>(this->lines.size());

  for (int index = 0; index < count; ++index) {
    length += static_cast<int>(this->lines[index].size());
  }

  return length;
}

// OFFSET: 0x0087AAF0
// FUNCTION: fn_11238
void UIChatHisBox::CopyHistory(const UIChatHisBox& other) {
  this->unknown_0100 = other.unknown_0100;
  this->unknown_010C = other.unknown_010C;
  this->unknown_0118 = other.unknown_0118;
  this->RebuildHistory();
}

// OFFSET: 0x00875860
// FUNCTION: fn_11200
void UIComboBoxID::AddLine(const char* text, int id, int data) {
  const int index = static_cast<int>(this->lines.size());
  this->lines.emplace(index, IdListLine{id, text});
  const int width = this->TextWidth(text, 0, 0, 12, 0, 0);
  this->contentWidth = std::max(this->contentWidth, width);
  this->Refresh();

  if (data != 0) {
    this->unknown_00C4.try_emplace(index, data);
  }
}

// OFFSET: 0x00875DD0
// FUNCTION: fn_11203
void UIListBoxID::AddLine(const char* text, int id) {
  this->lines.emplace(static_cast<int>(this->lines.size()), IdListLine{id, text});
  const int width = this->TextWidth(text, 0, 0, 12, 0, 0);
  this->contentWidth = std::max(this->contentWidth, width);
  this->Refresh();
}

// OFFSET: 0x0086D8C0
// FUNCTION: fn_11054
UIListBox2::UIListBox2() : UIListBox(), fontFamily(0), fontHeight(12), bold(0), italic(0) {
}
