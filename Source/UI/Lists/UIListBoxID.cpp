#include "UI/Lists/UIListBoxID.h"

// OFFSET: 0x0087B5F0
// FUNCTION: fn_11244
void UIComboBoxID::Configure(int x, int y, int width, int height, unsigned char transparent) {
  this->transparent = transparent;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x0087B6E0
// FUNCTION: fn_11247
void UIListBoxID::Configure(int x, int y, int width, int height, unsigned char transparent) {
  this->transparent = transparent;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}
