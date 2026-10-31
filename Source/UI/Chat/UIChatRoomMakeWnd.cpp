#include "UI/Chat/UIChatRoomMakeWnd.h"

#include "Contents/MessageTable.h"

// OFFSET: 0x008C7280
// FUNCTION: fn_11929
UIChatRoomMakeWnd::UIChatRoomMakeWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->m_title = LookupMessage(125);
}

// OFFSET: 0x008C7120
// FUNCTION: fn_11928
UIChatRoomChangeWnd::UIChatRoomChangeWnd(UIWindowContext& windows, UIFrameContext& frames) : UIChatRoomMakeWnd(windows, frames) {
  this->m_title = LookupMessage(126);
}

// OFFSET: 0x008CA5F0
// FUNCTION: fn_11994
UIChatRoomMakeWnd::~UIChatRoomMakeWnd() = default;
