#include "Client/BrowserRegistry.h"
#include "Client/WebBrowser.h"
#include "UI/UIWindow.h"
#include "UI/WindowFrameUpdate.h"

// OFFSET: 0x00AA26C0
// FUNCTION: fn_17215
void WindowFrameUpdate::Update() {
  for (auto window = this->rendered.begin(); window != this->rendered.end(); ++window) {
    (*window)->Slot19();
  }

  for (auto browser = this->browsers.browsers.begin(); browser != this->browsers.browsers.end();) {
    if (browser->second) {
      browser->second->Poll();
    }

    if (this->browsers.browsers.empty()) {
      break;
    }
    ++browser;
  }

  for (auto window = this->overlays.begin(); window != this->overlays.end(); ++window) {
    window->second->Slot19();
  }
}
