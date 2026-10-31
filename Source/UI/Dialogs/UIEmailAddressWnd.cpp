#include "UI/Dialogs/UIEmailAddressWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x008F9820
// FUNCTION: fn_12341
int UIEmailAddressWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    g_windowMgr.unknown_0474 = static_cast<int>(a);
    if (a == 184) {
      g_modeMgr.gameMode->SendMsg(10012, reinterpret_cast<int>(this->m_edit->Text()), 0, 0, 0);
    }

    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008C7800
// FUNCTION: fn_11939
UIEmailAddressWnd::UIEmailAddressWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIEditContext& edits)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls), m_editContext(&edits) {
}
