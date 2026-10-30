#include "UI/Window/UIPartyBookingHelpWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"

// OFFSET: 0x0092D630
// FUNCTION: fn_12670
void UIPartyBookingHelpWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(1716), 0);
  this->DrawPanelBackground(17, 0, "");
}
