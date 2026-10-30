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

// OFFSET: 0x008651A0
// FUNCTION: fn_10920
int UISlotBitmap::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    if (argument1 == 203 && this->parent) {
      this->parent->SendMsg(this, 6, 203, this->slot, 0, 0);
    }
    return 0;
  case 13:
    this->geometry.unknown_0030 = argument1;
    return 0;
  default:
    return UIStaticBitmap::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}
