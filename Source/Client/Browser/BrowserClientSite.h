#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCLIENTSITE_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCLIENTSITE_H

#include "Client/Browser/BrowserCommandTarget.h"
#include "Client/Browser/BrowserDocumentUI.h"
#include "Client/Browser/BrowserInPlace.h"
#include "Client/Browser/BrowserPopupDispatch.h"

class BrowserClientSite final : public IOleClientSite {
public:
  BrowserClientSite(HWND window, IUnknown& browser, BrowserFactory& factory, const int& width, const int& height);
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

private:
  HWND m_window;
  IUnknown* m_browser;
  BrowserPopupDispatch m_events;
  BrowserInPlaceSite m_inPlace;
  BrowserDocumentUI m_documentUI;
  BrowserCommandTarget m_commands;
};

#endif
