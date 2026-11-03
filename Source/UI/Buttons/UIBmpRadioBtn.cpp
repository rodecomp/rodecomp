#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Lists/UIChatHisBox.h"

// OFFSET: 0x0086C590
// FUNCTION: fn_11028
UIBmpRadioBtn::UIBmpRadioBtn() : UIWindow(0) {
  this->geometry.unknown_0030 = 0;
  this->geometry.id = 215;
  this->m_hovered = false;
  this->m_disabled = false;
}

// OFFSET: 0x0086C720
// FUNCTION: fn_11029
UICaptchaImage::UICaptchaImage() : UIWindow(0) {
}

// OFFSET: 0x00870390
// FUNCTION: fn_11100
UICaptchaImage::~UICaptchaImage() {
  this->m_bitmap.Unload();
}

// OFFSET: 0x008704D0
// FUNCTION: fn_11101
UIChatHisBox::~UIChatHisBox() {
}
