#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERCLOSE_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERCLOSE_H

#include "Client/BrowserSiteAttachment.h"

class CMouse;

// Borrowed original browser fields. External events use a separate COM reference
// independent of the client-site dispatch dependency. Auxiliary HWNDs and
// cookie remain untouched even after their host destroys the native children.
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
