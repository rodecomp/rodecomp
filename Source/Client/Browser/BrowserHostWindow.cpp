#include "Client/Browser/WebBrowser.h"
#include "Contents/Configuration/ServiceConfiguration.h"

// OFFSET: 0x00AB29D0
// FUNCTION: fn_45435
bool CWebBrowser::CreateHostWindow(HWND parent, HINSTANCE instance, int width, int height) {
  WNDCLASSEXA windowClass = {
    sizeof(WNDCLASSEXA), 0, BrowserHostProc, 0, 0, instance, LoadIconA(instance, MAKEINTRESOURCEA(123)), nullptr, nullptr, nullptr, "ROWebBrowser", nullptr
  };
  RegisterClassExA(&windowClass);
  this->host = CreateWindowExA(0, "ROWebBrowser", g_windowName, WS_POPUP | WS_OVERLAPPEDWINDOW, 0, 0, width, height, parent, nullptr, instance, nullptr);
  if (!this->host) {
    OutputDebugStringA("failed : CreateWindow() \n");
    return false;
  }

  UpdateWindow(this->host);
  SetParent(this->browserWindow, this->host);
  SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
  return true;
}
