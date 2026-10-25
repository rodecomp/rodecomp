#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCLOSE_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCLOSE_H

#include "Client/Browser/BrowserSiteAttachment.h"

class CMouse;

struct BrowserClose {
  BrowserSiteAttachment& attachment;
  IWebBrowser2*& browser;
  IUnknown*& externalEvents;
  HWND& host;
  HWND const& browserWindow;
  HWND const& applicationWindow;
  int& cursorState;
  CMouse& mouse;

  bool Close();
};

#endif
