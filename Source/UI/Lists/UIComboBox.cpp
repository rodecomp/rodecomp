#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>

template __declspec(noinline) std::map<int, int>::iterator std::map<int, int>::_Mybase::find(const int&);

// OFFSET: 0x00897C20
// FUNCTION: fn_11543
int UIComboBox::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  const int value = static_cast<int>(argument1);

  switch (message) {
  case UM_DEFPUSH:
    if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
      target->SendMsg(nullptr, UM_COMBOBOX_SELECTED, this->selected, 0, 0, 0);
    }

    if (g_windowMgr.comboBoxWnd != nullptr) {
      g_windowMgr.CloseWindow(WID_COMBOBOXWND);
    }

    return 0;
  case UM_UPPUSH:
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
  case UM_DOWNPUSH:
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
  case UM_ADDITEM:
    this->AddLine(reinterpret_cast<const char*>(argument1));
    this->Invalidate();
    return 0;
  case UM_SET_CALLER_WID:
    this->targetId = value;
    return 0;
  case UM_SETCURITEM:
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
  case UM_DEFPUSH:
    if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
      int extra = 0;
      const auto found = this->lineData.find(this->selected);

      if (found != this->lineData.end()) {
        extra = found->second;
      }

      const int lineId = this->lines[this->selected].id;
      target->SendMsg(nullptr, UM_COMBOBOX_SELECTED, lineId, extra, 0, 0);
    }

    if (g_windowMgr.comboBoxWnd != nullptr) {
      g_windowMgr.CloseWindow(WID_COMBOBOXWND);
    }

    return 0;
  case UM_UPPUSH:
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
  case UM_DOWNPUSH:
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
  case UM_ADDITEM:
    this->AddLine(reinterpret_cast<const char*>(argument1), static_cast<int>(argument2), static_cast<int>(argument3));
    this->Invalidate();
    return 0;
  case UM_SET_CALLER_WID:
    this->targetId = value;
    return 0;
  case UM_SETCURITEM:
    this->selected = value;
    if (value < 0) {
      this->selected = 0;
    }

    if (static_cast<unsigned>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }

    break;
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
