#include "UI/Navigation/UIListBoxNavi.h"

#include <algorithm>
#include <string>

// OFFSET: 0x0086DD10
// FUNCTION: fn_11058
UIListBoxNavi::UIListBoxNavi() : UIListBox(), lineColors(), lineOffset(-1) {
  this->defaultLineColor = 0xD2D2D2;
}

// OFFSET: 0x0089BA60
// FUNCTION: fn_11574
void UIListBoxNavi::SetLineOffset(int offset) {
  this->lineOffset = offset;
}

// OFFSET: 0x00875F20
// FUNCTION: fn_45486
void UIListBoxNavi::AddLine(const char* text) {
  this->lines.push_back(text);
  this->lineColors.push_back(0);
  const int width = this->TextWidth(text, 0, 0, 12, 0, 0);
  this->contentWidth = std::max(this->contentWidth, width);
  this->Refresh();
}
