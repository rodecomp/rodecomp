#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERCREATION_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERCREATION_H

#include "Client/BrowserAddressBar.h"
#include "Client/BrowserHostWindow.h"
#include "Client/BrowserSiteAttachment.h"

// Views must refer to the same CWebBrowser fields. Caller supplies the complete
// host procedure, edit subclass and site event dependency. No rollback on error:
// browser, host, site and native children remain published at their original steps.
struct BrowserCreation {
  IWebBrowser2*& browser;
  HWND& browserWindow;
  BrowserHostWindow& window;
  BrowserSiteAttachment& attachment;
  BrowserAddressBar& addressBar;

  bool Create(HWND parent, HINSTANCE instance, int width, int height);
};

#endif
