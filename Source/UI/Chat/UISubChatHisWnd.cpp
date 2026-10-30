#include "UI/Chat/UISubChatHisWnd.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Session.h"

#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009312E0
// FUNCTION: fn_12700
void UISubChatHisWnd::Slot25(int x, int y) {
  if (g_chatInputFlag) {
    if (UIWindow* window = g_windowMgr.LookupWindow(132)) {
      window->SendMsg(this, 6, 193, this->m_index, 0, 2);
    }
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x009309B0
// FUNCTION: fn_12690
void UISubChatHisWnd::Slot26(int x, int y) {
  if (g_chatInputFlag && x > 0 && x < this->m_inputWidth && y > 0 && y < this->m_inputHeight) {
    this->m_edit->SetText("");
    this->m_edit->Resize(this->m_inputWidth - 10, this->m_inputHeight - 2);
    this->m_edit->Move(5, 2);
    InputFocus focus{g_windowMgr.hoverWindow};
    focus.Set(this->m_edit);
  }
}

// OFFSET: 0x00932B70
// FUNCTION: fn_12715
void UISubChatHisWnd::Slot27(int x, int y) {
  if (g_windowMgr.Capture() == this) {
    this->UIFrameWnd::Slot27(x, y);
  }
}

// OFFSET: 0x00931710
// FUNCTION: fn_12706
void UISubChatHisWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);
  if (g_chatInputFlag) {
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);

    UIWindow* window = g_windowMgr.LookupWindow(1);
    if (window && window->geometry.x <= screenX && screenX <= window->geometry.width + window->geometry.x) {
      const int bottom = this->geometry.height + screenY;
      if (window->geometry.y <= bottom && bottom <= window->geometry.height + window->geometry.y) {
        window->SendMsg(this, 118, 0, 0, 0, 0);
      }
    }
  }
}

// OFFSET: 0x0091EA90
// FUNCTION: fn_12633
void UISubChatHisWnd::Move(int x, int y) {
  this->UIFrameWnd::Move(x, y);
  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  this->m_state.unknown_0000 = screenX;
  this->m_state.unknown_0004 = screenY + this->m_inputHeight;
  g_session.m_subChatWnds[this->m_index].x = screenX;
  g_session.m_subChatWnds[this->m_index].y = screenY;
}

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
