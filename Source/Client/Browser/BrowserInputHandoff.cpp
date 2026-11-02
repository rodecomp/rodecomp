#include "Client/Application/ApplicationActivation.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"
#include "Contents/Configuration/ServiceOptions.h"

// OFFSET: 0x00AB2720
// FUNCTION: fn_45433
void CWebBrowser::EnterInput() {
  if (g_serviceOptions.Get(ServiceOption::ConfineCursor) == 0) {
    return;
  }

  POINT position{g_mouse.x, g_mouse.y};
  g_serviceOptions.Set(ServiceOption::ConfineCursor, 0);
  g_mouse.SetX(position.x);
  g_mouse.SetY(position.y);
  this->restoreOption = 1;
  ClientToScreen(hWnd, &position);
  SetCursorPos(position.x, position.y);
}

// OFFSET: 0x00AB3350
// FUNCTION: fn_45440
void CWebBrowser::RestoreInput() {
  if (this->restoreOption == 0) {
    return;
  }

  POINT position;
  GetCursorPos(&position);
  ScreenToClient(hWnd, &position);
  g_serviceOptions.Set(ServiceOption::ConfineCursor, 1);
  g_mouse.SetX(position.x);
  g_mouse.SetY(position.y);
  SetActiveWindow(hWnd);
  SetApplicationActive(1);
}
