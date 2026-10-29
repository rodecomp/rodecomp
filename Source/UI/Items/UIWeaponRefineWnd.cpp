#include "UI/Items/UIWeaponRefineWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"

// OFFSET: 0x009A3CD0
// FUNCTION: fn_13613
void UIWeaponRefineWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(910), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}
