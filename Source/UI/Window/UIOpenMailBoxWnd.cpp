#include "UI/Window/UIOpenMailBoxWnd.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

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
