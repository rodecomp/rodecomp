#include "UI/Items/UIItemParamChangeDisplayWnd.h"

// OFFSET: 0x008C7FC0
// FUNCTION: fn_11947
UIItemParamChangeDisplayWnd::UIItemParamChangeDisplayWnd()
    : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0),
      m_before(0),
      m_after(0),
      m_beforeSecond(0),
      m_afterSecond(0),
      unknown_00C4(0),
      unknown_00C8(0),
      unknown_00CC(0),
      unknown_00D0(0),
      m_label("512"),
      unknown_00EC(0),
      m_placement(nullptr) {}
