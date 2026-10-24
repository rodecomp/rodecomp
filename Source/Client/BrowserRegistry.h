#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERREGISTRY_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERREGISTRY_H

#include <map>
#include <windows.h>

class CWebBrowser;
struct WebBrowserContext;

// Borrowed UIWindowMgr browser map and special-browser pointer.
// Map nodes survive browser destruction, permitting native callbacks to look up
// the original entry. The special alias is cleared before mapped destruction.
struct BrowserRegistry {
  std::map<HWND, CWebBrowser*>& browsers;
  CWebBrowser*& special;

  CWebBrowser* Find(HWND window) const;
  void Remove(HWND window);
  CWebBrowser* EnsureSpecial(WebBrowserContext& context, int width, int height);
};

#endif
