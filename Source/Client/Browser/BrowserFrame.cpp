#include "Client/Browser/WebBrowser.h"

// OFFSET: 0x00AB2610
// FUNCTION: fn_17414
void CWebBrowser::Normalize() {
  if (!this->externalEvents) {
    return;
  }

  RECT rectangle;
  GetWindowRect(this->browserWindow, &rectangle);
  if (rectangle.right - rectangle.left != this->screenWidth || rectangle.bottom - rectangle.top != this->screenHeight) {
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
