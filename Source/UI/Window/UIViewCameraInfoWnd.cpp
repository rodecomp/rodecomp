#include "UI/Window/UIViewCameraInfoWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/World.h"
#include "UI/Buttons/UIButton.h"
#include "Utility/Text/FormatString.h"

#include <string>

// OFFSET: 0x00A78C20
// FUNCTION: fn_16894
bool UIViewCameraInfoWnd::Slot08() {
  return true;
}

// OFFSET: 0x00A78C30
// FUNCTION: fn_16895
void UIViewCameraInfoWnd::Slot15(int, int) {
}

// OFFSET: 0x00A78C40

// OFFSET: 0x00A78DE0
// FUNCTION: fn_16897
int UIViewCameraInfoWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
