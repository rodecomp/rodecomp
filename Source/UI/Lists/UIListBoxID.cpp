#include "UI/Lists/UIListBoxID.h"
#include "UI/Window/UIEnums.h"

#include "UI/Lists/UIScrollBar.h"

// OFFSET: 0x0086CEA0
// FUNCTION: fn_11038
UIComboBoxID::UIComboBoxID() {
  this->targetId = 10041;
}

// OFFSET: 0x00871120
// FUNCTION: fn_11106
UIListBoxID::~UIListBoxID() {
}

// OFFSET: 0x0087B5F0
// FUNCTION: fn_11244
void UIComboBoxID::Configure(int x, int y, int width, int height, unsigned char transparent) {
  this->transparent = transparent;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->OnCreate(width, height);
  this->OnSize(width, height);
}

// OFFSET: 0x0087B6E0
// FUNCTION: fn_11247
void UIListBoxID::Configure(int x, int y, int width, int height, unsigned char transparent) {
  this->transparent = transparent;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->OnCreate(width, height);
  this->OnSize(width, height);
}

// OFFSET: 0x00886070
// FUNCTION: fn_11322
void UIComboBoxID::OnCreate(int width, int height) {
  auto* vertical = new UIScrollBar();
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move(width - 13, 0);
  vertical->OnCreate(verticalWidth, height);
  vertical->OnSize(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar();
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize(width - 13, horizontalHeight);
  horizontal->Move(0, height - 13);
  horizontal->OnCreate(width - 13, horizontalHeight);
  horizontal->OnSize(width - 13, horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x008991B0
// FUNCTION: fn_11554
int UIListBoxID::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_SETCURITEM:
    this->selected = argument1;
    if (this->selected < 0) {
      this->selected = 0;
    }
    if (static_cast<std::size_t>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }
    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, UM_SCROLLVERT, -1, 0, 0, 0);
    }
    if (this->selected > this->topLine + this->geometry.height / this->lineHeight - 1) {
      this->SendMsg(nullptr, UM_SCROLLVERT, 1, 0, 0, 0);
    }
    this->Invalidate();
    this->Refresh();
    return 0;
  case UM_UPPUSH:
    --this->selected;
    if (this->selected < 0) {
      this->selected = 0;
    }
    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, UM_SCROLLVERT, -1, 0, 0, 0);
    }
    this->Invalidate();
    this->Refresh();
    return 0;
  case UM_DOWNPUSH:
    ++this->selected;
    if (static_cast<std::size_t>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }
    if (this->selected > this->geometry.height / this->lineHeight + this->topLine - 1) {
      this->SendMsg(nullptr, UM_SCROLLVERT, 1, 0, 0, 0);
    }
    this->Invalidate();
    this->Refresh();
    return 0;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->topLine;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->topLine = previous - this->geometry.height / this->lineHeight + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->topLine = previous - 1 + this->geometry.height / this->lineHeight;
    } else {
      this->topLine = previous + argument1;
    }
    if (this->topLine < 0) {
      this->topLine = 0;
    }
    const std::size_t limit = this->lines.size() + (this->bottomMargin - this->geometry.height / this->lineHeight);
    if (static_cast<std::size_t>(this->topLine) > limit) {
      this->topLine = static_cast<int>(limit);
    }
    {
      auto* verticalBar = this->verticalBar;
      verticalBar->position = this->topLine;
      verticalBar->Invalidate();
    }
    this->Invalidate();
    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }
    if (this->Root()) {
      this->Root()->Invalidate();
    }
    return this->topLine - previous;
  }
  case UM_SCROLLHORZ:
  case UM_SCROLLHORZPAGEUP:
  case UM_SCROLLHORZPAGEDOWN: {
    const int previous = this->horizontalOffset;
    int delta = argument1;
    if (message == UM_SCROLLHORZPAGEUP) {
      delta = -15;
    } else if (message == UM_SCROLLHORZPAGEDOWN) {
      delta = 15;
    }
    this->horizontalOffset = previous + delta;
    if (this->horizontalOffset < 0) {
      this->horizontalOffset = 0;
    }
    const int limit = this->contentWidth + this->verticalOverflow * 16 - this->geometry.width;
    if (this->horizontalOffset > limit) {
      this->horizontalOffset = limit;
    }
    {
      auto* horizontalBar = this->horizontalBar;
      horizontalBar->position = this->horizontalOffset;
      horizontalBar->Invalidate();
    }
    this->Invalidate();
    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }
    return this->horizontalOffset - previous;
  }
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
