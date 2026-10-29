#include "UI/UISlotBitmap.h"

// OFFSET: 0x0085E980
// FUNCTION: fn_10833
void UISlotBitmap::Slot26(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, 6, 184, 0, 0, 0);
  }
}

// OFFSET: 0x0085ED30
// FUNCTION: fn_10846
void UISlotBitmap::Slot25(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, 13, this->slot, 0, 0, 0);
    this->parent->SendMsg(this, 6, this->geometry.id, 0, 0, 0);
  }
}

// OFFSET: 0x0085FE60
// FUNCTION: fn_10887
void UISlotBitmap::Slot32(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, 13, this->slot, 0, 0, 0);
    this->parent->SendMsg(this, 6, this->geometry.id, 226, 0, 0);
  }
}
