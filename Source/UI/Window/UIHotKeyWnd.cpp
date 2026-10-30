#include "UI/Window/UIHotKeyWnd.h"

#include "UI/Buttons/UIButton.h"

// OFFSET: 0x00932F70
// FUNCTION: fn_12721
void UIHotKeyWnd::Slot32(int, int) {
}

// OFFSET: 0x00933B30
// FUNCTION: fn_12734
void UIHotKeyWnd::UpdatePageButtons() {
  this->m_prevButton->Enable(this->unknown_010C > 1);
  this->m_nextButton->Enable(this->unknown_010C < this->unknown_0110);
  this->Invalidate();
}

// OFFSET: 0x00911300
