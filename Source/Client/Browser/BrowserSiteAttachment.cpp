#include "Client/Browser/BrowserClientSite.h"
#include "Client/Browser/WebBrowser.h"

#include <new>
#include <ocidl.h>

// OFFSET: 0x00AB2AB0
// FUNCTION: fn_17417
bool CWebBrowser::AttachSite() {
  this->site = new BrowserClientSite(this->host, this->browser);
  IOleObject* object;
  HRESULT result = this->browser->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&object));
  if (SUCCEEDED(result)) {
    result = object->SetClientSite(this->site);
    if (SUCCEEDED(result)) {
      OleSetContainedObject(object, TRUE);
      RECT rectangle;
      GetClientRect(this->host, &rectangle);
      result = object->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr, this->site, 0, this->host, &rectangle);
      if (SUCCEEDED(result)) {
        const int width = rectangle.right - rectangle.left;
        const int height = rectangle.bottom - rectangle.top;
        this->Resize(width, height);
        IConnectionPointContainer* container;
        result = object->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&container));
        if (SUCCEEDED(result)) {
          IConnectionPoint* point;
          result = container->FindConnectionPoint(DIID_DWebBrowserEvents2, &point);
          if (SUCCEEDED(result)) {
            result = point->Advise(this->site, &this->cookie);
            point->Release();
          }
          container->Release();
        }
      }
    }

    object->Release();
  }

  return SUCCEEDED(result);
}

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
