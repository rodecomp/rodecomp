#include "Client/Browser/WebBrowser.h"
#include "Client/Browser/BrowserFactory.h"
#include "Client/Browser/BrowserPopupDispatch.h"

#include "Utility/Math/ScalarConversion.h"
#include <exdispid.h>
#include <cwchar>
#include <memory>

// OFFSET: 0x00AB1AA0
// FUNCTION: fn_17380
HRESULT STDMETHODCALLTYPE BrowserPopupDispatch::QueryInterface(REFIID iid, void** output) {
  return this->m_site.QueryInterface(iid, output);
}

// OFFSET: 0x00AB15E0
// FUNCTION: fn_17341
ULONG STDMETHODCALLTYPE BrowserPopupDispatch::AddRef() {
  return 1;
}

// OFFSET: 0x00AB1C70
// FUNCTION: fn_17387
ULONG STDMETHODCALLTYPE BrowserPopupDispatch::Release() {
  return 1;
}

// OFFSET: 0x00AB17D0
// FUNCTION: fn_17364
HRESULT STDMETHODCALLTYPE BrowserPopupDispatch::GetTypeInfoCount(UINT*) {
  return S_OK;
}

// OFFSET: 0x00AB17C0
// FUNCTION: fn_17363
HRESULT STDMETHODCALLTYPE BrowserPopupDispatch::GetTypeInfo(UINT, LCID, ITypeInfo**) {
  return S_OK;
}

// OFFSET: 0x00AB1790
// FUNCTION: fn_17360
HRESULT STDMETHODCALLTYPE BrowserPopupDispatch::GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*) {
  return S_OK;
}

// OFFSET: 0x00AB1880
// FUNCTION: fn_17370
HRESULT STDMETHODCALLTYPE BrowserPopupDispatch::Invoke(DISPID id, REFIID, LCID, WORD, DISPPARAMS* args, VARIANT*, EXCEPINFO*, UINT*) {
  if (id != DISPID_NEWWINDOW3) {
    return S_OK;
  }

  const wchar_t* url = args->rgvarg[0].bstrVal;

  *args->rgvarg[3].pboolVal = 11;

  // STATIC: 0x01115448
  // GLOBAL: gl_13426
  const float scale = 0.7f;
  const int height = TruncateToInt(static_cast<float>(this->m_height) * scale);
  const int width = TruncateToInt(static_cast<float>(this->m_width) * scale);
  CWebBrowser* browser = this->m_factory.Create(width, height);

  if (!browser) {
    return S_OK;
  }

  const auto length = std::wcslen(url);
  auto converted = std::make_unique_for_overwrite<char[]>(length + 1);
  WideCharToMultiByte(CP_ACP, 0, url, -1, converted.get(), static_cast<int>(length), nullptr, nullptr);
  converted[length] = '\0';
  browser->Navigate(converted.get());

  return S_OK;
}
