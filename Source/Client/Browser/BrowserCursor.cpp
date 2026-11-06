#include "Client/Application/ApplicationWindow.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"

// OFFSET: 0x00AB32E0
// FUNCTION: fn_45439
void CWebBrowser::SetCursorState(int value) {
  if (this->cursorState == value) {
    return;
  }

  if (value != 0) {
    SetCursor(LoadCursorA(nullptr, reinterpret_cast<LPCSTR>(IDC_ARROW)));

    g_mouse.Center();
    g_mouse.SetX(-100);
    g_mouse.SetY(-100);
  } else {
    SetCursor(nullptr);
  }

  g_mouse.Reset();
  ShowCursor(value);
  this->cursorState = value;
}

// OFFSET: 0x00AB3230
// FUNCTION: fn_45438
void CWebBrowser::PollCursor() {
  POINT point;
  GetCursorPos(&point);
  if (hWnd != WindowFromPoint(point)) {
    if (this->cursorState != 1) {
      this->SetCursorState(1);
    }
  } else if (this->cursorState != 0) {
    this->SetCursorState(0);
  }
}
