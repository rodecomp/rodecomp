#include "UI/Chat/UISubChatHisWnd.h"

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
