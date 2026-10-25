#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERSITEATTACHMENT_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERSITEATTACHMENT_H

#include "Client/Browser/BrowserClientSite.h"
#include "Client/Browser/BrowserWindowSizing.h"

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
