#include "UI/Buttons/UINoneVisibleButton.h"

// OFFSET: 0x00851A80
// FUNCTION: fn_45650
UINoneVisibleButton::~UINoneVisibleButton() = default;

// OFFSET: 0x0085D460
// FUNCTION: fn_10813
void UINoneVisibleButton::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x0084FF80
// FUNCTION: fn_10669
UINoneVisibleButton::UINoneVisibleButton() : UIButton() {
}
