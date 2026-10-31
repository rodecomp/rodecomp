#include "UI/Window/UIShowWarpWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <string>

// OFFSET: 0x0094CA20

// OFFSET: 0x0094ECE0
// FUNCTION: fn_12914
void UIShowWarpWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x00951210
// FUNCTION: fn_12949
int UIShowWarpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    this->m_recorder->Record(this->geometry.id, static_cast<int>(argument1), argument2, argument3, argument4);
    if (argument1 == 201) {
      g_windowMgr.CloseWindow(0x134);
    }
    return 0;
  case 0x12:
    this->m_list->SendMsg(nullptr, 0x12, 0, 0, 0, 0);
    return 0;
  case 0x13:
    this->m_list->SendMsg(nullptr, 0x13, 0, 0, 0, 0);
    return 0;
  case 0x22: {
    const char* name = reinterpret_cast<const char*>(argument1);
    if (std::string(name) == "Random") {
      this->m_list->AddLine(LookupMessage(0xD4));
    } else if (std::string(name) == "cancel") {
      this->m_list->AddLine(LookupMessage(0xDA));
    } else {
      this->m_list->AddLine(g_session.GetMapDisplayName(g_session.GatName2RswName(name)));
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}
