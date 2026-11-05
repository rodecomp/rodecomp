#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Browser/WebBrowser.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"

#include <new>

namespace {
  constexpr unsigned maximumBrowserCount = 5;
}

// OFFSET: 0x00A8C840
// FUNCTION: fn_17136
CWebBrowser* UIWindowMgr::CreateBrowser(int width, int height) {
  if (this->browsers.size() >= maximumBrowserCount) {
    this->SendNotice(3, 0, 0, 0, 0, 0);
    this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(MessageId::MSI_FAILED_ADD_TAB)), RGB(255, 0, 0), 0, 0, 0);
    return nullptr;
  }

  CWebBrowser* browser = new CWebBrowser();
  if (browser && browser->Create(hWnd, g_instance, width, height)) {
    this->browsers.emplace(browser->host, browser);
    return browser;
  }

  return nullptr;
}

// OFFSET: 0x00A8C950
// FUNCTION: fn_17137
void __stdcall SubA8C950(int, int) {}
