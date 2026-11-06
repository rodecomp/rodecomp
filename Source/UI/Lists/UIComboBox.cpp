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

  switch (message) {
  case 0:
    if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
      target->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
    }

    if (g_windowMgr.unknown_0214 != nullptr) {
      g_windowMgr.CloseWindow(26);
    }

    return 0;
  case 18:
    if (this->lines.size() == 0) {
      return 0;
    }

    --this->selected;
    if (this->selected < 0) {
      this->selected = 0;
    }

    if (this->selected < this->topLine) {
      this->topLine = this->selected;
    }

    break;
  case 19:
    if (this->lines.size() == 0) {
      return 0;
    }

    ++this->selected;
    if (static_cast<unsigned>(this->selected) >= this->lines.size()) {
      this->selected = static_cast<int>(this->lines.size() - 1);
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
  case 83:
    this->targetId = value;
    return 0;
  case 40:
    this->selected = value;
    if (value < 0) {
      this->selected = 0;
    }

    if (static_cast<unsigned>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }

    break;
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

    if (g_windowMgr.unknown_0214 != nullptr) {
      g_windowMgr.CloseWindow(26);
    }

    return 0;
  case 18:
    if (this->lines.size() == 0) {
      return 0;
    }

    --this->selected;
    if (this->selected < 0) {
      this->selected = 0;
    }

    if (this->selected < this->topLine) {
      this->topLine = this->selected;
    }

    break;
  case 19:
    if (this->lines.size() == 0) {
      return 0;
    }

    ++this->selected;
    if (static_cast<unsigned>(this->selected) >= this->lines.size()) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }

    {
      const int edge = (this->bottomMargin - this->geometry.height / this->lineHeight) + this->selected;

      if (this->topLine <= edge) {
        this->topLine = edge + 1;
      }
    }

    break;
  case 31:
    this->AddLine(reinterpret_cast<const char*>(argument1), static_cast<int>(argument2), static_cast<int>(argument3));
    this->Invalidate();
    return 0;
  case 40:
    this->selected = value;
    if (value < 0) {
      this->selected = 0;
    }

    if (static_cast<unsigned>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }

    break;
  case 83:
    this->targetId = value;
    return 0;
  default:
    return UIListBoxID::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Refresh();
  this->Invalidate();
  return 0;
}

// OFFSET: 0x0086CB40
// FUNCTION: fn_11033
UIComboBox::UIComboBox() : UIListBox(), targetId(10041) {
}
