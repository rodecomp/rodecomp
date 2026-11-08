#include "UI/UISlotBitmap.h"
#include "UI/Window/UIEnums.h"
#include "UI/Buttons/UIBitmapButton.h"

// OFFSET: 0x0085ADA0
// FUNCTION: fn_10789
void UISlotBitmap::OnCreate(int, int) {
  if (this->showAddButton == 1) {
    this->addButton = new UIBitmapButton();
    this->addButton->SetBitmap("유저인터페이스\\select_character\\btn_add_out.bmp", 0, 0);
    this->addButton->SetBitmap("유저인터페이스\\select_character\\btn_add_over.bmp", 1, 0);
    this->addButton->SetBitmap("유저인터페이스\\select_character\\btn_add_press.bmp", 2, 0);
    this->addButton->SetSize(this->addButton->BitmapWidth(), this->addButton->BitmapHeight());
    this->addButton->Move(
      static_cast<int>((this->geometry.width - this->addButton->geometry.width) * 0.5),
      static_cast<int>((this->geometry.height - this->addButton->geometry.height) * 0.5)
    );
    this->addButton->SetId(203);
    this->AddChild(this->addButton);
  }
}

// OFFSET: 0x0085E980
// FUNCTION: fn_10833
void UISlotBitmap::OnLBtnDblClk(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, UM_COMMAND, ID_OK, 0, 0, 0);
  }
}

// OFFSET: 0x0085ED30
// FUNCTION: fn_10846
void UISlotBitmap::OnLBtnDown(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, UM_SETSTATE, this->slot, 0, 0, 0);
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, 0, 0, 0);
  }
}

// OFFSET: 0x0085FE60
// FUNCTION: fn_10887
void UISlotBitmap::OnRBtnDown(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, UM_SETSTATE, this->slot, 0, 0, 0);
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, 226, 0, 0);
  }
}

// OFFSET: 0x008651A0
// FUNCTION: fn_10920
int UISlotBitmap::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    if (argument1 == ID_MAKE && this->parent) {
      this->parent->SendMsg(this, UM_COMMAND, ID_MAKE, this->slot, 0, 0);
    }
    return 0;
  case UM_SETSTATE:
    this->geometry.state = argument1;
    return 0;
  default:
    return UIStaticBitmap::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x008501D0
// FUNCTION: fn_10672
UISlotBitmap::UISlotBitmap(int slot) : UIStaticBitmap() {
  this->slot = slot;
  this->showAddButton = 0;
  this->addButton = nullptr;
}

// OFFSET: 0x00850240
// FUNCTION: fn_10673
UISlotBitmap::UISlotBitmap(int slot, unsigned char flag) : UIStaticBitmap() {
  this->slot = slot;
  this->showAddButton = flag;
  this->addButton = nullptr;
}
