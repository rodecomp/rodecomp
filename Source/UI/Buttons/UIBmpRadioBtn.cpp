#include "UI/Buttons/UIBmpRadioBtn.h"

// OFFSET: 0x0086C590
// FUNCTION: fn_11028
UIBmpRadioBtn::UIBmpRadioBtn(UIWindowContext& windows, UIButtonContext& controls) : UIWindow(windows, 0), m_controls(&controls) {
  this->geometry.unknown_0030 = 0;
  this->geometry.id = 215;
  this->m_hovered = false;
  this->m_disabled = false;
}

// OFFSET: 0x0086C720
// FUNCTION: fn_11029
UICaptchaImage::UICaptchaImage(UIWindowContext& windows, BitmapResourceContext& bitmaps, UIButtonContext& controls)
    : UIWindow(windows, 0), m_bitmap(bitmaps), m_controls(&controls) {
}
