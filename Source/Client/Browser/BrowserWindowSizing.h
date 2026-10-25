#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERWINDOWSIZING_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERWINDOWSIZING_H

#include <windows.h>
#include <ole2.h>
#include <exdisp.h>

struct BrowserWindowSizing {
  IWebBrowser2* const& browser;
  const int& screenWidth;
  const int& screenHeight;
  HWND const& auxiliary;
  const unsigned char& auxiliaryVisible;
  int& width;
  int& height;

  void Resize(int requestedWidth, int requestedHeight);
  void Refresh();
};

struct BrowserAuxiliaryVisibility {
  BrowserWindowSizing& sizing;
  const bool& hasSite;
  HWND const& host;
  unsigned char& visible;

  void Show(unsigned char value);
};

#endif
