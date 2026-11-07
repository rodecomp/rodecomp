#include "Client/Browser/WebBrowser.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00AA26C0
// FUNCTION: fn_17215
void UIWindowMgr::UpdateFrameWindows() {
  for (auto window = this->rendered.begin(); window != this->rendered.end(); ++window) {
    (*window)->OnProcess();
  }

  for (auto browser = this->browsers.begin(); browser != this->browsers.end();) {
    if (browser->second) {
      browser->second->Poll();
    }

    if (this->browsers.empty()) {
      break;
    }

    ++browser;
  }

  for (auto window = this->overlays.begin(); window != this->overlays.end(); ++window) {
    window->second->OnProcess();
  }
}
