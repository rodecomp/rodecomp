#include "UI/Lists/UIComboBoxDefWnd.h"

// OFFSET: 0x0086DFE0
// FUNCTION: fn_11063
UINewComboBoxCtrl::UINewComboBoxCtrl()
    : UIWindow(*reinterpret_cast<UIWindowContext*>(nullptr), 0), button(nullptr), label(nullptr), borderless(0), dropped(false), red(255), green(255), blue(255), borderRed(0), borderGreen(0), borderBlue(0), selected(0), items() {
}

// OFFSET: 0x0087A180
// FUNCTION: fn_11227
void UINewComboBoxCtrl::Clear() {
  this->items.clear();
  this->selected = 0;
}