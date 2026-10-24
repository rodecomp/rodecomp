#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERADDRESSBAR_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERADDRESSBAR_H

#include <windows.h>

// Borrowed host, rebar, combo, height, visible, and shared
// previous-edit-procedure global. Caller initializes common controls and supplies
// the actual BrowserAddressEdit thunk. Native verification requires v6 controls
// for the original 100-byte x86 band; startup activation policy is not recovered.
// Failed creation retains native children; normal host destruction owns them.
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
