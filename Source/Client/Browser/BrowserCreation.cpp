#include "Client/Browser/WebBrowser.h"
#include "Contents/Configuration/ServiceConfiguration.h"

// OFFSET: 0x00AB22B0
// FUNCTION: fn_17413
bool CWebBrowser::Create(HWND parent, HINSTANCE instance, int width, int height) {
  const HRESULT result =
    CoCreateInstance(CLSID_WebBrowser, nullptr, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IWebBrowser2, reinterpret_cast<void**>(&this->browser));
  if (FAILED(result)) {
    OutputDebugStringA("error : MakeWebBrowserInterface_InProc FAILED \n");
    HLOCAL message = nullptr;
    FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0,
      reinterpret_cast<char*>(&message), 0, nullptr
    );
    OutputDebugStringA(static_cast<const char*>(LocalLock(message)));
    return false;
  }

  if (this->browser) {
    this->browser->get_HWND(reinterpret_cast<SHANDLE_PTR*>(&this->browserWindow));
  }

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

  if (!this->AttachSite()) {
    return false;
  }

  return this->CreateAddressBar(width, 22);
}
