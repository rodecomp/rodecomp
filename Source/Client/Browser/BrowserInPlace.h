#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERINPLACE_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERINPLACE_H

#include <windows.h>
#include <ole2.h>

class BrowserClientSite;

class BrowserInPlaceFrame final : public IOleInPlaceFrame {
public:
  // INLINED
  explicit BrowserInPlaceFrame(BrowserClientSite* site) : unknown_0004(0), m_site(site) {
  }
  ~BrowserInPlaceFrame();

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
  /* 0004 */ int unknown_0004;
  /* 0008 */ BrowserClientSite* m_site;
};

class BrowserInPlaceSite final : public IOleInPlaceSite {
public:
  // INLINED
  explicit BrowserInPlaceSite(BrowserClientSite* site) : unknown_0004(0), m_frame(new BrowserInPlaceFrame(site)), m_site(site) {
  }
  // INLINED
  ~BrowserInPlaceSite() {
    delete this->m_frame;
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
  /* 0004 */ int unknown_0004;
  /* 0008 */ BrowserInPlaceFrame* m_frame;
  /* 000C */ BrowserClientSite* m_site;
};

#endif
