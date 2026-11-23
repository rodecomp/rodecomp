#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Lists/UIChatHisBox.h"

// OFFSET: 0x0086C590
// FUNCTION: fn_11028
UIBmpRadioBtn::UIBmpRadioBtn() : UIWindow(0), m_names{std::string(), std::string(), std::string(), std::string()} {
  this->geometry.state = 0;
  this->geometry.id = 215;
  this->m_hovered = false;
  this->m_disabled = false;
}

// OFFSET: 0x007C6F80
// FUNCTION: fn_45815
UIBmpRadioBtn2::~UIBmpRadioBtn2() = default;

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

// OFFSET: 0x00871040
// FUNCTION: fn_11105
UIListBox::~UIListBox() = default;
