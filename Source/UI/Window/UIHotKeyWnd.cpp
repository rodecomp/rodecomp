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
// FUNCTION: fn_12513
UIHotKeyWnd::UIHotKeyWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), unknown_00B4(-1), unknown_00B8(18), unknown_00BC(17), unknown_00C0(0), unknown_00C4(20), unknown_00C8(3), unknown_00CC(3),
      unknown_00D0(20), unknown_00D4(80), unknown_00D8(135), unknown_00FC(0), unknown_0104(0), unknown_010C(1), unknown_0110(0), unknown_0114(0) {
}
