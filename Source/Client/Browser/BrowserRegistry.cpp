#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Browser/WebBrowser.h"
#include "UI/Window/UIWindowMgr.h"

#include <new>

// OFFSET: 0x00A91990
// FUNCTION: fn_17167
CWebBrowser* UIWindowMgr::FindBrowser(HWND window) {
  const auto found = this->browsers.find(window);
  return found != this->browsers.end() ? found->second : nullptr;
}

// OFFSET: 0x00A8F860
// FUNCTION: fn_17146
void UIWindowMgr::RemoveBrowser(HWND window) {
  const auto found = this->browsers.find(window);
  if (found != this->browsers.end()) {
    CWebBrowser* browser = found->second;
    if (browser) {
      if (browser == this->specialBrowser) {
        this->specialBrowser = nullptr;
      }

      delete browser;
    }

    this->browsers.erase(found);
    return;
  }

  if (window == this->specialBrowser->host) {
    delete this->specialBrowser;
    this->specialBrowser = nullptr;
  }
}

// OFFSET: 0x00A8C960
// FUNCTION: fn_17138
CWebBrowser* UIWindowMgr::EnsureSpecialBrowser(int width, int height) {
  if (this->specialBrowser) {
    return this->specialBrowser;
  }

  this->specialBrowser = new CWebBrowser();
  if (this->specialBrowser && this->specialBrowser->Create(hWnd, g_instance, width, height)) {
    this->browsers.insert(std::make_pair(this->specialBrowser->host, this->specialBrowser));
  }

  return this->specialBrowser;
}
