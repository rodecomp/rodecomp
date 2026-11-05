#include "Client/Browser/WebBrowser.h"
#include "UI/Window/UIWindowMgr.h"

#include <oleauto.h>

// GLOBAL: gl_24370
WNDPROC lpPrevWndFunc;

struct BrowserAddressBstr {
  BSTR text;
  ~BrowserAddressBstr();
};

// OFFSET: 0x00AB20F0
// FUNCTION: fn_17411
BrowserAddressBstr::~BrowserAddressBstr() {
  SysFreeString(text);
}

// OFFSET: 0x00AB21F0
// FUNCTION: fn_17412
LRESULT CALLBACK BrowserAddressEditProc(HWND window, UINT message, WPARAM value, LPARAM flags) {
  switch (message) {
  case WM_KEYDOWN:
    if (value == VK_RETURN && g_windowMgr.activeBrowser) {
      SendMessageA(g_windowMgr.activeBrowser->host, WM_COMMAND, static_cast<WPARAM>(BrowserCommand::Navigate), 0);
      return 0;
    }
    break;
  case WM_KEYUP:
  case WM_CHAR:
    if (value == VK_RETURN) {
      return 0;
    }
    break;
  }

  return CallWindowProcA(lpPrevWndFunc, window, message, value, flags);
}
