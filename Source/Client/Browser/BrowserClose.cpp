#include "Client/Application/ApplicationWindow.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"

// OFFSET: 0x00AB24D0
// FUNCTION: fn_45432
bool CWebBrowser::Close() {
  if (this->cursorState != 0) {
    SetCursor(nullptr);
    g_mouse.Reset();
    ShowCursor(FALSE);
    this->cursorState = 0;
  }

  if (this->site) {
    this->DetachSite();
  }

  if (this->externalEvents) {
    this->externalEvents->Release();
    this->externalEvents = nullptr;
  }

  if (this->browser) {
    this->browser->Stop();
    SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
    SetParent(this->browserWindow, hWnd);
    this->browser->Quit();
    this->browser->Release();
    this->browser = nullptr;
  }

  if (this->host) {
    DestroyWindow(this->host);
    this->host = nullptr;
  }

  return true;
}
