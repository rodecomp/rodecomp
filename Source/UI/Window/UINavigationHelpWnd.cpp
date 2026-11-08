#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UINavigationHelpWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowManagerWindows.h"

#include "Resource/MSGSTRINGID.h"
// OFFSET: 0x00921530
// FUNCTION: fn_12642
void UINavigationHelpWnd::OnCreate(int width, int height) {
  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(5, 22, width - 10, height - 27, 0);
  this->m_viewer->backgroundBlue = 0xd2;
  this->m_viewer->backgroundGreen = 0xd2;
  this->m_viewer->backgroundRed = 0xd2;
  this->AddChild(this->m_viewer);

  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP4)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP5)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP6)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP7)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP8)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP9)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP10)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP11)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP12)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP13)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP14)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP15)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP16)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP17)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP18)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_HELP19)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GOT_EXPERIENCE_POINT_INT64)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(" "), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTIFY_EXP_INT64)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARGE_FEE_HELP)), 0x30, 0, 0);

  this->AddCloseButton(LookupMessage(MSI_NAVIGATION_HELP), 0);
}

// OFFSET: 0x0092CEC0
// FUNCTION: fn_12667
void UINavigationHelpWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_NAVIGATION_HELP3), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x009359D0
// FUNCTION: fn_12748
int UINavigationHelpWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == UM_COMMAND && a == ID_CLOSE) {
    g_windowMgr.CloseWindow(WID_NAVIGATIONHELPWND);
  }
  return 0;
}

// OFFSET: 0x009115B0
// FUNCTION: fn_12517
UINavigationHelpWnd::UINavigationHelpWnd()
    : UIFrameWnd(0), m_viewer(nullptr), unknown_00B8(0) {
}
