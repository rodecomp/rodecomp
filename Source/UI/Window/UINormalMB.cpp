#include "UI/Window/UINormalMB.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00950740
// FUNCTION: fn_12943
int UINormalMB::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    const int id = static_cast<int>(a);
    const auto action = this->m_actions.find(id);

    if (action != this->m_actions.end()) {
      action->second();
    } else {
      if (this->m_owner) {
        this->m_owner->SendMsg(this, 6, 488, id, 0, 0);
      }

      if (this->m_keepOpen) {
        g_windowMgr.unknown_0474 = id;
        return 0;
      }
    }

    g_windowMgr.Schedule(this);
    return 0;
  }

  case 57:
    this->m_viewer->Clear();
    this->m_viewer->SendMsg(nullptr, 29, a, 38, 0, 0);
    return 0;

  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}
