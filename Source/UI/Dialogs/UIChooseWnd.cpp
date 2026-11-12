#include "UI/Dialogs/UIChooseWnd.h"

// OFFSET: 0x008CA740
// FUNCTION: fn_11995
__declspec(noinline) UIChooseWnd::~UIChooseWnd() {
  this->m_items.clear();
}

// OFFSET: 0x008C74C0
// FUNCTION: fn_11932
UIChooseWnd::UIChooseWnd()
    : UIFrameWnd(0), m_list(nullptr), m_okButton(nullptr), m_cancelButton(nullptr) {
  this->m_lastItemIndex = -1;
  this->m_placement = nullptr;
}
