#include "Client/Application/ApplicationWindow.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Browser/BrowserClientSite.h"
#include "Client/Input/Mouse.h"

#include <ocidl.h>

// OFFSET: 0x00AB2430
// FUNCTION: fn_45431
void CWebBrowser::DetachSite() {
  IOleObject* object;
  if (SUCCEEDED(this->browser->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&object)))) {
    IConnectionPointContainer* container;
    if (SUCCEEDED(object->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&container)))) {
      IConnectionPoint* point;
      if (SUCCEEDED(container->FindConnectionPoint(DIID_DWebBrowserEvents2, &point))) {
        point->Unadvise(this->cookie);
        point->Release();
      }
      container->Release();
    }
    object->Release();
  }

  delete this->site;
  this->site = nullptr;
}

// OFFSET: 0x00AB24D0
// FUNCTION: fn_45432
bool CWebBrowser::Close() {
  if (this->cursorState != 0) {
    SetCursor(nullptr);
    g_mouse.Reset();
    ShowCursor(FALSE);
    this->cursorState = 0;
  }

  if (this->site) {
    this->DetachSite();
  }

  if (this->externalEvents) {
    this->externalEvents->Release();
    this->externalEvents = nullptr;
  }

  if (this->browser) {
    this->browser->Stop();
    SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
    SetParent(this->browserWindow, hWnd);
    this->browser->Quit();
    this->browser->Release();
    this->browser = nullptr;
  }

  if (this->host) {
    DestroyWindow(this->host);
    this->host = nullptr;
  }

  return true;
}
