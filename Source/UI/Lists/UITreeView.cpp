#include "UI/Lists/UITreeView.h"

// OFFSET: 0x0086F180
// FUNCTION: fn_11087
UITreeView::UITreeView(UIWindowContext& windows, UIEditContext& edit, UIButtonContext& controls) : UIWindow(windows, 0), m_edit(&edit), m_controls(&controls) {
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
