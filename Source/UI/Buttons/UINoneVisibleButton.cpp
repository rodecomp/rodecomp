#include "UI/Buttons/UINoneVisibleButton.h"

// OFFSET: 0x0085D460
// FUNCTION: fn_10813
void UINoneVisibleButton::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x0084FF80
// FUNCTION: fn_10669
UINoneVisibleButton::UINoneVisibleButton(UIWindowContext& windowContext, UIButtonContext& buttonContext) : UIButton(windowContext, buttonContext) {
}
