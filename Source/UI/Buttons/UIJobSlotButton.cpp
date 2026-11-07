#include "UI/Buttons/UIJobSlotButton.h"

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

// OFFSET: 0x007C1C50
// FUNCTION: fn_09318
void UIJobSlotButton::SetSelected(unsigned char value) {
  this->selected = value;
  this->SetState(0);

  if (value == 1) {
    this->SetBitmap(this->selectedImage.c_str(), 0, 0);
    this->SetBitmap(this->selectedImage.c_str(), 1, 0);
    this->SetBitmap(this->unselectedImage.c_str(), 2, 0);
  } else {
    this->SetBitmap(this->unselectedImage.c_str(), 0, 0);
    this->SetBitmap(this->unselectedImage.c_str(), 1, 0);
    this->SetBitmap(this->selectedImage.c_str(), 2, 0);
  }

  this->Invalidate();
}
