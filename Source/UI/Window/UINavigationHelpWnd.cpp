#include "UI/Window/UINavigationHelpWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"

// OFFSET: 0x0092CEC0
// FUNCTION: fn_12667
void UINavigationHelpWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(2240), 0);
  this->DrawPanelBackground(17, 0, "");
}
