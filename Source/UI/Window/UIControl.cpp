#include "UI/Window/UIControl.h"

// INLINED
CUIControl::CUIControl(UIWindowContext& context) : UIWindow(context, 0), hitEnabled(1) {
}

// OFFSET: 0x009C08C0
CUIControl::~CUIControl() = default;

// OFFSET: 0x009D5490
// FUNCTION: fn_14047
bool CUIControl::CanHitTest() {
  return this->hitEnabled != 0;
}

// OFFSET: 0x009EEAD0
// FUNCTION: fn_14634
void CUIControl::Slot10(int x, int y, std::intptr_t value) {
  if (this->parent) {
    this->parent->Slot10(x + this->geometry.x, y + this->geometry.y, value);
  }
}

// OFFSET: 0x009EF680
// FUNCTION: fn_14639
void CUIControl::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF650
// FUNCTION: fn_14638
void CUIControl::Slot26(int x, int y) {
  if (this->parent) {
    this->parent->Slot26(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF710
// FUNCTION: fn_14642
void CUIControl::Slot27(int x, int y) {
  if (this->parent) {
    this->parent->Slot27(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF6E0
// FUNCTION: fn_14641
void CUIControl::Slot28(int x, int y) {
  if (this->parent) {
    this->parent->Slot28(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF740
// FUNCTION: fn_14643
void CUIControl::ResetCursor(int x, int y) {
  if (this->parent) {
    this->parent->ResetCursor(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF6B0
// FUNCTION: fn_14640
void CUIControl::Slot31(int x, int y) {
  if (this->parent) {
    this->parent->Slot31(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF7A0
// FUNCTION: fn_14645
void CUIControl::Slot32(int x, int y) {
  if (this->parent) {
    this->parent->Slot32(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF7D0
// FUNCTION: fn_14646
void CUIControl::Slot33(int x, int y) {
  if (this->parent) {
    this->parent->Slot33(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF770
// FUNCTION: fn_14644
void CUIControl::Slot34(int x, int y) {
  if (this->parent) {
    this->parent->Slot34(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF800
// FUNCTION: fn_14647
void CUIControl::Slot35(int value) {
  if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x009EF5E0
// FUNCTION: fn_14636
void CUIControl::Invalidate() {
  this->Root()->InvalidateChildren();
}

// OFFSET: 0x009EF820
// FUNCTION: fn_14648
void CUIControl::Slot52(int value) {
  this->Slot53(value);

  for (UIWindow* child : this->m_children) {
    child->Slot52(value);
  }
}

// OFFSET: 0x009D3FC0
// FUNCTION: fn_14036
void CUIControl::Slot53(int) {
}
