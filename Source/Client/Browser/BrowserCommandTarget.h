#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCOMMANDTARGET_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCOMMANDTARGET_H

#include <windows.h>
#include <ole2.h>
#include <docobj.h>

class BrowserClientSite;

class BrowserCommandTarget final : public IOleCommandTarget {
public:
  // INLINED
  explicit BrowserCommandTarget(BrowserClientSite* site) : unknown_0004(0), m_site(site) {
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE QueryStatus(const GUID* group, ULONG count, OLECMD commands[], OLECMDTEXT* text) override;
  HRESULT STDMETHODCALLTYPE Exec(const GUID* group, DWORD command, DWORD options, VARIANT* input, VARIANT* output) override;

private:
  /* 0004 */ int unknown_0004;
  /* 0008 */ BrowserClientSite* m_site;
};

#endif
