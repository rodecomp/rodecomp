#include "Client/BrowserWindowSizing.h"

// OFFSET: 0x00AB3060
// FUNCTION: fn_17421
void BrowserWindowSizing::Resize(int requestedWidth, int requestedHeight) {
  if (!this->browser) {
    return;
  }

  this->width = requestedWidth == this->screenWidth ? requestedWidth - 1 : requestedWidth;
  int height = requestedHeight == this->screenHeight ? requestedHeight - 1 : requestedHeight;

  if (this->auxiliary && this->auxiliaryVisible != 0) {
    RECT rectangle;
    GetClientRect(this->auxiliary, &rectangle);
    height -= (rectangle.bottom - rectangle.top + 4);
  }

  this->height = height;
  this->browser->put_Width(this->width);
  this->browser->put_Height(this->height);

  if (this->auxiliary) {
    SetWindowPos(this->auxiliary, nullptr, 0, 0, requestedWidth, requestedHeight, SWP_NOMOVE);
  }
}

// OFFSET: 0x00AB2260
// FUNCTION: fn_45430
void BrowserWindowSizing::Refresh() {
  long width = 0;
  long height = 0;
  this->browser->get_Width(&width);
  this->browser->get_Height(&height);

  if (width != this->width || height != this->height) {
    this->Resize(this->width, this->height);
  }
}

// OFFSET: 0x00AB3120
// FUNCTION: fn_17422
void BrowserAuxiliaryVisibility::Show(unsigned char value) {
  if (!this->sizing.browser) {
    return;
  }

  this->visible = value;
  if (!this->sizing.auxiliary) {
    return;
  }

  this->sizing.browser->put_Visible(VARIANT_FALSE);
  ShowWindow(this->sizing.auxiliary, this->visible != 0 ? SW_SHOW : SW_HIDE);

  if (this->hasSite) {
    RECT rectangle;
    GetWindowRect(this->host, &rectangle);
    POINT origin{};
    ClientToScreen(this->host, &origin);
    this->sizing.browser->put_Left(-origin.x);
  }

  long top = 0;
  if (this->sizing.auxiliary && this->visible != 0) {
    RECT rectangle;
    GetClientRect(this->sizing.auxiliary, &rectangle);
    top = rectangle.bottom - rectangle.top + 4;
  }
  this->sizing.browser->put_Top(top);

  RECT rectangle;
  GetClientRect(this->host, &rectangle);
  this->sizing.Resize(rectangle.right - rectangle.left, rectangle.bottom - rectangle.top);
  this->sizing.browser->put_Visible(VARIANT_TRUE);
}
