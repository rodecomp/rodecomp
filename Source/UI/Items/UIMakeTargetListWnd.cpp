#include "UI/Items/UIMakeTargetListWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"

// OFFSET: 0x009A38A0
// FUNCTION: fn_13610
void UIMakeTargetListWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(425), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}
