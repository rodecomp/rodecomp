#include "Client/Browser/BrowserCursor.h"
#include "Client/Input/Mouse.h"
#include "Render/RenderSystemBase.h"

// OFFSET: 0x00AB32E0
// FUNCTION: fn_45439
void BrowserCursor::Set(int value) {
  if (this->state == value) {
    return;
  }

  if (value != 0) {
    SetCursor(LoadCursorA(nullptr, reinterpret_cast<LPCSTR>(IDC_ARROW)));

    this->mouse.SetX(static_cast<int>(this->renderSystem.Width()) / 2);
    this->mouse.SetY(static_cast<int>(this->renderSystem.Height()) / 2);
    this->mouse.SetX(-100);
    this->mouse.SetY(-100);
  } else {
    SetCursor(nullptr);
  }

  this->mouse.Reset();
  ShowCursor(value);
  this->state = value;
}

// OFFSET: 0x00AB3230
// FUNCTION: fn_45438
void BrowserCursor::Poll() {
  POINT point;
  GetCursorPos(&point);
  if (WindowFromPoint(point) != this->applicationWindow) {
    if (this->state != 1) {
      this->Set(1);
    }
  } else if (this->state != 0) {
    this->Set(0);
  }
}
