#include "Client/Browser/BrowserRegistry.h"
#include "Client/Browser/WebBrowser.h"

#include <new>

// OFFSET: 0x00A91990
// FUNCTION: fn_17167
CWebBrowser* BrowserRegistry::Find(HWND window) const {
  const auto found = this->browsers.find(window);
  return found == this->browsers.end() ? nullptr : found->second;
}

// OFFSET: 0x00A8F860
// FUNCTION: fn_17146
void BrowserRegistry::Remove(HWND window) {
  const auto found = this->browsers.find(window);
  if (found != this->browsers.end()) {
    CWebBrowser* browser = found->second;
    if (browser) {
      if (browser == this->special) {
        this->special = nullptr;
      }

      delete browser;
    }

    this->browsers.erase(found);
    return;
  }

  if (window == this->special->host) {
    delete this->special;
    this->special = nullptr;
  }
}

// OFFSET: 0x00A8C960
// FUNCTION: fn_17138
CWebBrowser* BrowserRegistry::EnsureSpecial(WebBrowserContext& context, int width, int height) {
  if (this->special) {
    return this->special;
  }

  this->special = new (std::nothrow) CWebBrowser(context);
  if (this->special && this->special->Create(width, height)) {
    this->browsers.emplace(this->special->host, this->special);
  }

  return this->special;
}
