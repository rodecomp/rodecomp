#include "UI/Window/UIPartyBookingHelpWnd.h"

#include "Client/Input/InputCharset.h"
#include "Contents/MessageTable.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>
#include <string>

// OFFSET: 0x00924810
// FUNCTION: fn_12645
void UIPartyBookingHelpWnd::Slot15(int width, int height) {
  auto& controls = *this->m_controls;
  this->AddBaseButton(controls);
  this->AddCloseButton(controls, "", 0);

  this->m_viewer = new UITextViewer(*this->m_context, *this->m_text);
  this->m_viewer->Configure(10, 25, width - 20, height - 35, 1);
  this->AddChild(this->m_viewer);

  std::string help;
  const char* result = nullptr;
  if (g_luaScript->Call("GetPartyBookingHelp", "d>s", -1, &result)) {
    help = result;
    if (help.size()) {
      char* text = new char[help.size() + 4];
      sprintf(text, "%s%s", g_serverLanguage->Prefix(true), help.c_str());
      this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text), 0, 1, 1);
    }
  }
}

// OFFSET: 0x0092D630
// FUNCTION: fn_12670
void UIPartyBookingHelpWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(1716), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x00938810
// FUNCTION: fn_12751
int UIPartyBookingHelpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (argument1 == 201) {
    g_windowMgr.CloseWindow(173);
  }
  return 0;
}
