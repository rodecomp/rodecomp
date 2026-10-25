#include "Client/Input/TextInputBuffer.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/UIEditCtrl.h"

// OFFSET: 0x0084F250
UIEditCtrl2::UIEditCtrl2(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit) {
}

// OFFSET: 0x00852700
// FUNCTION: fn_10714
UIEditCtrl2::~UIEditCtrl2() = default;

// OFFSET: 0x00863BF0
// FUNCTION: fn_10911
int UIEditCtrl2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 0) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    this->m_edit->focus.Advance();
    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0084F7B0
UIEditCtrl4::UIEditCtrl4(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit), placeholderColor(0xFFFFFFFFu), placeholderEnabled(0) {
}

// OFFSET: 0x00852870
UIEditCtrl4::~UIEditCtrl4() = default;

// OFFSET: 0x0085AA40
// FUNCTION: fn_10782
void UIEditCtrl4::Slot23() {
  this->text.clear();
  this->m_edit->input.CancelComposition();
  this->m_edit->input.CancelComposition();
  this->m_edit->input.Insert(this->text.c_str());
  this->m_edit->password = this->mask != 0;
  this->SelectAll();
}

// OFFSET: 0x008563D0
// FUNCTION: fn_10754
void UIEditCtrl4::PaintText() {
  this->PaintTextWithPolicy(true, &this->placeholder, this->placeholderColor, this->placeholderEnabled == 1);
}

// OFFSET: 0x0084F7B0
UIEditCtrl5::UIEditCtrl5(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl4(windows, edit) {
}

// OFFSET: 0x008528F0
// FUNCTION: fn_10717
UIEditCtrl5::~UIEditCtrl5() = default;

// OFFSET: 0x00864140
// FUNCTION: fn_10913
int UIEditCtrl5::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 0) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    this->m_edit->focus.Advance();
    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}
