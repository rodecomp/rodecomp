#include "UI/Window/UIHotKeyWnd.h"

#include "UI/Buttons/UIButton.h"

// OFFSET: 0x00932F70
// FUNCTION: fn_12721
void UIHotKeyWnd::Slot32(int, int) {
}

// OFFSET: 0x00933B30
// FUNCTION: fn_12734
void UIHotKeyWnd::UpdatePageButtons() {
  this->m_prevButton->Enable(this->m_page > 1);
  this->m_nextButton->Enable(this->m_page < this->m_pageCount);
  this->Invalidate();
}

// OFFSET: 0x00911300
// FUNCTION: fn_12513
UIHotKeyWnd::UIHotKeyWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_selectedRow(-1), m_rowsPerPage(18), unknown_00BC(17), unknown_00C0(0), unknown_00C4(20), unknown_00C8(3), unknown_00CC(3),
      unknown_00D0(20), unknown_00D4(80), unknown_00D8(135), m_balloon(nullptr), m_tab(0), m_page(1), m_pageCount(0), unknown_0114(0) {
}

// OFFSET: 0x00916760
// FUNCTION: fn_12585
void UIHotKeyWnd::SetHotkey(int tab, int index, HotkeyInfo info) {
  this->hotkeys[tab][index] = info;
}
