#include "Client/Input/Language.h"
#include "Contents/SkinMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/UIEditCtrl.h"

// OFFSET: 0x0084F250
// FUNCTION: fn_10657
UIEditCtrl2::UIEditCtrl2(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit) {
}

// OFFSET: 0x00852700
UIEditCtrl2::~UIEditCtrl2() = default;

// OFFSET: 0x00863BF0
// FUNCTION: fn_10911
int UIEditCtrl2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 0) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    InputFocus& focus = this->m_edit->focus;
    if (focus.IsLast(focus.window)) {
      focus.First();
    } else {
      focus.Next();
    }

    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0084F570
// FUNCTION: fn_10659
UIEditCtrl3::UIEditCtrl3(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl2(windows, edit) {
  this->unknown_0130 = 0;
  this->unknown_0134 = 0;
  this->unknown_0138 = 14;
  this->unknown_013C = 0;
  this->unknown_0140 = 0;
  this->unknown_0144 = 0;
  this->unknown_0148 = 0;
  this->unknown_014C = 0;
  this->unknown_0150 = 0;
  this->unknown_0154 = 0;
  this->unknown_0158 = 0;
  this->unknown_015C = 0;
  this->unknown_015E = 0;
  this->unknown_0184 = 0;
  this->unknown_0160 = 0;
  this->unknown_0164 = 0;
}

// OFFSET: 0x00853760
// FUNCTION: fn_10741
void UIEditCtrl_OldList::RecallHistory(int step) {
  if (this->historyIndex == -1) {
    return;
  }

  this->historyIndex += step;
  if (this->historyIndex < 0) {
    this->historyIndex = static_cast<int>(this->history.size()) - 1;
  }

  if (static_cast<std::size_t>(this->historyIndex) >= this->history.size()) {
    this->historyIndex = 0;
  }

  this->SetText(this->history[this->historyIndex].c_str());
  this->Slot23();
  this->Invalidate();
}

// OFFSET: 0x0084F7B0
// FUNCTION: fn_10660
UIEditCtrl4::UIEditCtrl4(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit), placeholderColor(0xFFFFFFFFu), placeholderEnabled(0) {
}

// OFFSET: 0x00852870
UIEditCtrl4::~UIEditCtrl4() = default;

// OFFSET: 0x0085AA40
// FUNCTION: fn_10782
void UIEditCtrl4::Slot23() {
  this->text.clear();
  this->m_edit->input.ResetInput();
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(this->text.c_str());
  this->m_edit->password = this->mask != 0;
  this->SelectAll();
}

// OFFSET: 0x008563D0
// FUNCTION: fn_10754
void UIEditCtrl4::PaintText() {
  this->PaintTextWithPolicy(true, &this->placeholder, this->placeholderColor, this->placeholderEnabled == 1);
}

// OFFSET: 0x00869800
// FUNCTION: fn_10972
void UIEditCtrl4::SetPlaceholderEnabled(bool enabled) {
  this->placeholderEnabled = enabled;
}

// OFFSET: 0x0084F7B0
// FUNCTION: fn_10661
UIEditCtrl5::UIEditCtrl5(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl4(windows, edit) {
}

// OFFSET: 0x008528F0
UIEditCtrl5::~UIEditCtrl5() = default;

// OFFSET: 0x00864140
// FUNCTION: fn_10913
int UIEditCtrl5::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 0) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    InputFocus& focus = this->m_edit->focus;
    if (focus.IsLast(focus.window)) {
      focus.First();
    } else {
      focus.Next();
    }

    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0085CBF0

// OFFSET: 0x0085FF60
// FUNCTION: fn_10892
void UIEditCtrl3::Slot35(int value) {
  if (this->unknown_0134 == 0) {
    return;
  }

  if (value < 0) {
    this->SendMsg(this->unknown_0130, 7, 3, 0, 0, 0);
  } else {
    this->SendMsg(this->unknown_0130, 7, -3, 0, 0, 0);
  }
}

// OFFSET: 0x0085BB60
// FUNCTION: fn_10796
void UIBitmapEditCtrl::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  this->textY = 3;
  this->PaintText();
}

// OFFSET: 0x0085EB50

// OFFSET: 0x0085CDE0

// OFFSET: 0x0085FFC0
// FUNCTION: fn_10894
void UIEditCtrl_OldList::Slot35(int value) {
  if (value < 0) {
    this->SendMsg(nullptr, 19, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, 18, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0085E3D0
// FUNCTION: fn_10827
void UIEditCtrlVarId::Slot24(int) {
  this->Synchronize();
  this->Invalidate();
  this->SendMsg(this, 0, 0, 0, 0, 0);
}

// OFFSET: 0x0085FFA0
// FUNCTION: fn_10893
void UIEditCtrlVarId::Slot35(int value) {
  if (this->parent) {
    this->parent->Slot35(value);
  }
}
