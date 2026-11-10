#include "UI/Window/UIEnums.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Browser/WebBrowser.h"
#include "Contents/MessageTable.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/Window/UIWindowMgr.h"

#include <new>

namespace {
  constexpr unsigned maximumBrowserCount = 5;
}

// OFFSET: 0x00A8C840
// FUNCTION: fn_17136
CWebBrowser* UIWindowMgr::CreateBrowser(int width, int height) {
  if (this->browsers.size() < maximumBrowserCount) {
    CWebBrowser* browser = new CWebBrowser();
    if (browser && browser->Create(hWnd, g_instance, width, height)) {
      this->browsers.insert(std::make_pair(browser->host, browser));
      return browser;
    }
  } else {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_ADD_TAB)), RGB(255, 0, 0), 0, 0, 0);
  }

  return nullptr;
}

// OFFSET: 0x00A8C950
// FUNCTION: fn_17137
void __stdcall SubA8C950(int, int) {}
