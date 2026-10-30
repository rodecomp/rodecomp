#include "UI/Window/UINavigationHelpWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/WindowManagerWindows.h"

// OFFSET: 0x0092CEC0
// FUNCTION: fn_12667
void UINavigationHelpWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(2240), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x009359D0
// FUNCTION: fn_12748
int UINavigationHelpWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6 && a == 201) {
    this->m_frameContext->windows.CloseWindow(229);
  }
  return 0;
}
