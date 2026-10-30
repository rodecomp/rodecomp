#include "UI/Text/UITagTextViewer.h"

#include <algorithm>

#include "UI/Lists/UIScrollBar.h"

// OFFSET: 0x0083E1B0
UITagTextViewer::UITagTextViewer(UIWindowContext& context)
    : UIScroll(context), m_backgroundColor(0xFFFFFF), m_textWidth(0), m_lineSpacing(6), m_margin(3), m_topMargin(4), m_font{0, 12, 0, 0}, m_outlined(0),
      m_outlineColor(0xFFFFFF), m_horizontalAlign(0), m_verticalAlign(3), m_cursorY(0), m_offsetY(m_topMargin) {
}

// OFFSET: 0x0083E740
void TagTextViewer::UIScroll::StorePosition(UIWindow* child) {
  const float y = static_cast<float>(child->geometry.y);
  const float x = static_cast<float>(child->geometry.x);
  Position& position = this->m_positions[child];
  position.x = x;
  position.y = y;
}

// OFFSET: 0x0083EDE0
int TagTextViewer::UIScroll::ContentHeight() {
  int height = 0;

  for (std::map<UIWindow*, Position>::iterator it = this->m_positions.begin(); it != this->m_positions.end(); ++it) {
    height = static_cast<int>(std::max(static_cast<float>(height), static_cast<float>(it->first->geometry.height) + it->second.y));
  }

  return height;
}

// OFFSET: 0x0083EEF0
// FUNCTION: fn_10525
void UITagTextViewer::Paint() {
  this->ClearSurface(this->m_backgroundColor);

  if (this->m_backgroundColor == 0xFF00FF && this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0083EF20
// FUNCTION: fn_10526
void UITagTextViewer::Slot19() {
  for (UIWindow* child : this->m_children) {
    child->Slot19();
  }
}

// OFFSET: 0x0083EF50
void TagTextViewer::UIScroll::Slot35(int delta) {
  if (delta < 0) {
    this->SendMsg(this->m_scrollBar, 10, 0, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, 9, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0083F140
void UITagTextViewer::Resize(int width, int height) {
  this->m_textWidth += width - this->geometry.width;
  this->m_textWidth = std::max(0, this->m_textWidth);

  UIWindow::Resize(width, height);
  this->m_scrollBar->Resize(this->m_scrollBar->geometry.width, height);
  this->m_scrollBar->Move(width - this->m_scrollBar->geometry.width, 0);
  this->SendMsg(nullptr, 60, 0, 0, 0, 0);
}

// OFFSET: 0x0083F590
void UITagTextViewer::SetAlign(unsigned char align) {
  switch (align) {
  case 0:
  case 1:
  case 2:
    this->m_horizontalAlign = align;
    break;
  case 3:
  case 4:
  case 5:
    this->m_verticalAlign = align;
    break;
  }
}
