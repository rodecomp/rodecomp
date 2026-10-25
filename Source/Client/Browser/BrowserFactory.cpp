#include "Client/Browser/BrowserRegistry.h"
#include "Client/Browser/WebBrowser.h"
#include "Contents/MessageTable.h"
#include "Client/Browser/BrowserFactory.h"
#include "UI/Chat/ChatNotifications.h"

#include <new>

// OFFSET: 0x00A8C840
// FUNCTION: fn_17136
CWebBrowser* BrowserFactory::Create(int width, int height) {
  if (this->registry.browsers.size() >= 5) {
    this->notifications.Refresh();
    const char* message = LookupMessage(1578, this->languagePrefix, this->serviceType, this->language);
    this->notifications.Text(message, 255, nullptr, 0, false);

    return nullptr;
  }

  auto* browser = new CWebBrowser(this->context);
  if (browser->Create(width, height)) {
    this->registry.browsers.emplace(browser->host, browser);
    return browser;
  }

  return nullptr;
}
