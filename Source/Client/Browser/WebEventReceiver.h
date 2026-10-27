#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_WEBEVENTRECEIVER_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_WEBEVENTRECEIVER_H

#include <windows.h>
#include <ole2.h>
#include <exdisp.h>

struct BrowserRegistry;

class CWebEventReceiver final : public IDispatch {
public:
  explicit CWebEventReceiver(BrowserRegistry& registry);
  bool Register(IWebBrowser2* browser, HWND host);
  HRESULT Disconnect();

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* count) override;
  HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT index, LCID locale, ITypeInfo** output) override;
  HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID iid, LPOLESTR* names, UINT count, LCID locale, DISPID* ids) override;
  HRESULT STDMETHODCALLTYPE
  Invoke(DISPID id, REFIID iid, LCID locale, WORD flags, DISPPARAMS* args, VARIANT* result, EXCEPINFO* error, UINT* argumentError) override;

private:
  ~CWebEventReceiver();
  /* 0004 */ LONG m_references;
  /* 0008 */ IWebBrowser2* m_browser;
  /* 000C */ DWORD m_browserCookie;
  /* 0010 */ IDispatch* m_document;
  /* 0014 */ DWORD m_documentCookie;
  /* 0018 */ HWND m_host;
  BrowserRegistry& m_registry;
  void ReleaseDocument();
};

#endif
