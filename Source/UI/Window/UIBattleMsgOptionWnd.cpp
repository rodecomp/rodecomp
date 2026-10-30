#include "UI/Window/UIBattleMsgOptionWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Lists/UIScrollBar.h"

// OFFSET: 0x009335E0
// FUNCTION: fn_12726
void UIBattleMsgOptionWnd::Slot35(int delta) {
  if (this->m_scrollable != 0) {
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

// OFFSET: 0x00910C40
