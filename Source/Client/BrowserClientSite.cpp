#include "Client/BrowserClientSite.h"

// OFFSET: 0x00AB1310
// FUNCTION: fn_17334
BrowserClientSite::BrowserClientSite(
  HWND window,
  IUnknown& browser,
  BrowserFactory& factory,
  const int& width,
  const int& height
)
    : m_window(window), m_browser(&browser), m_events(*this, factory, width, height),
      m_inPlace(*this, this->m_window, this->m_browser), m_documentUI(*this), m_commands(*this) {
  this->m_browser->AddRef();
}

// OFFSET: 0x00AB14E0
// FUNCTION: fn_17339
BrowserClientSite::~BrowserClientSite() {
  IOleObject* object;
  if (SUCCEEDED(this->m_browser->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&object)))) {
    object->Close(OLECLOSE_NOSAVE);
    object->Release();
  }

  this->m_browser->Release();
}

// OFFSET: 0x00AB1B20
// FUNCTION: fn_17384
HRESULT STDMETHODCALLTYPE BrowserClientSite::QueryInterface(REFIID iid, void** output) {
  if (iid == IID_IUnknown || iid == IID_IOleClientSite) {
    *output = static_cast<IOleClientSite*>(this);
  } else if (iid == IID_IOleInPlaceSite) {
    *output = static_cast<IOleInPlaceSite*>(&this->m_inPlace);
  } else if (iid == IID_IDocHostUIHandler) {
    *output = static_cast<IDocHostUIHandler*>(&this->m_documentUI);
  } else if (iid == IID_IDispatch) {
    *output = &this->m_events;
  } else if (iid == IID_IOleCommandTarget) {
    *output = static_cast<IOleCommandTarget*>(&this->m_commands);
  } else {
    // Unlike standard COM helpers, the original does not clear a failed output
    // and does not AddRef a successful result.
    return E_NOINTERFACE;
  }

  return S_OK;
}

// OFFSET: 0x00AB1620
// FUNCTION: fn_17345
ULONG STDMETHODCALLTYPE BrowserClientSite::AddRef() {
  return 1;
}

// OFFSET: 0x00AB1CB0
// FUNCTION: fn_17391
ULONG STDMETHODCALLTYPE BrowserClientSite::Release() {
  return 1;
}

// OFFSET: 0x00AB1D00
// FUNCTION: fn_17396
HRESULT STDMETHODCALLTYPE BrowserClientSite::SaveObject() {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB17A0
// FUNCTION: fn_17361
HRESULT STDMETHODCALLTYPE BrowserClientSite::GetMoniker(DWORD, DWORD, IMoniker**) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1720
// FUNCTION: fn_17356
HRESULT STDMETHODCALLTYPE BrowserClientSite::GetContainer(IOleContainer** container) {
  *container = nullptr;
  return E_NOINTERFACE;
}

// OFFSET: 0x00AB1D70
// FUNCTION: fn_17403
HRESULT STDMETHODCALLTYPE BrowserClientSite::ShowObject() {
  return S_OK;
}

// OFFSET: 0x00AB1A50
// FUNCTION: fn_17376
HRESULT STDMETHODCALLTYPE BrowserClientSite::OnShowWindow(BOOL) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1CE0
// FUNCTION: fn_17394
HRESULT STDMETHODCALLTYPE BrowserClientSite::RequestNewObjectLayout() {
  return E_NOTIMPL;
}
