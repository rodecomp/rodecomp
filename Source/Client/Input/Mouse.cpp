#include "Client/Input/Mouse.h"
#include "Render/DeviceContext/DCSurface.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x006370C0
// OFFSET: 0x006370D0
// FUNCTION: fn_04724
CMouse::~CMouse() = default;

// OFFSET: 0x00637100
// FUNCTION: fn_04726
void CMouse::Update() {
  if (!this->active) {
    return;
  }

  this->deltaX = this->pendingX - this->x;
  this->deltaY = this->pendingY - this->y;
  this->x = this->pendingX;
  this->y = this->pendingY;
  const std::uint32_t now = timeGetTime();

  for (unsigned index = 0; index < 2; ++index) {
    if (this->deltaX != 0 || this->deltaY != 0) {
      this->pressTicks[index] = now - this->doubleClickTime;
    }

    if (this->pendingButtons[index]) {
      if (this->previousButtons[index]) {
        this->buttons[index] = 2;
      } else if (now >= this->pressTicks[index] + this->doubleClickTime) {
        this->pressTicks[index] = now;
        this->buttons[index] = 1;
      } else {
        this->buttons[index] = 4;
        this->pressTicks[index] = now - this->doubleClickTime;
      }
    } else {
      this->buttons[index] = this->previousButtons[index] ? 3 : 0;
    }
  }

  this->wheel = this->pendingWheel;
  this->pendingWheel = 0;
  this->previousButtons[0] = this->pendingButtons[0];
  this->previousButtons[1] = this->pendingButtons[1];
}

// OFFSET: 0x006371E0
// FUNCTION: fn_04727
void CMouse::Reset() {
  this->buttons[1] = 0;
  this->buttons[0] = 0;
  this->previousButtons[0] = 0;
  this->previousButtons[1] = 0;
  this->pendingButtons[0] = 0;
  this->pendingButtons[1] = 0;
}

// OFFSET: 0x00637200
// FUNCTION: fn_04728
int CMouse::Center() {
  const int halfWidth = static_cast<int>(GetRenderSystemHolder()->Width()) / 2;
  this->pendingX = halfWidth;
  this->x = halfWidth;
  const int halfHeight = static_cast<int>(GetRenderSystemHolder()->Height()) / 2;
  this->pendingY = halfHeight;
  this->y = halfHeight;

  return halfHeight;
}

// OFFSET: 0x00637240
// FUNCTION: fn_04729
void CMouse::SetX(int value) {
  this->pendingX = value;
  this->x = value;
}

// OFFSET: 0x00637250
// FUNCTION: fn_04730
void CMouse::SetY(int value) {
  this->pendingY = value;
  this->y = value;
}

// OFFSET: 0x00637260
// FUNCTION: fn_04731
void CMouse::Message(CMouse& global, HWND mainWindow, HWND source, UINT message, WPARAM keys, LPARAM) {
  if (global.active != 1) {
    global.Reset();
    global.active = 1;
  }

  switch (message) {
  case WM_MOUSEMOVE: {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(mainWindow, &point);
    this->pendingX = point.x;
    this->pendingY = point.y;
    break;
  }
  case WM_LBUTTONDOWN:
    this->pendingButtons[0] = 1;
    SetCapture(source);
    break;
  case WM_LBUTTONUP:
    this->pendingButtons[0] = 0;
    ReleaseCapture();
    break;
  case WM_RBUTTONDOWN:
    this->pendingButtons[1] = 1;
    break;
  case WM_RBUTTONUP:
    this->pendingButtons[1] = 0;
    break;
  case WM_MOUSEWHEEL:
    this->pendingWheel = GET_WHEEL_DELTA_WPARAM(keys);
    break;
  }
}
