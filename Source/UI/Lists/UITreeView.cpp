#include "UI/Lists/UITreeView.h"

// OFFSET: 0x0086F180
// FUNCTION: fn_11087
UITreeView::UITreeView() : UIWindow(0) {
  this->itemPadding = 5;
  this->itemHeight = 20;
  this->itemCount = 0;
  this->verticalBar = nullptr;
  this->verticalOverflow = 0;
  this->unknown_00A8 = 0;
  this->unknown_0088 = nullptr;
  this->unknown_008C = nullptr;
  this->unknown_0090 = nullptr;
  this->unknown_00AC = 0;
  this->unknown_00B0 = 0;
}
