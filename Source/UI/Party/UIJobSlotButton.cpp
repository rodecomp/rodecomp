#include "UI/Party/UIJobSlotButton.h"

// OFFSET: 0x007BAD70
// FUNCTION: fn_09276
UIJobSlotButton::UIJobSlotButton() : UIBitmapButton(), selected(0) {
}

UIJobSlotButton::~UIJobSlotButton() = default;

// OFFSET: 0x007BFC70
// FUNCTION: fn_09307
void UIJobSlotButton::OnLBtnUp(int x, int y) {
  this->UIButton::OnLBtnUp(x, y);
  this->SetSelected(this->selected == 0);
}
