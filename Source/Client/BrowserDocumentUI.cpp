#include "Client/BrowserDocumentUI.h"

// OFFSET: 0x00AB1AC0
// FUNCTION: fn_17381
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::QueryInterface(REFIID iid, void** output) {
  return this->m_site.QueryInterface(iid, output);
}

// OFFSET: 0x00AB15F0
// FUNCTION: fn_17342
ULONG STDMETHODCALLTYPE BrowserDocumentUI::AddRef() {
  return 1;
}

// OFFSET: 0x00AB1C80
// FUNCTION: fn_17388
ULONG STDMETHODCALLTYPE BrowserDocumentUI::Release() {
  return 1;
}

// OFFSET: 0x00AB1D60
// FUNCTION: fn_17402
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::ShowContextMenu(DWORD, POINT*, IUnknown*, IDispatch*) {
  return S_OK;
}

// OFFSET: 0x00AB1770
// FUNCTION: fn_17359
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::GetHostInfo(DOCHOSTUIINFO* info) {
  // Original x86 cbSize, even when compiling for a wider host. The two optional
  // string pointers are not initialized by this operation.
  info->cbSize = 20;
  info->dwFlags = 4;
  info->dwDoubleClick = 0;
  return S_OK;
}

// OFFSET: 0x00AB1D80
// FUNCTION: fn_17404
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::ShowUI(
  DWORD,
  IOleInPlaceActiveObject*,
  IOleCommandTarget*,
  IOleInPlaceFrame*,
  IOleInPlaceUIWindow*
) {
  return S_OK;
}

// OFFSET: 0x00AB1860
// FUNCTION: fn_17368
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::HideUI() {
  return S_OK;
}

// OFFSET: 0x00AB1DD0
// FUNCTION: fn_17408
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::UpdateUI() {
  return S_OK;
}

// OFFSET: 0x00AB1680
// FUNCTION: fn_17351
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::EnableModeless(BOOL) {
  return S_OK;
}

// OFFSET: 0x00AB19B0
// FUNCTION: fn_17371
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::OnDocWindowActivate(BOOL) {
  return S_OK;
}

// OFFSET: 0x00AB19C0
// FUNCTION: fn_17372
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::OnFrameWindowActivate(BOOL) {
  return S_OK;
}

// OFFSET: 0x00AB1CF0
// FUNCTION: fn_17395
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::ResizeBorder(const RECT*, IOleInPlaceUIWindow*, BOOL) {
  return S_OK;
}

// OFFSET: 0x00AB1D90
// FUNCTION: fn_17405
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::TranslateAccelerator(MSG*, const GUID*, DWORD) {
  return S_FALSE;
}

// OFFSET: 0x00AB17B0
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::GetOptionKeyPath(LPOLESTR*, DWORD) {
  return S_FALSE;
}

// OFFSET: 0x00AB1740
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::GetDropTarget(IDropTarget*, IDropTarget**) {
  return S_FALSE;
}

// OFFSET: 0x00AB1750
// FUNCTION: fn_17358
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::GetExternal(IDispatch** dispatch) {
  *dispatch = nullptr;
  return S_FALSE;
}

// OFFSET: 0x00AB1DB0
// FUNCTION: fn_17407
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::TranslateUrl(DWORD, OLECHAR*, OLECHAR** translated) {
  *translated = nullptr;
  return S_FALSE;
}

// OFFSET: 0x00AB16F0
// FUNCTION: fn_17354
HRESULT STDMETHODCALLTYPE BrowserDocumentUI::FilterDataObject(IDataObject*, IDataObject** filtered) {
  *filtered = nullptr;
  return S_FALSE;
}
