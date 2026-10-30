#include "UI/Window/UINoteWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/WindowBorder.h"

// OFFSET: 0x0094D870
// FUNCTION: fn_12907
void UINoteWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFF);
  DrawRoundedBorder(*this, 2, 2, this->geometry.width - 4, this->geometry.height - 4, 0xC8C8C8);
  this->FillRegion(8, 8, this->geometry.width - 16, this->geometry.height - 38, 0xF7F7F7);
}

// OFFSET: 0x0094D8E0
// FUNCTION: fn_12908
void UINoteWnd_Scroll::Paint() {
  this->ClearSurface(0xF7F7F7);
}

// OFFSET: 0x0094EE80
// FUNCTION: fn_12919
void UINoteWnd_Scroll::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x + 23, y + 27);
  }
}

// OFFSET: 0x0094EF80
// FUNCTION: fn_12924
void UINoteWnd_Scroll::Slot31(int x, int y) {
  if (this->parent) {
    this->parent->Slot31(x + 23, y + 27);
  }
}

// OFFSET: 0x0094F2C0
// FUNCTION: fn_12935
void UINoteWnd_Scroll::Slot35(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, 7, 12, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, 7, -12, 0, 0, 0);
    }
  }
}
