#include "Client/Browser/WebBrowser.h"

// INLINED
int CWebBrowser::AuxiliaryHeight() {
  if (this->rebar && this->auxiliaryVisible != 0) {
    RECT rectangle;
    GetClientRect(this->rebar, &rectangle);
    return rectangle.bottom - rectangle.top + 4;
  }

  return 0;
}

// OFFSET: 0x00AB3060
// FUNCTION: fn_17421
void CWebBrowser::Resize(int requestedWidth, int requestedHeight) {
  if (!this->browser) {
    return;
  }

  this->width = this->screenWidth != requestedWidth ? requestedWidth : requestedWidth - 1;
  const int height = this->screenHeight != requestedHeight ? requestedHeight : requestedHeight - 1;
  this->height = height - this->AuxiliaryHeight();
  this->browser->put_Width(this->width);
  this->browser->put_Height(this->height);

  if (this->rebar) {
    SetWindowPos(this->rebar, nullptr, 0, 0, requestedWidth, requestedHeight, SWP_NOMOVE);
  }
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
    POINT origin;
    origin.x = 0;
    origin.y = 0;
    ClientToScreen(this->host, &origin);
    this->browser->put_Left(-origin.x);
  }

  this->browser->put_Top(this->AuxiliaryHeight());

  RECT rectangle;
  GetClientRect(this->host, &rectangle);
  this->Resize(rectangle.right - rectangle.left, rectangle.bottom - rectangle.top);
  this->browser->put_Visible(VARIANT_TRUE);
}
