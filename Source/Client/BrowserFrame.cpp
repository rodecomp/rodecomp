#include "Client/BrowserFrame.h"

// OFFSET: 0x00AB2610
// FUNCTION: fn_17414
void BrowserFrame::Normalize() {
  if (!this->hasExternalEvents) {
    return;
  }

  RECT rectangle;
  GetWindowRect(this->browserWindow, &rectangle);
  if (
    rectangle.right - rectangle.left != this->sizing.screenWidth ||
    rectangle.bottom - rectangle.top != this->sizing.screenHeight
  ) {
    return;
  }

  this->sizing.browser->put_Visible(VARIANT_FALSE);
  SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
  SetParent(this->browserWindow, this->host);
  this->sizing.browser->put_FullScreen(VARIANT_FALSE);
  this->sizing.browser->put_AddressBar(VARIANT_FALSE);
  SetParent(this->browserWindow, this->host);
  SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
  this->sizing.browser->put_Visible(VARIANT_TRUE);
}

// OFFSET: 0x00AB2E70
// FUNCTION: fn_17419
void BrowserFrame::Poll() {
  if (!IsWindowVisible(this->host)) {
    return;
  }

  this->Normalize();
  this->sizing.Refresh();
  this->cursor.Poll();
}
