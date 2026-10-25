#include "Client/Browser/BrowserInPlace.h"

// OFFSET: 0x00AB1AE0
// FUNCTION: fn_17382
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::QueryInterface(REFIID iid, void** output) {
  return this->m_site.QueryInterface(iid, output);
}

// OFFSET: 0x00AB1600
// FUNCTION: fn_17343
ULONG STDMETHODCALLTYPE BrowserInPlaceFrame::AddRef() {
  return 1;
}

// OFFSET: 0x00AB1C90
// FUNCTION: fn_17389
ULONG STDMETHODCALLTYPE BrowserInPlaceFrame::Release() {
  return 1;
}

// OFFSET: 0x00AB17E0
// FUNCTION: fn_17365
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::GetWindow(HWND* output) {
  *output = this->m_window;
  return S_OK;
}

// OFFSET: 0x00AB1640
// FUNCTION: fn_17347
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::ContextSensitiveHelp(BOOL) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1710
// FUNCTION: fn_17355
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::GetBorder(RECT*) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1CD0
// FUNCTION: fn_17393
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::RequestBorderSpace(LPCBORDERWIDTHS) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1D30
// FUNCTION: fn_17399
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::SetBorderSpace(LPCBORDERWIDTHS) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1D20
// FUNCTION: fn_17398
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::SetActiveObject(IOleInPlaceActiveObject*, LPCOLESTR) {
  return S_OK;
}

// OFFSET: 0x00AB1870
// FUNCTION: fn_17369
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::InsertMenus(HMENU, LPOLEMENUGROUPWIDTHS) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1D40
// FUNCTION: fn_17400
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::SetMenu(HMENU, HOLEMENU, HWND) {
  return S_OK;
}

// OFFSET: 0x00AB1CC0
// FUNCTION: fn_17392
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::RemoveMenus(HMENU) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1D50
// FUNCTION: fn_17401
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::SetStatusText(LPCOLESTR) {
  return S_OK;
}

// OFFSET: 0x00AB1690
// FUNCTION: fn_17352
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::EnableModeless(BOOL) {
  return S_OK;
}

// OFFSET: 0x00AB1DA0
// FUNCTION: fn_17406
HRESULT STDMETHODCALLTYPE BrowserInPlaceFrame::TranslateAccelerator(MSG*, WORD) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1B00
// FUNCTION: fn_17383
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::QueryInterface(REFIID iid, void** output) {
  return this->m_site.QueryInterface(iid, output);
}

// OFFSET: 0x00AB1610
// FUNCTION: fn_17344
ULONG STDMETHODCALLTYPE BrowserInPlaceSite::AddRef() {
  return 1;
}

// OFFSET: 0x00AB1CA0
// FUNCTION: fn_17390
ULONG STDMETHODCALLTYPE BrowserInPlaceSite::Release() {
  return 1;
}

// OFFSET: 0x00AB1800
// FUNCTION: fn_17366
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::GetWindow(HWND* output) {
  *output = this->m_window;
  return S_OK;
}

// OFFSET: 0x00AB1650
// FUNCTION: fn_17348
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::ContextSensitiveHelp(BOOL) {
  return S_OK;
}

// OFFSET: 0x00AB1630
// FUNCTION: fn_17346
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::CanInPlaceActivate() {
  return S_OK;
}

// OFFSET: 0x00AB19D0
// FUNCTION: fn_17373
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::OnInPlaceActivate() {
  return S_OK;
}

// OFFSET: 0x00AB1A60
// FUNCTION: fn_17377
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::OnUIActivate() {
  return S_OK;
}

// OFFSET: 0x00AB1820
// FUNCTION: fn_17367
HRESULT STDMETHODCALLTYPE
BrowserInPlaceSite::GetWindowContext(IOleInPlaceFrame** frame, IOleInPlaceUIWindow** document, RECT*, RECT*, OLEINPLACEFRAMEINFO* info) {
  *frame = &this->m_frame;
  *document = nullptr;
  info->fMDIApp = FALSE;
  info->hwndFrame = this->m_window;
  info->haccel = nullptr;
  info->cAccelEntries = 0;

  return S_OK;
}

// OFFSET: 0x00AB1D10
// FUNCTION: fn_17397
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::Scroll(SIZE) {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1A70
// FUNCTION: fn_17378
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::OnUIDeactivate(BOOL) {
  return S_OK;
}

// OFFSET: 0x00AB19E0
// FUNCTION: fn_17374
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::OnInPlaceDeactivate() {
  return S_OK;
}

// OFFSET: 0x00AB1670
// FUNCTION: fn_17350
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::DiscardUndoState() {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB1660
// FUNCTION: fn_17349
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::DeactivateAndUndo() {
  return E_NOTIMPL;
}

// OFFSET: 0x00AB19F0
// FUNCTION: fn_17375
HRESULT STDMETHODCALLTYPE BrowserInPlaceSite::OnPosRectChange(const RECT* position) {
  IOleObject* object;
  HRESULT result = this->m_browser->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&object));
  if (SUCCEEDED(result)) {
    IOleInPlaceObject* inPlace;
    result = object->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&inPlace));
    if (SUCCEEDED(result)) {
      inPlace->SetObjectRects(position, position);
      inPlace->Release();
    }
  }

  return S_OK;
}
