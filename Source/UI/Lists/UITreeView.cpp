#include "UI/Lists/UITreeView.h"

// OFFSET: 0x0086F180
// FUNCTION: fn_11087
UITreeView::UITreeView() : UIWindow(0) {
  this->itemPadding = 5;
  this->itemHeight = 20;
  this->itemCount = 0;
  this->verticalBar = nullptr;
  this->verticalOverflow = 0;
  this->scrollRow = 0;
  this->selectedNode = nullptr;
  this->dragNode = nullptr;
  this->dropNode = nullptr;
  this->mouseInside = 0;
  this->highlightColor = 0;
}
