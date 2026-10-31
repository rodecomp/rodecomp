#include "UI/Lists/UIListBoxID.h"

#include "UI/Lists/UIScrollBar.h"

// OFFSET: 0x0086CEA0
// FUNCTION: fn_11038
UIComboBoxID::UIComboBoxID() {
  this->targetId = 10041;
}

// OFFSET: 0x0087B5F0
// FUNCTION: fn_11244
void UIComboBoxID::Configure(int x, int y, int width, int height, unsigned char transparent) {
  this->transparent = transparent;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x0087B6E0
// FUNCTION: fn_11247
void UIListBoxID::Configure(int x, int y, int width, int height, unsigned char transparent) {
  this->transparent = transparent;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x008991B0
// FUNCTION: fn_11554
int UIListBoxID::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 40:
    this->selected = argument1;
    if (this->selected < 0) {
      this->selected = 0;
    }
    if (static_cast<std::size_t>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }
    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
    }
    if (this->selected > this->topLine + this->geometry.height / this->lineHeight - 1) {
      this->SendMsg(nullptr, 7, 1, 0, 0, 0);
    }
    this->Invalidate();
    this->Refresh();
    return 0;
  case 18:
    --this->selected;
    if (this->selected < 0) {
      this->selected = 0;
    }
    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
    }
    this->Invalidate();
    this->Refresh();
    return 0;
  case 19:
    ++this->selected;
    if (static_cast<std::size_t>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }
    if (this->selected > this->geometry.height / this->lineHeight + this->topLine - 1) {
      this->SendMsg(nullptr, 7, 1, 0, 0, 0);
    }
    this->Invalidate();
    this->Refresh();
    return 0;
  case 7:
  case 9:
  case 10: {
    const int previous = this->topLine;
    if (message == 9) {
      this->topLine = previous - this->geometry.height / this->lineHeight + 1;
    } else if (message == 10) {
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
    this->verticalBar->position = this->topLine;
    this->verticalBar->Invalidate();
    this->Invalidate();
    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }
    if (this->Root()) {
      this->Root()->Invalidate();
    }
    return this->topLine - previous;
  }
  case 8:
  case 11:
  case 12: {
    const int previous = this->horizontalOffset;
    int delta = argument1;
    if (message == 11) {
      delta = -15;
    } else if (message == 12) {
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
    this->horizontalBar->position = this->horizontalOffset;
    this->horizontalBar->Invalidate();
    this->Invalidate();
    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }
    return this->horizontalOffset - previous;
  }
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
