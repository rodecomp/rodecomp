#include "UI/Window/UIAccountLimitedToolWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"

// OFFSET: 0x0094CF50
// FUNCTION: fn_12901
void UIAccountLimitedToolWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, false, LookupMessage(3650), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawTextAt(controls.fonts, controls.locale, 5, 28, LookupMessage(3651), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 50, LookupMessage(3652), 0, 0, 12, 0, 0, 0);
  this->FillRegion(3, 70, this->geometry.width - 6, 2, 0x99938A);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 80, LookupMessage(3653), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 102, LookupMessage(3654), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 117, 102, "/", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 152, 102, "/", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 187, 102, "(YY/MM/DD)", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 296, 102, ":", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 335, 102, "(HH:MM)", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 124, LookupMessage(3655), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 117, 124, "/", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 152, 124, "/", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 187, 124, "(YY/MM/DD)", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 296, 124, ":", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 335, 124, "(HH:MM)", 0, 0, 12, 0, 0, 0);
  this->FillRegion(3, 144, this->geometry.width - 6, 2, 0x99938A);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 154, LookupMessage(3916), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 172, 154, "(DD)", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 278, 154, ":", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 322, 154, "(HH:MM)", 0, 0, 12, 0, 0, 0);
  this->FillRegion(3, 174, this->geometry.width - 6, 2, 0x99938A);
}
