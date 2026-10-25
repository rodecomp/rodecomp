#include "Client/Browser/BrowserInterface.h"

#include <memory>
#include <ocidl.h>

// OFFSET: 0x00AB2CD0
// FUNCTION: fn_45437
bool CreateBrowserInterface(IWebBrowser2*& browser, HWND& window) {
  const HRESULT result =
    CoCreateInstance(CLSID_WebBrowser, nullptr, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IWebBrowser2, reinterpret_cast<void**>(&browser));

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

  if (browser) {
    browser->get_HWND(reinterpret_cast<SHANDLE_PTR*>(&window));
  }

  return true;
}

// OFFSET: 0x00AB2C10
// FUNCTION: fn_45436
bool CreateExternalBrowserInterface(IWebBrowser2*& browser, HWND& window, HWND const& host) {
  const HRESULT result = CoCreateInstance(CLSID_InternetExplorer, nullptr, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, reinterpret_cast<void**>(&browser));

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

  if (browser) {
    browser->get_HWND(reinterpret_cast<SHANDLE_PTR*>(&window));
    SetWindowLongA(window, GWL_STYLE, WS_CHILD);
    SetParent(window, host);
    browser->put_Top(0);
    browser->put_Left(0);
    browser->put_StatusBar(VARIANT_FALSE);
  }

  return true;
}

// OFFSET: 0x00AB2D50
// FUNCTION: fn_17418
bool NavigateBrowser(IWebBrowser2* const& browser, HWND const& host, HWND const& title, std::string address) {
  if (!browser) {
    return false;
  }

  const auto length = address.size();
  auto converted = std::make_unique_for_overwrite<wchar_t[]>(length + 1);
  MultiByteToWideChar(CP_ACP, 0, address.c_str(), -1, converted.get(), static_cast<int>(length));
  converted[length] = L'\0';
  BSTR url = SysAllocString(converted.get());
  converted.reset();

  VARIANT optional;
  VariantInit(&optional);
  const HRESULT result = browser->Navigate(url, &optional, &optional, &optional, &optional);
  SysFreeString(url);
  ShowWindow(host, SW_SHOW);
  browser->put_Visible(VARIANT_TRUE);

  if (title) {
    SetWindowTextA(title, address.c_str());
  }

  if (FAILED(result)) {
    OutputDebugStringA("failed : Navigate() \n");
    return false;
  }

  return true;
}

// OFFSET: 0x00AB3910
// FUNCTION: fn_17427
HRESULT AdviseBrowserEvents(IUnknown* source, IUnknown* sink, REFIID events, DWORD* cookie) {
  if (!source) {
    return E_INVALIDARG;
  }

  IConnectionPointContainer* container = nullptr;
  IConnectionPoint* point = nullptr;
  HRESULT result = source->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&container));

  if (SUCCEEDED(result)) {
    result = container->FindConnectionPoint(events, &point);
    if (SUCCEEDED(result)) {
      result = point->Advise(sink, cookie);
    }
  }

  if (point) {
    point->Release();
  }

  if (container) {
    container->Release();
  }

  return result;
}

// OFFSET: 0x00AB39E0
// FUNCTION: fn_17428
HRESULT UnadviseBrowserEvents(IUnknown* source, REFIID events, DWORD cookie) {
  if (!source) {
    return E_INVALIDARG;
  }

  IConnectionPointContainer* container = nullptr;
  IConnectionPoint* point = nullptr;
  HRESULT result = source->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&container));

  if (SUCCEEDED(result)) {
    result = container->FindConnectionPoint(events, &point);
    if (SUCCEEDED(result)) {
      result = point->Unadvise(cookie);
    }
  }

  if (point) {
    point->Release();
  }

  if (container) {
    container->Release();
  }

  return result;
}
