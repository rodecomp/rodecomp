#include "Client/Browser/BrowserCommandTarget.h"

#include <mshtmhst.h>

// OFFSET: 0x00AB1A80
// FUNCTION: fn_17379
HRESULT STDMETHODCALLTYPE BrowserCommandTarget::QueryInterface(REFIID iid, void** output) {
  return this->m_site.QueryInterface(iid, output);
}

// OFFSET: 0x00AB15D0
// FUNCTION: fn_17340
ULONG STDMETHODCALLTYPE BrowserCommandTarget::AddRef() {
  return 1;
}

// OFFSET: 0x00AB1C60
// FUNCTION: fn_17386
ULONG STDMETHODCALLTYPE BrowserCommandTarget::Release() {
  return 1;
}

// OFFSET: 0x00AB1C50
// FUNCTION: fn_17385
HRESULT STDMETHODCALLTYPE BrowserCommandTarget::QueryStatus(const GUID*, ULONG, OLECMD[], OLECMDTEXT*) {
  return S_OK;
}

// OFFSET: 0x00AB16A0
// FUNCTION: fn_17353
HRESULT STDMETHODCALLTYPE BrowserCommandTarget::Exec(const GUID* group, DWORD command, DWORD, VARIANT*, VARIANT*) {
  // STATIC: 0x011B0880
  if (!group || !IsEqualGUID(*group, CGID_DocHostCommandHandler)) {
    return OLECMDERR_E_UNKNOWNGROUP;
  }

  if (command == OLECMDID_SHOWSCRIPTERROR || command == OLECMDID_SHOWMESSAGE) {
    return S_OK;
  }

  return OLECMDERR_E_NOTSUPPORTED;
}
