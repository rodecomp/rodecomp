#include "UI/UICaptchaRegisterWnd.h"

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/WindowBorder.h"

// OFFSET: 0x007EA8D0
// FUNCTION: fn_09789
void UICaptchaRegisterWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFFFF);
  this->DrawTitleBar(controls.fonts, controls.locale, true, "", 0);
  DrawRectangleBorder(*this, 34, 80, 222, 92, 0);
}

// OFFSET: 0x007EAD90
// FUNCTION: fn_09791
void UICaptchaRegisterWnd::SetRegisterDisabled(unsigned char disabled) {
  this->registerButton->SetDisabled(disabled);
  this->registerButton->Invalidate();
  this->Invalidate();
}
