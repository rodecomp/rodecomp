#include "Client/Browser/WebBrowser.h"

// OFFSET: 0x00AB3060
// FUNCTION: fn_17421
void CWebBrowser::Resize(int requestedWidth, int requestedHeight) {
  if (!this->browser) {
    return;
  }

  this->width = requestedWidth == this->screenWidth ? requestedWidth - 1 : requestedWidth;
  int height = requestedHeight == this->screenHeight ? requestedHeight - 1 : requestedHeight;

  if (this->rebar && this->auxiliaryVisible != 0) {
    RECT rectangle;
    GetClientRect(this->rebar, &rectangle);
    height -= (rectangle.bottom - rectangle.top + 4);
  }

  this->height = height;
  this->browser->put_Width(this->width);
  this->browser->put_Height(this->height);

  if (this->rebar) {
    SetWindowPos(this->rebar, nullptr, 0, 0, requestedWidth, requestedHeight, SWP_NOMOVE);
  }
}

// OFFSET: 0x00AB2260
// FUNCTION: fn_45430
void CWebBrowser::RefreshSize() {
  long width = 0;
  long height = 0;
  this->browser->get_Width(&width);
  this->browser->get_Height(&height);

  if (width != this->width || height != this->height) {
    this->Resize(this->width, this->height);
  }
}

// INLINED
int CWebBrowser::AuxiliaryHeight() {
  if (this->rebar && this->auxiliaryVisible != 0) {
    RECT rectangle;
    GetClientRect(this->rebar, &rectangle);
    return rectangle.bottom - rectangle.top + 4;
  }

  return 0;
}

// OFFSET: 0x00AB3120
// FUNCTION: fn_17422
void CWebBrowser::ShowAuxiliary(unsigned char value) {
  if (!this->browser) {
    return;
  }

  this->auxiliaryVisible = value;
  if (!this->rebar) {
    return;
  }

  this->browser->put_Visible(VARIANT_FALSE);
  ShowWindow(this->rebar, this->auxiliaryVisible != 0 ? SW_SHOW : SW_HIDE);

  if (this->externalEvents) {
    RECT rectangle;
    GetWindowRect(this->host, &rectangle);
    POINT origin{};
    ClientToScreen(this->host, &origin);
    this->browser->put_Left(-origin.x);
  }

  this->browser->put_Top(this->AuxiliaryHeight());

  RECT rectangle;
  GetClientRect(this->host, &rectangle);
  this->Resize(rectangle.right - rectangle.left, rectangle.bottom - rectangle.top);
  this->browser->put_Visible(VARIANT_TRUE);
}
