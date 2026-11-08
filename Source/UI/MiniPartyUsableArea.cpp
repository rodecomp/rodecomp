#include "UI/Window/WindowId.h"
#include "UI/MiniPartyGrid.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00A918B0
// FUNCTION: fn_17166
RECT UIWindowMgr::GetMiniPartyUsableArea() {
  const int height = this->screenHeight;
  int top = static_cast<int>(height * 0.3f);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
    top = window->geometry.height;
  }

  RECT area;
  area.right = this->screenWidth;
  area.top = top;
  area.left = 0;
  area.bottom = height - g_miniPartyCellHeight * 3;
  return area;
}
