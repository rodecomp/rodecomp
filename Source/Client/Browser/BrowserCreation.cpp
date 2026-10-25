#include "Client/Browser/BrowserCreation.h"
#include "Client/Browser/BrowserInterface.h"

// OFFSET: 0x00AB22B0
// FUNCTION: fn_17413
bool BrowserCreation::Create(HWND parent, HINSTANCE instance, int width, int height) {
  if (!CreateBrowserInterface(this->browser, this->browserWindow)) {
    return false;
  }

  if (!this->window.Create(parent, instance, width, height)) {
    return false;
  }

  if (!this->attachment.Attach()) {
    return false;
  }

  return this->addressBar.Create(width, 22);
}
