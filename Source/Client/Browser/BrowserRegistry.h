#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERREGISTRY_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERREGISTRY_H

#include <map>
#include <windows.h>

class CWebBrowser;
struct WebBrowserContext;

struct BrowserRegistry {
  std::map<HWND, CWebBrowser*>& browsers;
  CWebBrowser*& special;

  CWebBrowser* Find(HWND window) const;
  void Remove(HWND window);
  CWebBrowser* EnsureSpecial(WebBrowserContext& context, int width, int height);
};

#endif
