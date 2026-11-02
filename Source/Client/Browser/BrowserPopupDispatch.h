#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERPOPUPDISPATCH_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERPOPUPDISPATCH_H

#include <windows.h>
#include <ole2.h>

class BrowserPopupDispatch final : public IDispatch {
public:
  // INLINED
  explicit BrowserPopupDispatch(IUnknown& site) : m_site(site) {
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* count) override;
  HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT index, LCID locale, ITypeInfo** output) override;
  HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID iid, LPOLESTR* names, UINT count, LCID locale, DISPID* ids) override;
  HRESULT STDMETHODCALLTYPE
  Invoke(DISPID id, REFIID iid, LCID locale, WORD flags, DISPPARAMS* args, VARIANT* result, EXCEPINFO* error, UINT* argumentError) override;

private:
  IUnknown& m_site;
};

#endif
