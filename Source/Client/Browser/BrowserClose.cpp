#include "Client/Browser/BrowserClose.h"
#include "Client/Input/Mouse.h"

// OFFSET: 0x00AB24D0
// FUNCTION: fn_45432
bool BrowserClose::Close() {
  if (this->cursorState != 0) {
    SetCursor(nullptr);
    this->mouse.Reset();
    ShowCursor(FALSE);
    this->cursorState = 0;
  }

  if (this->attachment.site) {
    this->attachment.Detach();
  }

  if (this->externalEvents) {
    this->externalEvents->Release();
    this->externalEvents = nullptr;
  }

  if (this->browser) {
    this->browser->Stop();
    SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
    SetParent(this->browserWindow, this->applicationWindow);
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
