#include "UI/Lists/UIComboBoxDefWnd.h"

// OFFSET: 0x0087A180
// FUNCTION: fn_11227
void UINewComboBoxCtrl::Clear() {
  this->items.clear();
  this->selected = 0;
}
