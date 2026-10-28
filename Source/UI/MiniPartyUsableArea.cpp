#include "UI/MiniPartyGrid.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00A918B0
// FUNCTION: fn_17166
void UIWindowMgr::GetMiniPartyUsableArea(RECT* area) {
  const int height = this->unknown_0480;
  int top = static_cast<int>(height * 0.3f);
  if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
    top = window->geometry.height;
  }

  area->right = this->unknown_047C;
  area->top = top;
  area->left = 0;
  area->bottom = height - g_miniPartyCellHeight * 3;
}
