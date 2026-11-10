#include "UI/Chat/UIChatRoomMakeWnd.h"

#include "Contents/Tables/MessageTable.h"

#include "Resource/MSGSTRINGID.h"
// OFFSET: 0x008C7280
// FUNCTION: fn_11929
UIChatRoomMakeWnd::UIChatRoomMakeWnd() : UIFrameWnd(0) {
  this->m_title = LookupMessage(MSI_MAKING_ROOM);
}

// OFFSET: 0x008C7120
// FUNCTION: fn_11928
UIChatRoomChangeWnd::UIChatRoomChangeWnd() : UIChatRoomMakeWnd() {
  this->m_title = LookupMessage(MSI_CHANGE_ROOM_SETTING);
}

// OFFSET: 0x008CA5F0
// FUNCTION: fn_11994
UIChatRoomMakeWnd::~UIChatRoomMakeWnd() = default;
