#include "Client/Browser/WebBrowser.h"
#include "UI/Window/UIWindowMgr.h"

#include <oleauto.h>

// GLOBAL: gl_24370
WNDPROC lpPrevWndFunc;

// OFFSET: 0x00AB20F0
// FUNCTION: fn_17411
void __fastcall sub_AB20F0(BSTR* address) {
  SysFreeString(*address);
}

// OFFSET: 0x00AB21F0
// FUNCTION: fn_17412
LRESULT CALLBACK BrowserAddressEditProc(HWND window, UINT message, WPARAM value, LPARAM flags) {
  if (value == VK_RETURN) {
    if (message == WM_KEYUP || message == WM_CHAR) {
      return 0;
    }

    if (message == WM_KEYDOWN && g_windowMgr.activeBrowser) {
      SendMessageA(g_windowMgr.activeBrowser->host, WM_COMMAND, static_cast<WPARAM>(BrowserCommand::Navigate), 0);
      return 0;
    }
  }

  return CallWindowProcA(lpPrevWndFunc, window, message, value, flags);
}
