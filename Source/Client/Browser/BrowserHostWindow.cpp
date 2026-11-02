#include "Client/Browser/WebBrowser.h"
#include "Contents/Configuration/ServiceConfiguration.h"

// OFFSET: 0x00AB29D0
// FUNCTION: fn_45435
bool CWebBrowser::CreateHostWindow(HWND parent, HINSTANCE instance, int width, int height) {
  WNDCLASSEXA windowClass{};
  windowClass.cbSize = sizeof(windowClass);
  windowClass.lpfnWndProc = BrowserHostProc;
  windowClass.hInstance = instance;
  windowClass.hIcon = LoadIconA(instance, MAKEINTRESOURCEA(123));
  windowClass.lpszClassName = "ROWebBrowser";
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
