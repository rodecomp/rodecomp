#include "UI/Items/UIItemRepairWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"

// OFFSET: 0x009A3860
// FUNCTION: fn_13609
void UIItemRepairWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(812), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}
