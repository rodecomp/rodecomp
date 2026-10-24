#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERSITEATTACHMENT_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERSITEATTACHMENT_H

#include "Client/BrowserClientSite.h"
#include "Client/BrowserWindowSizing.h"

// Borrowed original owner fields, real popup factory and renderer dimensions.
// Site publication precedes COM work and remains on failure. Detach destroys the
// site even when QueryInterface fails; it does not clear the COM client site.
struct BrowserSiteAttachment {
  BrowserWindowSizing& sizing;
  HWND const& host;
  BrowserClientSite*& site;
  DWORD& cookie;
  BrowserFactory& factory;
  const int& renderWidth;
  const int& renderHeight;

  bool Attach();
  void Detach();
};

#endif
