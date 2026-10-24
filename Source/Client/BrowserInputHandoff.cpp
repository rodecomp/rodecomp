#include "Client/ApplicationActivation.h"
#include "Client/BrowserInputHandoff.h"
#include "Client/Mouse.h"
#include "Contents/ServiceOptions.h"

// OFFSET: 0x00AB2720
// FUNCTION: fn_45433
void BrowserInputHandoff::Enter() {
  if (this->service.Get(271) == 0) {
    return;
  }

  POINT position{this->mouse.x, this->mouse.y};
  this->service.Set(271, 0);
  this->mouse.SetX(position.x);
  this->mouse.SetY(position.y);
  this->restoreOption = 1;
  ClientToScreen(this->applicationWindow, &position);
  SetCursorPos(position.x, position.y);
}

// OFFSET: 0x00AB3350
// FUNCTION: fn_45440
void BrowserInputHandoff::Restore() {
  if (this->restoreOption == 0) {
    return;
  }

  POINT position;
  GetCursorPos(&position);
  ScreenToClient(this->applicationWindow, &position);
  this->service.Set(271, 1);
  this->mouse.SetX(position.x);
  this->mouse.SetY(position.y);
  SetActiveWindow(this->applicationWindow);
  this->activation.Set(1);
}
