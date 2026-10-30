#include "UI/Window/UIPartyBookingHelpWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0092D630
// FUNCTION: fn_12670
void UIPartyBookingHelpWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(1716), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x00938810
// FUNCTION: fn_12751
int UIPartyBookingHelpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (argument1 == 201) {
    g_windowMgr.CloseWindow(173);
  }
  return 0;
}
