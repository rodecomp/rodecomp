#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERCOMMANDTARGET_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERCOMMANDTARGET_H

// clang-format off
#include <windows.h>
#include <ole2.h>
#include <docobj.h>
// clang-format on

// CWebBrowser's site owns this tear-off interface.
// Identity delegates to that site; AddRef/Release deliberately return one.
// The caller must keep the site alive for this interface's entire lifetime.
class BrowserCommandTarget final : public IOleCommandTarget {
public:
  explicit BrowserCommandTarget(IUnknown& site) : m_site(site) {
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE QueryStatus(const GUID* group, ULONG count, OLECMD commands[], OLECMDTEXT* text) override;
  HRESULT STDMETHODCALLTYPE
  Exec(const GUID* group, DWORD command, DWORD options, VARIANT* input, VARIANT* output) override;

private:
  IUnknown& m_site;
};

#endif
