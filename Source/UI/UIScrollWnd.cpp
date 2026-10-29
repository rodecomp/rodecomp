#include "UI/UIScrollWnd.h"
#include "UI/Lists/UIScrollBar.h"

// OFFSET: 0x009AC2B0
// FUNCTION: fn_13772
UIScrollWnd::~UIScrollWnd() {
  if (this->horizontalBar) {
    this->RemoveChild(this->horizontalBar, 0);
    delete this->horizontalBar;
    this->horizontalBar = nullptr;
  }
  if (this->scrollBar) {
    this->RemoveChild(this->scrollBar, 0);
    delete this->scrollBar;
    this->scrollBar = nullptr;
  }
}

// OFFSET: 0x009AD3A0
// FUNCTION: fn_13783
void UIScrollWnd::Slot15(int width, int height) {
  this->horizontalBar = new UIScrollBar(*this->m_context);
  this->horizontalBar->Configure(0, height - this->scrollBarHeight, 0, width - this->scrollBarWidth);
  this->AddChild(this->horizontalBar);
  this->scrollBar = new UIScrollBar(*this->m_context);
  this->scrollBar->Configure(width - this->scrollBarWidth, 0, 1, height - this->scrollBarHeight);
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x009AD4A0
// FUNCTION: fn_13784
void UIScrollWnd::Slot35(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, 7, 3, 0, 0, 0);
      return;
    }
    this->SendMsg(this->scrollBar, 7, -3, 0, 0, 0);
  }
}

// OFFSET: 0x009AD790
// FUNCTION: fn_13786
void UIScrollWnd::Clear() {
  this->unknown_009C = 0;
  this->scrollPosition = 0;
  this->childOrigins.clear();
  this->DeleteChildren();
}

// OFFSET: 0x009AD7C0
// FUNCTION: fn_13787
void UIScrollWnd::RemoveChild(UIWindow* child, unsigned char destroy) {
  if (this->horizontalBar && this->scrollBar) {
    this->childOrigins.erase(child);
  }
  UIWindow::RemoveChild(child, destroy);
}
