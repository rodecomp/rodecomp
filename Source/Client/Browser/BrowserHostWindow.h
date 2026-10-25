#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERHOSTWINDOW_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERHOSTWINDOW_H

#include <windows.h>

struct BrowserHostWindow {
  HWND& host;
  HWND const& browser;
  const char* const& title;
  WNDPROC procedure;

  bool Create(HWND parent, HINSTANCE instance, int width, int height);
};

#endif
