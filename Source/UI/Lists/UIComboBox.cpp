#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>

// GLOBAL: gl_24685
int g_closeChatAfterGroups = 0x039E2580;

// OFFSET: 0x00897C20
// FUNCTION: fn_11543
int UIComboBox::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  const int value = static_cast<int>(argument1);
  const unsigned count = static_cast<unsigned>(this->lines.size());

  switch (message) {
  case 0:
    if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
      target->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
    }

    if (g_closeChatAfterGroups != 0) {
      g_windowMgr.CloseWindow(26);
    }

    return 0;
  case 18:
    if (count == 0) {
      return 0;
    }

    this->selected = std::max(this->selected - 1, 0);

    if (this->selected < this->topLine) {
      this->topLine = this->selected;
    }

    break;
  case 19:
    if (count == 0) {
      return 0;
    }

    this->selected = this->selected + 1;

    if (static_cast<unsigned>(this->selected) >= count) {
      this->selected = static_cast<int>(count - 1);
    }

    {
      const int edge = (this->bottomMargin - this->geometry.height / this->lineHeight) + this->selected;

      if (this->topLine <= edge) {
        this->topLine = edge + 1;
      }
    }

    break;
  case 31:
    this->AddLine(reinterpret_cast<const char*>(argument1));
    this->Invalidate();
    return 0;
  case 40:
    this->selected = std::max(value, 0);

    if (static_cast<unsigned>(this->selected) > count - 1) {
      this->selected = static_cast<int>(count - 1);
    }

    break;
  case 83:
    this->targetId = value;
    return 0;
  default:
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Refresh();
  this->Invalidate();
  return 0;
}

// OFFSET: 0x00898280
// FUNCTION: fn_11546
int UIComboBoxID::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  const int value = static_cast<int>(argument1);
  const unsigned count = static_cast<unsigned>(this->lines.size());

  switch (message) {
  case 0:
    if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
      int extra = 0;
      const auto found = this->unknown_00C4.find(this->selected);

      if (found != this->unknown_00C4.end()) {
        extra = found->second;
      }

      const int lineId = this->lines[this->selected].id;
      target->SendMsg(nullptr, 39, lineId, extra, 0, 0);
    }

    if (g_closeChatAfterGroups != 0) {
      g_windowMgr.CloseWindow(26);
    }

    return 0;
  case 18:
    if (count == 0) {
      return 0;
    }

    this->selected = std::max(this->selected - 1, 0);

    if (this->selected < this->topLine) {
      this->topLine = this->selected;
    }

    break;
  case 19:
    if (count == 0) {
      return 0;
    }

    this->selected = this->selected + 1;

    if (static_cast<unsigned>(this->selected) >= count) {
      this->selected = static_cast<int>(count - 1);
    }

    {
      const int edge = (this->bottomMargin - this->geometry.height / this->lineHeight) + this->selected;

      if (this->topLine <= edge) {
        this->topLine = edge + 1;
      }
    }

    break;
  case 31:
    this->AddIndexedLine(reinterpret_cast<const char*>(argument1), argument2, argument3);
    this->Invalidate();
    return 0;
  case 40:
    this->selected = std::max(value, 0);

    if (count - 1 < static_cast<unsigned>(this->selected)) {
      this->selected = static_cast<int>(count - 1);
    }

    break;
  case 83:
    this->targetId = value;
    return 0;
  default:
    return this->sub_8991B0(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Refresh();
  this->Invalidate();
  return 0;
}

void UIComboBoxID::AddIndexedLine(const char* text, std::intptr_t, std::intptr_t value) {
  const int index = static_cast<int>(this->lines.size());
  IdListLine& created = this->lines[index];
  created.text = text;
  this->Refresh();

  if (value != 0 && this->unknown_00C4.find(index) == this->unknown_00C4.end()) {
    this->unknown_00C4.emplace(index, static_cast<int>(value));
  }
}

int UIListBoxID::sub_8991B0(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  const int value = static_cast<int>(argument1);

  switch (message) {
  case 7:
  case 9:
  case 10: {
    const int previous = this->topLine;
    const int visible = this->geometry.height / this->lineHeight;
    const int delta = message == 9 ? (1 - visible) : message == 10 ? (visible - 1) : value;
    int top = previous + delta;

    if (top < 0) {
      top = 0;
    }

    const int maximum = static_cast<int>(this->lines.size()) + (this->bottomMargin - visible);

    if (static_cast<unsigned>(maximum) < static_cast<unsigned>(top)) {
      top = maximum;
    }

    this->topLine = top;
    this->verticalBar->position = top;
    this->verticalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent != nullptr) {
      this->parent->Invalidate();
    }

    if (UIWindow* root = this->Root()) {
      root->Invalidate();
    }

    return top - previous;
  }
  case 8:
  case 11:
  case 12: {
    const int previous = this->horizontalOffset;
    const int delta = message == 11 ? -15 : message == 12 ? 15 : value;
    int offset = previous + delta;

    if (offset < 0) {
      offset = 0;
    }

    const int maximum = this->contentWidth + (this->verticalOverflow * 16 - this->geometry.width);

    if (offset > maximum) {
      offset = maximum;
    }

    this->horizontalOffset = offset;
    this->horizontalBar->position = offset;
    this->horizontalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent != nullptr) {
      this->parent->Invalidate();
    }

    if (UIWindow* root = this->Root()) {
      root->Invalidate();
    }

    return offset - previous;
  }
  case 18:
    this->selected = this->selected - 1;

    if (this->selected < 0) {
      this->selected = 0;
    }

    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
    }

    break;
  case 19:
    this->selected = this->selected + 1;

    if (this->lines.size() - 1 < static_cast<unsigned>(this->selected)) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }

    if (this->selected > this->geometry.height / this->lineHeight + this->topLine - 1) {
      this->SendMsg(nullptr, 7, 1, 0, 0, 0);
    }

    break;
  case 40:
    this->selected = value;

    if (this->selected < 0) {
      this->selected = 0;
    }

    if (this->lines.size() - 1 < static_cast<unsigned>(this->selected)) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }

    if (this->selected < this->topLine) {
      this->SendMsg(nullptr, 7, -1, 0, 0, 0);
    }

    if (this->selected > this->geometry.height / this->lineHeight - 1 + this->topLine) {
      this->SendMsg(nullptr, 7, 1, 0, 0, 0);
    }

    break;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  this->Refresh();
  return 0;
}

// OFFSET: 0x0086CB40
// FUNCTION: fn_11033
UIComboBox::UIComboBox(UIWindowContext& windows, UIListBoxContext& list) : UIListBox(windows, list), targetId(10041) {
}
