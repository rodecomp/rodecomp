#include "UI/Lists/UIListBoxNavi.h"

// OFFSET: 0x0086DD10
// FUNCTION: fn_11058
UIListBoxNavi::UIListBoxNavi(UIWindowContext& windows, UIListBoxContext& list) : UIListBox(windows, list), lineColors(), lineOffset(-1) {
  this->defaultLineColor = 0xD2D2D2;
}

// OFFSET: 0x0089BA60
// FUNCTION: fn_11574
void UIListBoxNavi::SetLineOffset(int offset) {
  this->lineOffset = offset;
}
