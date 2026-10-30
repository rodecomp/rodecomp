#include "UI/Window/UINoteWnd.h"
#include "UI/Lists/UIScrollBar.h"

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
