#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERFACTORY_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERFACTORY_H

#include <windows.h>

class CWebBrowser;
struct WebBrowserContext;
struct BrowserRegistry;
struct ChatNotifications;

struct BrowserFactory {
  BrowserRegistry& registry;
  WebBrowserContext& context;
  ChatNotifications& notifications;
  const bool& languagePrefix;
  const int& serviceType;
  const WORD& language;

  CWebBrowser* Create(int width, int height);
};

#endif
