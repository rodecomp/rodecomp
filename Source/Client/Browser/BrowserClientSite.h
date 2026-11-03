#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCLIENTSITE_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCLIENTSITE_H

#include "Client/Browser/BrowserCommandTarget.h"
#include "Client/Browser/BrowserDocumentUI.h"
#include "Client/Browser/BrowserInPlace.h"
#include "Client/Browser/BrowserPopupDispatch.h"

class BrowserClientSite final : public IOleClientSite {
public:
  BrowserClientSite(HWND window, IUnknown* browser);
  ~BrowserClientSite();
  BrowserClientSite(const BrowserClientSite&) = delete;
  BrowserClientSite& operator=(const BrowserClientSite&) = delete;

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE SaveObject() override;
  HRESULT STDMETHODCALLTYPE GetMoniker(DWORD assign, DWORD which, IMoniker** moniker) override;
  HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer** container) override;
  HRESULT STDMETHODCALLTYPE ShowObject() override;
  HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL show) override;
  HRESULT STDMETHODCALLTYPE RequestNewObjectLayout() override;

  friend class BrowserInPlaceFrame;
  friend class BrowserInPlaceSite;

private:
  /* 0004 */ int unknown_0004;
  /* 0008 */ HWND m_window;
  /* 000C */ IUnknown* m_browser;
  /* 0010 */ BrowserInPlaceSite* m_inPlace;
  /* 0014 */ BrowserDocumentUI* m_documentUI;
  /* 0018 */ BrowserPopupDispatch* m_events;
  /* 001C */ BrowserCommandTarget* m_commands;
};

#endif
