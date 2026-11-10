#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIPartyBookingHelpWnd.h"
#include "Client/Session.h"

#include "Client/Input/InputCharset.h"
#include "Contents/Tables/MessageTable.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <string>

// OFFSET: 0x00924810
// FUNCTION: fn_12645
void UIPartyBookingHelpWnd::OnCreate(int width, int height) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(10, 25, width - 20, height - 35, 1);
  this->AddChild(this->m_viewer);

  std::string help;
  const char* result = nullptr;
  if (g_session.m_hotkeyScript->Call("GetPartyBookingHelp", "d>s", -1, &result)) {
    help = result;
    if (help.size()) {
      char* text = new char[help.size() + 4];
      sprintf(text, "%s%s", g_serverLanguage->Prefix(true), help.c_str());
      this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text), 0, 1, 1);
    }
  }
}

// OFFSET: 0x0092D630
// FUNCTION: fn_12670
void UIPartyBookingHelpWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_DESCRIBE_PARTY_BOOKING), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x00938810
// FUNCTION: fn_12751
int UIPartyBookingHelpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (argument1 == 201) {
    g_windowMgr.CloseWindow(WID_PARTYBOOKINGHELPWND);
  }
  return 0;
}

// OFFSET: 0x00911860
// FUNCTION: fn_12519
UIPartyBookingHelpWnd::UIPartyBookingHelpWnd()
    : UIFrameWnd(0) {
}
