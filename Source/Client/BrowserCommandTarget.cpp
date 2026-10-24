#include "Client/BrowserCommandTarget.h"

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
  // The original reports success without reading or publishing command flags.
  return S_OK;
}

// OFFSET: 0x00AB16A0
// FUNCTION: fn_17353
HRESULT STDMETHODCALLTYPE BrowserCommandTarget::Exec(const GUID* group, DWORD command, DWORD, VARIANT*, VARIANT*) {
  // STATIC: 0x011B0880 (CGID_DocHostCommandHandler)
  static constexpr GUID documentHostCommands = {
    0xF38BC242, 0xB950, 0x11D1, {0x89, 0x18, 0x00, 0xC0, 0x4F, 0xC2, 0xC8, 0x36}
  };

  if (!group || !IsEqualGUID(*group, documentHostCommands)) {
    return OLECMDERR_E_UNKNOWNGROUP;
  }

  if (command == 40 || command == 41) {
    return S_OK;
  }

  return OLECMDERR_E_NOTSUPPORTED;
}
