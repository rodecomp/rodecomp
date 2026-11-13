#include "Client/Application/ApplicationWindow.h"
#include "Client/Browser/WebBrowser.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>

// OFFSET: 0x00AB33C0
// FUNCTION: fn_17423
LRESULT CALLBACK BrowserHostProc(HWND window, UINT message, WPARAM value, LPARAM flags) {
  CWebBrowser* browser = g_windowMgr.FindBrowser(window);

  switch (message) {
  case WM_CREATE:
  case WM_NCHITTEST:
  case WM_NCLBUTTONDOWN:
  case WM_PARENTNOTIFY:
    g_windowMgr.activeBrowser = g_windowMgr.FindBrowser(window);
    break;
  case WM_DESTROY:
    SetFocus(hWnd);
    break;
  case WM_CLOSE:
    g_windowMgr.RemoveBrowser(window);
    break;
  case WM_SIZE:
    if (browser) {
      const auto dimensions = static_cast<std::uint32_t>(flags);
      browser->Resize(static_cast<std::uint16_t>(dimensions), dimensions >> 16);
    }
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
