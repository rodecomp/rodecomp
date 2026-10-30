#include "UI/Window/UINewSkillListWnd.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009B3E70
// FUNCTION: fn_13826
void UINewSkillListWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}

// OFFSET: 0x009B2C60
// FUNCTION: fn_13821
void UINewSkillListWnd::Slot35(int delta) {
  UIWindow* window = g_windowMgr.LookupWindow(46);
  if (window && window->geometry.visible) {
    static_cast<UINewSkillListWnd*>(window)->m_scrollBar->Slot35(delta);
    return;
  }

  if (this->m_scrollable) {
    int step = this->m_visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, 7, step, 0, 0, 0);
  }
}
