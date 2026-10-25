#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCREATION_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCREATION_H

#include "Client/Browser/BrowserAddressBar.h"
#include "Client/Browser/BrowserHostWindow.h"
#include "Client/Browser/BrowserSiteAttachment.h"

struct BrowserCreation {
  IWebBrowser2*& browser;
  HWND& browserWindow;
  BrowserHostWindow& window;
  BrowserSiteAttachment& attachment;
  BrowserAddressBar& addressBar;

  bool Create(HWND parent, HINSTANCE instance, int width, int height);
};

#endif
