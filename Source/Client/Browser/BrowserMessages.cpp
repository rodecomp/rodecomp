#include "Client/Application/ApplicationWindow.h"
#include "Client/Browser/WebBrowser.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>

// OFFSET: 0x00AB2D50
// FUNCTION: fn_17418
bool CWebBrowser::Navigate(std::string address) {
  if (!this->browser) {
    return false;
  }

  const auto length = address.size();
  wchar_t* converted = new wchar_t[length + 1];
  MultiByteToWideChar(CP_ACP, 0, address.c_str(), -1, converted, static_cast<int>(length));
  converted[length] = L'\0';
  BSTR url = SysAllocString(converted);
  delete[] converted;

  VARIANT optional;
  VariantInit(&optional);
  const HRESULT result = this->browser->Navigate(url, &optional, &optional, &optional, &optional);
  SysFreeString(url);
  ShowWindow(this->host, SW_SHOW);
  this->browser->put_Visible(VARIANT_TRUE);

  if (this->combo) {
    SetWindowTextA(this->combo, address.c_str());
  }

  if (FAILED(result)) {
    OutputDebugStringA("failed : Navigate() \n");
    return false;
  }

  return true;
}

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
