#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERMESSAGES_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERMESSAGES_H

#include <windows.h>

class CWebBrowser;
struct BrowserRegistry;

enum class BrowserCommand : WORD { Navigate = 1000 };

struct BrowserMessages {
  BrowserRegistry& registry;
  CWebBrowser*& active;
  HWND const& applicationWindow;

  LRESULT Message(HWND window, UINT message, WPARAM value, LPARAM flags);
};

#endif
