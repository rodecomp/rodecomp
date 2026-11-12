#include "UI/Items/UIItemParamChangeDisplayWnd.h"

// OFFSET: 0x008C7FC0
// FUNCTION: fn_11947
UIItemParamChangeDisplayWnd::UIItemParamChangeDisplayWnd()
    : UIFrameWnd(0) {
  this->itemParam = 0;
  this->itemSecondParam = 0;
  this->equippedParam = 0;
  this->equippedSecondParam = 0;
  this->m_after = 0;
  this->m_afterSecond = 0;
  this->m_before = 0;
  this->m_beforeSecond = 0;
  this->m_placement = nullptr;
  this->itemIndex = 0;
  this->m_label = "512";
}
