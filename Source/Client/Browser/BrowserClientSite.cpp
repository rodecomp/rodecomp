#include "Client/Browser/BrowserClientSite.h"

// OFFSET: 0x00AB1310
// FUNCTION: fn_17334
BrowserClientSite::BrowserClientSite(HWND window, IUnknown* browser)
    : unknown_0004(0), m_window(window), m_browser(browser) {
  this->m_inPlace = new BrowserInPlaceSite(this);
  this->m_documentUI = new BrowserDocumentUI(this);
  this->m_events = new BrowserPopupDispatch(this);
  this->m_commands = new BrowserCommandTarget(this);
  this->m_browser->AddRef();
}

struct CCmdTargetEx {
  CCmdTargetEx();
  virtual ~CCmdTargetEx() = default;
};

// OFFSET: 0x00AB1480
// FUNCTION: fn_17335
CCmdTargetEx::CCmdTargetEx() = default;

struct CIDispatchEx {
  CIDispatchEx();
  virtual ~CIDispatchEx() = default;
};

// OFFSET: 0x00AB1490
// FUNCTION: fn_17336
CIDispatchEx::CIDispatchEx() = default;

struct CIDocHostUIHandlerEx {
  CIDocHostUIHandlerEx();
  virtual ~CIDocHostUIHandlerEx() = default;
};

// OFFSET: 0x00AB14A0
// FUNCTION: fn_17337
CIDocHostUIHandlerEx::CIDocHostUIHandlerEx() = default;

struct CIOleInPlaceFrameEx {
  CIOleInPlaceFrameEx();
  virtual ~CIOleInPlaceFrameEx() = default;
};

// OFFSET: 0x00AB14B0
// FUNCTION: fn_17338
CIOleInPlaceFrameEx::CIOleInPlaceFrameEx() = default;

// OFFSET: 0x00AB14E0
// FUNCTION: fn_17339
BrowserClientSite::~BrowserClientSite() {
  IOleObject* object;
  if (SUCCEEDED(this->m_browser->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&object)))) {
    object->Close(OLECLOSE_NOSAVE);
    object->Release();
  }

  delete this->m_commands;
  delete this->m_events;
  delete this->m_documentUI;
  delete this->m_inPlace;
  this->m_browser->Release();
}

// OFFSET: 0x00AB1B20
// FUNCTION: fn_17384
HRESULT STDMETHODCALLTYPE BrowserClientSite::QueryInterface(REFIID iid, void** output) {
  if (iid == IID_IUnknown || iid == IID_IOleClientSite) {
    *output = static_cast<IOleClientSite*>(this);
  } else if (iid == IID_IOleInPlaceSite) {
    *output = static_cast<IOleInPlaceSite*>(this->m_inPlace);
  } else if (iid == IID_IDocHostUIHandler) {
    *output = static_cast<IDocHostUIHandler*>(this->m_documentUI);
  } else if (iid == IID_IDispatch) {
    *output = static_cast<IDispatch*>(this->m_events);
  } else if (iid == IID_IOleCommandTarget) {
    *output = static_cast<IOleCommandTarget*>(this->m_commands);
  } else {
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
