#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIEnums.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Contents/Skin/SkinMgr.h"
#include "UI/Text/ColoredText.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIListBoxEx.h"
#include "UI/Lists/UIScrollBar.h"

#include <algorithm>

// OFFSET: 0x0086DA90
// FUNCTION: fn_11056
UIListBoxEx::UIListBoxEx()
    : UIWindow(0), backgroundRed(255), backgroundGreen(255), backgroundBlue(255), selected(0), topLine(0), horizontalOffset(0), contentWidth(0),
      verticalOverflow(0), bottomMargin(0), verticalBar(nullptr), horizontalBar(nullptr), transparent(0), lineHeight(16), selectionColor(0) {
  const SkinPaletteCell cell = {2, 2};
  this->selectionColor = SkinPaletteColor(cell, g_skinInfo.Selected());
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
  this->OnCreate(width, height);
  this->OnSize(width, height);
}

// OFFSET: 0x00886990
// FUNCTION: fn_11327
void UIListBoxEx::OnCreate(int width, int height) {
  auto* vertical = new UIScrollBar();
  this->verticalBar = vertical;
  const int thickness = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(thickness, height);
  vertical->Move((width - 13), 0);
  vertical->OnCreate(thickness, height);
  vertical->OnSize(thickness, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar();
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize((width - 13), horizontalHeight);
  horizontal->Move(0, (height - 13));
  horizontal->OnCreate((width - 13), horizontalHeight);
  horizontal->OnSize((width - 13), horizontalHeight);
  this->AddChild(this->horizontalBar);
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
  const int width = this->TextWidth(text, 0, 0, 12, 0, 0);
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
  int overflow = count - visible;

  if (overflow < 0) {
    overflow = 0;
  }

  this->verticalOverflow = overflow != 0;
  auto* bar = this->verticalBar;

  if (overflow == 0) {
    bar->maximum = 0;
  } else {
    bar->maximum = this->bottomMargin + overflow;
  }

  bar->total = count;
  bar->visible = visible;
  bar->Invalidate();

  if (bar->position > bar->maximum) {
    bar->position = bar->maximum;
  }

  bar->Invalidate();
}

// OFFSET: 0x008905A0
// FUNCTION: fn_11419
void UIListBoxEx::OnLBtnDown(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
}

// OFFSET: 0x008901B0
// FUNCTION: fn_11404
void UIListBoxEx::OnLBtnDblClk(int, int) {
  if (this->selected != -1) {
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, this->selected, 0, 0);
  }
}

// OFFSET: 0x00892EA0
// FUNCTION: fn_11497
void UIListBoxEx::OnWheel(int value) {
  if (this->verticalOverflow != 0) {
    if (value < 0) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, 3, 0, 0, 0);
      return;
    }

    this->SendMsg(this->verticalBar, UM_SCROLLVERT, -3, 0, 0, 0);
  }
}

// OFFSET: 0x0088B920
// FUNCTION: fn_11367
void UIListBoxEx::Paint() {
  if (this->transparent != 0) {
    this->ClearSurface(0xFFFF00FF);

    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(RGB(this->backgroundRed, this->backgroundGreen, this->backgroundBlue));
  }

  const unsigned end = std::min(
    static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(this->topLine + (this->geometry.height / this->lineHeight - this->bottomMargin))
  );

  for (int index = this->topLine; index < static_cast<int>(end); ++index) {
    const int y = this->lineHeight * (index - this->topLine);

    if (index == this->selected) {
      this->FillRegion(0, y, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    std::uint32_t color = this->lines[index].color;
    this->DrawColoredText(
      3 - this->horizontalOffset, y, this->lines[index].text.c_str(), static_cast<unsigned>(this->lines[index].text.size()), color, 0, 12,
      false
    );
  }
}

// OFFSET: 0x00898EB0
// FUNCTION: fn_11553
int UIListBoxEx::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case UM_SETCURITEM:
    this->selected = value;

    if (this->selected < 0) {
      this->selected = 0;
    }

    if (static_cast<unsigned>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size()) - 1;
    }

    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, UM_SCROLLVERT, -1, 0, 0, 0);
    }

    if (this->selected > this->topLine + this->geometry.height / this->lineHeight - 1) {
      this->SendMsg(nullptr, UM_SCROLLVERT, 1, 0, 0, 0);
    }

    break;

  case UM_UPPUSH:
    if (--this->selected < 0) {
      this->selected = 0;
    }

    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, UM_SCROLLVERT, -1, 0, 0, 0);
    }

    break;

  case UM_DOWNPUSH:
    ++this->selected;

    if (static_cast<unsigned>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size()) - 1;
    }

    if (this->selected > this->topLine + this->geometry.height / this->lineHeight - 1) {
      this->SendMsg(nullptr, UM_SCROLLVERT, 1, 0, 0, 0);
    }

    break;

  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->topLine;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->topLine = previous - this->geometry.height / this->lineHeight + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->topLine = previous - 1 + this->geometry.height / this->lineHeight;
    } else {
      this->topLine = previous + value;
    }

    if (this->topLine < 0) {
      this->topLine = 0;
    }
    if (this->topLine > this->lines.size() - this->geometry.height / this->lineHeight + this->bottomMargin) {
      this->topLine = this->lines.size() - this->geometry.height / this->lineHeight + this->bottomMargin;
    }

    this->verticalBar->position = this->topLine;
    this->verticalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }

    if (this->Root()) {
      this->Root()->Invalidate();
    }
    return (this->topLine - previous);
  }

  case UM_SCROLLHORZ:
  case UM_SCROLLHORZPAGEUP:
  case UM_SCROLLHORZPAGEDOWN: {
    const int previous = this->horizontalOffset;
    const int delta = message == UM_SCROLLHORZPAGEUP ? -15 : message == UM_SCROLLHORZPAGEDOWN ? 15 : value;
    this->horizontalOffset = previous + delta;

    if (this->horizontalOffset < 0) {
      this->horizontalOffset = 0;
    }

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

  default:
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  this->Invalidate();
  this->Refresh();
  return 0;
}
