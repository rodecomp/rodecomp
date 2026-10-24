#ifndef RAGNAROK_SOURCE_UI_BROWSERFACTORY_H
#define RAGNAROK_SOURCE_UI_BROWSERFACTORY_H

#include <windows.h>

class CWebBrowser;
struct WebBrowserContext;
struct BrowserRegistry;
struct ChatNotifications;

// Ordinary browser creation is distinct from the special-browser factory.
// The limit counts all map nodes, not nonnull entries. Failed ordinary creation
// retains the allocated browser in the original; it is not published or deleted.
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
