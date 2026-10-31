#include "UI/Dialogs/UIMinimapZoomWnd.h"

// OFFSET: 0x008C8460
// FUNCTION: fn_11954
UIMinimapZoomWnd::UIMinimapZoomWnd()
    : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0),
      m_zoom(),
      unknown_00BC(0),
      unknown_00C0(0),
      unknown_00C4(0) {}
