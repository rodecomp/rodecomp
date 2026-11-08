#include "UI/Items/UIItemParamChangeDisplayWnd.h"

// OFFSET: 0x008C7FC0
// FUNCTION: fn_11947
UIItemParamChangeDisplayWnd::UIItemParamChangeDisplayWnd()
    : UIFrameWnd(0),
      m_before(0),
      m_after(0),
      m_beforeSecond(0),
      m_afterSecond(0),
      itemParam(0),
      itemSecondParam(0),
      equippedParam(0),
      equippedSecondParam(0),
      m_label("512"),
      itemIndex(0),
      m_placement(nullptr) {}
