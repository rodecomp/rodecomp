#include "UI/Login/UIEmailAddressWnd.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x008F9820
// FUNCTION: fn_12341
int UIEmailAddressWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  g_windowMgr.modalResponse = static_cast<int>(a);
  if (a == 184) {
    CMode* mode = g_modeMgr.gameMode;
    mode->SendMsg(LMM_SET_EMAIL_ADDRESS, reinterpret_cast<int>(this->m_edit->Text()), 0, 0, 0);
  }

  return 0;
}

// OFFSET: 0x008C7800
// FUNCTION: fn_11939
UIEmailAddressWnd::UIEmailAddressWnd()
    : UIFrameWnd(0) {
}
