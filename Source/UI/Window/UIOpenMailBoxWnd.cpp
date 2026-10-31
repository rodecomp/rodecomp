#include "UI/Window/UIOpenMailBoxWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

// OFFSET: 0x0094F020
// FUNCTION: fn_12927
void UIOpenMailBoxWnd::Slot53() {
}

// OFFSET: 0x0094C410
// FUNCTION: fn_12896
void UIOpenMailBoxWnd::Slot15(int width, int height) {
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_noti_gotmail.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_noti_gotmail.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_noti_gotmail.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->SetId(192);
  this->AddChild(button);
}

// OFFSET: 0x0094D8F0
// FUNCTION: fn_12909
void UIOpenMailBoxWnd::Paint() {
}

// OFFSET: 0x00950A60
// FUNCTION: fn_12945
int UIOpenMailBoxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  if (a != 192) {
    return 0;
  }

  if (g_windowMgr.LookupWindow(295)) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2992)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (g_windowMgr.LookupWindow(343)) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3715)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (g_windowMgr.LookupWindow(10006)) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3843)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (g_windowMgr.LookupWindow(361)) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4054)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (!this->m_frameContext->windows.CloseWindow(263)) {
    this->m_frameContext->windows.EnsureWindow(263);
  }

  return 0;
}

// OFFSET: 0x00946DE0
// FUNCTION: fn_12853
UIOpenMailBoxWnd::UIOpenMailBoxWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}
