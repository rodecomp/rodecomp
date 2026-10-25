#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERADDRESSBAR_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERADDRESSBAR_H

#include <windows.h>

struct BrowserAddressBar {
  HWND const& host;
  HWND& rebar;
  HWND& combo;
  int& height;
  const unsigned char& visible;
  WNDPROC& previous;
  WNDPROC procedure;

  bool Create(int width, int height);
};

#endif
