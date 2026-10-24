#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERADDRESSEDIT_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERADDRESSEDIT_H

#include <windows.h>

// Borrowed original subclass procedure and active browser-host selection.
// Null activeHost denotes no active CWebBrowser, not a null HWND field inside
// an otherwise active object.
struct BrowserAddressEdit {
  WNDPROC const& previous;
  HWND const* const& activeHost;

  LRESULT Message(HWND window, UINT message, WPARAM value, LPARAM flags);
};

#endif
