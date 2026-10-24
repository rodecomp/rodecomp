#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERMESSAGES_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERMESSAGES_H

#include <windows.h>

class CWebBrowser;
struct BrowserRegistry;

// Borrowed UIWindowMgr browser registry, active browser and main HWND.
// Caller installs its native thunk; this implements the entire original body.
struct BrowserMessages {
  BrowserRegistry& registry;
  CWebBrowser*& active;
  HWND const& applicationWindow;

  LRESULT Message(HWND window, UINT message, WPARAM value, LPARAM flags);
};

#endif
