#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERWINDOWSIZING_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERWINDOWSIZING_H

// clang-format off
#include <windows.h>
#include <ole2.h>
#include <exdisp.h>
// clang-format on

// Borrowed IWebBrowser2 and original browser-owner dimensions. COM lifetime,
// activation and navigation are separate; this view owns no references.
struct BrowserWindowSizing {
  IWebBrowser2* const& browser;
  const int& screenWidth;
  const int& screenHeight;
  HWND const& auxiliary;
  const unsigned char& auxiliaryVisible;
  int& width;
  int& height;

  // Exact screen dimensions are reduced by one, then optional auxiliary
  // client height plus four is removed. Native setter errors are ignored.
  void Resize(int requestedWidth, int requestedHeight);
  void Refresh();
};

// Browser host-site presence, host HWND and auxiliary-visible byte.
struct BrowserAuxiliaryVisibility {
  BrowserWindowSizing& sizing;
  const bool& hasSite;
  HWND const& host;
  unsigned char& visible;

  void Show(unsigned char value);
};

#endif
