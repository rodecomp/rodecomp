#include "UI/Dialogs/UIChooseWnd.h"

// OFFSET: 0x008CA740
// FUNCTION: fn_11995
UIChooseWnd::~UIChooseWnd() {
  this->m_items.clear();
}

// OFFSET: 0x008C74C0
// FUNCTION: fn_11932
UIChooseWnd::UIChooseWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIListBoxContext& lists)
    : UIFrameWnd(windows, frames, 0), m_list(nullptr), m_okButton(nullptr), m_cancelButton(nullptr), m_controls(&controls), m_listContext(&lists) {
  this->unknown_00B4 = -1;
  this->m_placement = nullptr;
}
