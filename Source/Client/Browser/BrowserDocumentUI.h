#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERDOCUMENTUI_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERDOCUMENTUI_H

#include <windows.h>
#include <ole2.h>
#include <mshtmhst.h>

class BrowserDocumentUI final : public IDocHostUIHandler {
public:
  explicit BrowserDocumentUI(IUnknown& site) : m_site(site) {
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD id, POINT* point, IUnknown* reserved, IDispatch* dispatch) override;
  HRESULT STDMETHODCALLTYPE GetHostInfo(DOCHOSTUIINFO* info) override;
  HRESULT STDMETHODCALLTYPE
  ShowUI(DWORD id, IOleInPlaceActiveObject* active, IOleCommandTarget* commands, IOleInPlaceFrame* frame, IOleInPlaceUIWindow* document) override;
  HRESULT STDMETHODCALLTYPE HideUI() override;
  HRESULT STDMETHODCALLTYPE UpdateUI() override;
  HRESULT STDMETHODCALLTYPE EnableModeless(BOOL enable) override;
  HRESULT STDMETHODCALLTYPE OnDocWindowActivate(BOOL activate) override;
  HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(BOOL activate) override;
  HRESULT STDMETHODCALLTYPE ResizeBorder(const RECT* border, IOleInPlaceUIWindow* window, BOOL frame) override;
  HRESULT STDMETHODCALLTYPE TranslateAccelerator(MSG* message, const GUID* group, DWORD command) override;
  HRESULT STDMETHODCALLTYPE GetOptionKeyPath(LPOLESTR* path, DWORD reserved) override;
  HRESULT STDMETHODCALLTYPE GetDropTarget(IDropTarget* target, IDropTarget** replacement) override;
  HRESULT STDMETHODCALLTYPE GetExternal(IDispatch** dispatch) override;
  HRESULT STDMETHODCALLTYPE TranslateUrl(DWORD flags, OLECHAR* url, OLECHAR** translated) override;
  HRESULT STDMETHODCALLTYPE FilterDataObject(IDataObject* data, IDataObject** filtered) override;

private:
  IUnknown& m_site;
};

#endif
