#include "UI/Menu/UIMenuIconWnd.h"

#include "UI/Layout/WindowSnapping.h"

// GLOBAL: gl_24966
static bool s_menuIconExpanded;

// OFFSET: 0x0084A880
// FUNCTION: fn_10613
UIWindow* UIMenuIconWnd::HitTest(int x, int y) {
  if (!this->CanHitTest() || this->geometry.visible == 0 || x < this->geometry.x) {
    return nullptr;
  }

  if (!s_menuIconExpanded) {
    if (x >= this->geometry.x + this->geometry.width || y < this->geometry.y || y >= this->geometry.y + this->unknown_00D0->geometry.height) {
      return nullptr;
    }
    if (this->unknown_00C4.size() == 0) {
      return this->unknown_00CC;
    }
    return this->unknown_00D0;
  }

  if (x >= this->geometry.x + this->geometry.width || y < this->geometry.y || y >= this->geometry.y + this->geometry.height) {
    return nullptr;
  }

  for (auto child = this->m_children.rbegin(); child != this->m_children.rend(); ++child) {
    UIWindow* hit = (*child)->HitTest((x - this->geometry.x), (y - this->geometry.y));
    if (hit) {
      return hit;
    }
  }

  return this;
}

// OFFSET: 0x0084A960
// FUNCTION: fn_10614
bool UIMenuIconWnd::Slot08() {
  return true;
}

// OFFSET: 0x0084A970
// FUNCTION: fn_10615
void UIMenuIconWnd::Move(int x, int y) {
  this->geometry.x = x;
  this->geometry.y = y - 4;
  this->m_frameContext->snapping.Store(this);
}

// OFFSET: 0x0084BD20
// FUNCTION: fn_10618
void UIMenuIconWnd::Slot25(int x, int y) {}

// OFFSET: 0x0084BD30
// FUNCTION: fn_10619
void UIMenuIconWnd::Slot31(int x, int y) {}

// OFFSET: 0x0084C250
// FUNCTION: fn_10621
void UIMenuIconWnd::Slot27(int x, int y) {}
