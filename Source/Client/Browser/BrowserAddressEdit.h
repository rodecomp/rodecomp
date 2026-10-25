#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERADDRESSEDIT_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERADDRESSEDIT_H

#include <windows.h>

struct BrowserAddressEdit {
  WNDPROC const& previous;
  HWND const* const& activeHost;

  LRESULT Message(HWND window, UINT message, WPARAM value, LPARAM flags);
};

#endif
