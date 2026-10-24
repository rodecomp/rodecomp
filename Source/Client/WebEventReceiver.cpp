#include "Client/BrowserRegistry.h"
#include "Client/WebEventReceiver.h"

#include <mshtml.h>

// OFFSET: 0x00AB3710
// FUNCTION: fn_17424
CWebEventReceiver::CWebEventReceiver(BrowserRegistry& registry)
    : m_references(0), m_browser(nullptr), m_browserCookie(0), m_document(nullptr), m_documentCookie(0),
      m_host(nullptr), m_registry(registry) {
  OutputDebugStringA("CWebEventReceiver() \n");
}

// OFFSET: 0x00AB38A0
CWebEventReceiver::~CWebEventReceiver() {
  OutputDebugStringA("~CWebEventReceiver() \n");
}

// OFFSET: 0x00AB3AE0
// FUNCTION: fn_17432
bool CWebEventReceiver::Register(IWebBrowser2* browser, HWND host) {
  this->m_browser = browser;
  this->m_host = host;
  this->m_browser->AddRef();
  return SUCCEEDED(AdviseBrowserEvents(this->m_browser, this, DIID_DWebBrowserEvents2, &this->m_browserCookie));
}

// OFFSET: 0x00AB3F30
// FUNCTION: fn_17434
HRESULT STDMETHODCALLTYPE CWebEventReceiver::QueryInterface(REFIID iid, void** output) {
  if (iid != IID_IUnknown && iid != IID_IDispatch) {
    return E_NOINTERFACE;
  }

  *output = static_cast<IDispatch*>(this);
  this->AddRef();
  return S_OK;
}

// OFFSET: 0x00AB38F0
// FUNCTION: fn_17426
ULONG STDMETHODCALLTYPE CWebEventReceiver::AddRef() {
  return InterlockedIncrement(&this->m_references);
}

// OFFSET: 0x00AB4020
// FUNCTION: fn_17435
ULONG STDMETHODCALLTYPE CWebEventReceiver::Release() {
  const LONG references = InterlockedDecrement(&this->m_references);
  if (references <= 0) {
    delete this;
  }

  return static_cast<ULONG>(references);
}

// OFFSET: 0x00AB3AD0
// FUNCTION: fn_17431
HRESULT STDMETHODCALLTYPE CWebEventReceiver::GetTypeInfoCount(UINT*) {
  return S_OK;
}

// OFFSET: 0x00AB3AC0
// FUNCTION: fn_17430
HRESULT STDMETHODCALLTYPE CWebEventReceiver::GetTypeInfo(UINT, LCID, ITypeInfo**) {
  return S_OK;
}

// OFFSET: 0x00AB3AB0
// FUNCTION: fn_17429
HRESULT STDMETHODCALLTYPE CWebEventReceiver::GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*) {
  return S_OK;
}

void CWebEventReceiver::ReleaseDocument() {
  if (this->m_document) {
    UnadviseBrowserEvents(this->m_document, DIID_HTMLDocumentEvents2, this->m_documentCookie);
    this->m_document->Release();
    this->m_document = nullptr;
    this->m_documentCookie = 0;
  }
}

// OFFSET: 0x00AB3FA0
// FUNCTION: fn_45418
HRESULT CWebEventReceiver::Disconnect() {
  this->ReleaseDocument();
  if (this->m_browser) {
    this->m_browser->Stop();
    UnadviseBrowserEvents(this->m_browser, DIID_DWebBrowserEvents2, this->m_browserCookie);
    this->m_browser->Release();
    this->m_browser = nullptr;
  }

  this->m_registry.Remove(this->m_host);
  return S_OK;
}

// OFFSET: 0x00AB3B20
// FUNCTION: fn_17433
HRESULT STDMETHODCALLTYPE
CWebEventReceiver::Invoke(DISPID id, REFIID, LCID, WORD, DISPPARAMS* args, VARIANT*, EXCEPINFO*, UINT*) {
  if (id == 253) {
    return this->Disconnect();
  }

  if (id == 259) {
    IDispatch* browserIdentity = nullptr;
    if (FAILED(this->m_browser->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&browserIdentity)))) {
      return S_OK;
    }

    // DocumentComplete's pDisp is the second reverse-order VARIANT. The
    // reference retains browserIdentity on an identity mismatch.
    if (args->rgvarg[1].pdispVal != browserIdentity) {
      return S_OK;
    }

    this->ReleaseDocument();
    if (SUCCEEDED(this->m_browser->get_Document(&this->m_document)) && this->m_document) {
      AdviseBrowserEvents(this->m_document, this, DIID_HTMLDocumentEvents2, &this->m_documentCookie);
    }

    browserIdentity->Release();
    return S_OK;
  }

  if (id != -602 && id != static_cast<DISPID>(0x8001000A) && id != 1023) {
    return S_OK;
  }

  IHTMLEventObj* event = nullptr;
  if (FAILED(args->rgvarg[0].pdispVal->QueryInterface(IID_IHTMLEventObj, reinterpret_cast<void**>(&event)))) {
    return S_OK;
  }

  if (id == -602) {
    long key = 0;
    if (FAILED(event->get_keyCode(&key))) {
      return S_OK;
    }

    switch (key) {
    case 18:
    case 112:
    case 114:
    case 115:
    case 122:
      break;
    default:
      // Deliberately retained reference on the original non-filtered branch.
      return S_OK;
    }
  }

  if (id != 1023) {
    event->put_keyCode(0);
  }

  VARIANT cancelled;
  VariantInit(&cancelled);
  cancelled.vt = VT_BOOL;
  cancelled.boolVal = VARIANT_FALSE;
  event->put_returnValue(cancelled);
  event->Release();
  return id == -602 ? S_FALSE : S_OK;
}
