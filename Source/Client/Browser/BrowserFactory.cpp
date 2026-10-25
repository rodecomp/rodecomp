#include "Client/Browser/BrowserRegistry.h"
#include "Client/Browser/WebBrowser.h"
#include "Contents/MessageTable.h"
#include "Client/Browser/BrowserFactory.h"
#include "UI/Chat/ChatNotifications.h"

namespace {
  constexpr unsigned maximumBrowserCount = 5;
}

// OFFSET: 0x00A8C840
// FUNCTION: fn_17136
CWebBrowser* BrowserFactory::Create(int width, int height) {
  if (this->registry.browsers.size() >= maximumBrowserCount) {
    this->notifications.Refresh();
    const char* message = LookupMessage(MessageId::MSI_FAILED_ADD_TAB, this->languagePrefix, this->serviceType, this->language);
    this->notifications.Text(message, RGB(255, 0, 0), nullptr, 0, false);

    return nullptr;
  }

  auto* browser = new CWebBrowser(this->context);
  if (browser->Create(width, height)) {
    this->registry.browsers.emplace(browser->host, browser);
    return browser;
  }

  return nullptr;
}
