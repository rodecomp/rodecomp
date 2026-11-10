#include "UI/Window/UIOpenMailBoxWnd.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include "Resource/MSGSTRINGID.h"
// OFFSET: 0x0094F020
// FUNCTION: fn_12927
void UIOpenMailBoxWnd::UpdateMailIcon() {
}

// OFFSET: 0x0094C410
// FUNCTION: fn_12896
void UIOpenMailBoxWnd::OnCreate(int width, int height) {
  auto* button = new UIBitmapButton();
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
  if (message != UM_COMMAND) {
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  if (a != 192) {
    return 0;
  }

  if (g_windowMgr.LookupWindow(WID_REFININGWND)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_AURA_EFFECT_OFF)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (g_windowMgr.LookupWindow(WID_GRADEENCHANTWND)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BROADCASTING_GRADE_ENCHANT_SUCCESS)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (g_windowMgr.LookupWindow(WID_ENCHANTUI)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ENCHANT_REQUEST)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (g_windowMgr.LookupWindow(WID_RUNESYSTEMWND)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_CLOSE_BANKING)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (!g_windowMgr.CloseWindow(WID_RODEXINBOXWND)) {
    g_windowMgr.EnsureWindow(WID_RODEXINBOXWND);
  }

  return 0;
}

// OFFSET: 0x00946DE0
// FUNCTION: fn_12853
UIOpenMailBoxWnd::UIOpenMailBoxWnd()
    : UIFrameWnd(0) {
}
