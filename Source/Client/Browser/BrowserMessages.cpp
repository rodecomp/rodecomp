#include "Client/Browser/BrowserMessages.h"
#include "Client/Browser/BrowserRegistry.h"
#include "Client/Browser/WebBrowser.h"

#include <cstdint>

// OFFSET: 0x00AB33C0
// FUNCTION: fn_17423
LRESULT BrowserMessages::Message(HWND window, UINT message, WPARAM value, LPARAM flags) {
  CWebBrowser* browser = this->registry.Find(window);

  switch (message) {
  case WM_CREATE:
  case WM_NCHITTEST:
  case WM_NCLBUTTONDOWN:
  case WM_PARENTNOTIFY:
    this->active = this->registry.Find(window);
    break;
  case WM_DESTROY:
    SetFocus(this->applicationWindow);
    break;
  case WM_SIZE:
    if (browser) {
      const auto dimensions = static_cast<std::uint32_t>(flags);
      browser->Resize(static_cast<std::uint16_t>(dimensions), dimensions >> 16);
    }
    break;
  case WM_CLOSE:
    this->registry.Remove(window);
    break;
  case WM_HELP:
    return 1;
  case WM_KEYDOWN:
    if (value == VK_F1 || value == VK_F11) {
      return 1;
    }
    break;
  case WM_COMMAND:
    if (LOWORD(value) == static_cast<WORD>(BrowserCommand::Navigate) && browser) {
      char address[512];
      GetWindowTextA(browser->combo, address, sizeof(address));
      browser->Navigate(address);
    }
    break;
  }

  return DefWindowProcA(window, message, value, flags);
}
