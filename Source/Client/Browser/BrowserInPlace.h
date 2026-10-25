#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERINPLACE_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERINPLACE_H

#include <windows.h>
#include <ole2.h>

class BrowserInPlaceFrame final : public IOleInPlaceFrame {
public:
  BrowserInPlaceFrame(IUnknown& site, HWND const& window) : m_site(site), m_window(window) {
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE GetWindow(HWND* output) override;
  HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL enter) override;
  HRESULT STDMETHODCALLTYPE GetBorder(RECT* border) override;
  HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS widths) override;
  HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS widths) override;
  HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject* object, LPCOLESTR name) override;
  HRESULT STDMETHODCALLTYPE InsertMenus(HMENU menu, LPOLEMENUGROUPWIDTHS widths) override;
  HRESULT STDMETHODCALLTYPE SetMenu(HMENU menu, HOLEMENU descriptor, HWND active) override;
  HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU menu) override;
  HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR text) override;
  HRESULT STDMETHODCALLTYPE EnableModeless(BOOL enable) override;
  HRESULT STDMETHODCALLTYPE TranslateAccelerator(MSG* message, WORD id) override;

private:
  IUnknown& m_site;
  HWND const& m_window;
};

class BrowserInPlaceSite final : public IOleInPlaceSite {
public:
  BrowserInPlaceSite(IUnknown& site, HWND const& window, IUnknown* const& browser) : m_site(site), m_window(window), m_browser(browser), m_frame(site, window) {
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE GetWindow(HWND* output) override;
  HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL enter) override;
  HRESULT STDMETHODCALLTYPE CanInPlaceActivate() override;
  HRESULT STDMETHODCALLTYPE OnInPlaceActivate() override;
  HRESULT STDMETHODCALLTYPE OnUIActivate() override;
  HRESULT STDMETHODCALLTYPE
  GetWindowContext(IOleInPlaceFrame** frame, IOleInPlaceUIWindow** document, RECT* position, RECT* clip, OLEINPLACEFRAMEINFO* info) override;
  HRESULT STDMETHODCALLTYPE Scroll(SIZE extent) override;
  HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL undoable) override;
  HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate() override;
  HRESULT STDMETHODCALLTYPE DiscardUndoState() override;
  HRESULT STDMETHODCALLTYPE DeactivateAndUndo() override;
  HRESULT STDMETHODCALLTYPE OnPosRectChange(const RECT* position) override;

private:
  IUnknown& m_site;
  HWND const& m_window;
  IUnknown* const& m_browser;
  BrowserInPlaceFrame m_frame;
};

#endif
