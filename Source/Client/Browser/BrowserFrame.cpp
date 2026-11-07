#include "Client/Application/ApplicationWindow.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"

// OFFSET: 0x00AB2260
// FUNCTION: fn_45430
void CWebBrowser::RefreshSize() {
  long width = 0;
  long height = 0;
  this->browser->get_Width(&width);
  this->browser->get_Height(&height);

  if (this->width != width || this->height != height) {
    this->Resize(this->width, this->height);
  }
}

// OFFSET: 0x00AB2610
// FUNCTION: fn_17414
void CWebBrowser::Normalize() {
  if (!this->externalEvents) {
    return;
  }

  RECT rectangle;
  GetWindowRect(this->browserWindow, &rectangle);
  if (this->screenWidth != rectangle.right - rectangle.left || this->screenHeight != rectangle.bottom - rectangle.top) {
    return;
  }

  this->browser->put_Visible(VARIANT_FALSE);
  SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
  SetParent(this->browserWindow, this->host);
  this->browser->put_FullScreen(VARIANT_FALSE);
  this->browser->put_AddressBar(VARIANT_FALSE);
  SetParent(this->browserWindow, this->host);
  SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
  this->browser->put_Visible(VARIANT_TRUE);
}

// OFFSET: 0x00AB2E70
// FUNCTION: fn_17419
void CWebBrowser::Poll() {
  if (!IsWindowVisible(this->host)) {
    return;
  }

  this->Normalize();
  this->RefreshSize();
  this->PollCursor();
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
