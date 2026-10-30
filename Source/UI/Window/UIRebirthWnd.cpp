#include "UI/Window/UIRebirthWnd.h"

#include "Client/Mode/ModeMgr.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00938E30
// FUNCTION: fn_12753
int UIRebirthWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case 185:
    g_windowMgr.CloseWindow(129);
    break;
  case 220:
    mode->SendMsg(25, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(129);
    break;
  case 251:
    g_windowMgr.CloseWindow(129);
    mode->SendMsg(88, 0, 0, 0, 0);
    break;
  case 252:
    mode->SendMsg(25, 1, 0, 0, 0);
    g_windowMgr.CloseWindow(129);
    g_windowMgr.CloseWindow(269);
    break;
  case 353:
    mode->SendMsg(250, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(129);
    break;
  }
  return 0;
}
