#include "Client/Browser/WebBrowser.h"

#include <atlbase.h>
#include <cstdlib>
#include <memory>
#include <ocidl.h>

// OFFSET: 0x00AB2CD0
// FUNCTION: fn_45437
bool CWebBrowser::CreateInterface() {
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

  return true;
}

// OFFSET: 0x00AB2C10
// FUNCTION: fn_45436
bool CWebBrowser::CreateExternalInterface() {
  const HRESULT result = CoCreateInstance(CLSID_InternetExplorer, nullptr, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, reinterpret_cast<void**>(&this->browser));

  if (FAILED(result)) {
    OutputDebugStringA("error : MakeWebBrowserInterface FAILED \n");
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
    SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
    SetParent(this->browserWindow, this->host);
    this->browser->put_Top(0);
    this->browser->put_Left(0);
    this->browser->put_ToolBar(0);
  }

  return true;
}

