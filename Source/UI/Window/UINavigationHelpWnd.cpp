#include "UI/Window/UINavigationHelpWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowManagerWindows.h"

// OFFSET: 0x00921530
// FUNCTION: fn_12642
void UINavigationHelpWnd::Slot15(int width, int height) {
  this->m_viewer = new UITextViewer(*this->m_context, *this->m_text);
  this->m_viewer->Configure(5, 22, width - 10, height - 27, 0);
  this->m_viewer->backgroundRed = 0xd2;
  this->m_viewer->backgroundGreen = 0xd2;
  this->m_viewer->backgroundBlue = 0xd2;
  this->AddChild(this->m_viewer);

  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2241)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2242)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2243)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2244)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2245)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2246)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2247)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2248)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2249)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2250)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2251)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2252)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2253)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2254)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2255)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2256)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(3216)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(3217)), 0x30, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(3218)), 0x30, 0, 0);

  this->AddCloseButton(*this->m_controls, LookupMessage(2202), 0);
}

// OFFSET: 0x0092CEC0
// FUNCTION: fn_12667
void UINavigationHelpWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(2240), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x009359D0
// FUNCTION: fn_12748
int UINavigationHelpWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6 && a == 201) {
    this->m_frameContext->windows.CloseWindow(229);
  }
  return 0;
}

// OFFSET: 0x009115B0
// FUNCTION: fn_12517
UINavigationHelpWnd::UINavigationHelpWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UITextViewerContext& text)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls), m_text(&text), m_viewer(nullptr), unknown_00B8(0) {
}
