#include "UI/Chat/UISubChatHisWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009312E0

// OFFSET: 0x009309B0

// OFFSET: 0x00931710

// OFFSET: 0x0091C9E0
// FUNCTION: fn_12619
bool UISubChatHisWnd::Slot42(void* output) {
  *static_cast<SubChatHistoryState*>(output) = this->m_state;
  return true;
}

// OFFSET: 0x00941020
// FUNCTION: fn_12788
void UISubChatHisWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
  }
}
