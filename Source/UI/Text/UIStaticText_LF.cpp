#include "UI/Text/UIStaticText_LF.h"

// OFFSET: 0x0094EEA0
// FUNCTION: fn_12920
void UIStaticText_LF::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x, y);
  }
}

// OFFSET: 0x0094EFA0
// FUNCTION: fn_12925
void UIStaticText_LF::Slot31(int x, int y) {
  if (this->parent) {
    this->parent->Slot31(x, y);
  }
}

// OFFSET: 0x0094F300
// FUNCTION: fn_12936
void UIStaticText_LF::Slot35(int delta) {
  if (this->parent) {
    this->parent->Slot35(delta);
  }
}
