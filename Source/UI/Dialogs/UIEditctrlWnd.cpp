#include "UI/Dialogs/UIEditctrlWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"

// OFFSET: 0x0099C530
// FUNCTION: fn_13558
void UIEditctrlWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2755), 0);
}
