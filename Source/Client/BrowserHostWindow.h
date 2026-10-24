#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERHOSTWINDOW_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERHOSTWINDOW_H

#include <windows.h>

// Borrowed CWebBrowser host and browser HWND, application
// title and complete browser window procedure. This operation does not replace
// that procedure with DefWindowProc or invent the browser host-site lifecycle.
struct BrowserHostWindow {
  HWND& host;
  HWND const& browser;
  const char* const& title;
  WNDPROC procedure;

  bool Create(HWND parent, HINSTANCE instance, int width, int height);
};

#endif
