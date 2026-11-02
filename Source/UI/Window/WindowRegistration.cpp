#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowRegistration.h"

// OFFSET: 0x00A8B060
// FUNCTION: fn_17120
void WindowRegistration::Add(UIWindow* window) {
  for (UIWindow* existing : this->windows) {
    if (existing == window) {
      return;
    }
  }

  this->windows.push_back(window);
}

// OFFSET: 0x00AA7200
// FUNCTION: fn_17236
void UIWindowMgr::UnregisterWindow(UIWindow* window) {
  for (auto entry = this->registeredWindows.begin(); entry != this->registeredWindows.end(); ++entry) {
    if (*entry == window) {
      this->registeredWindows.erase(entry);
      return;
    }
  }
}

// OFFSET: 0x00AA6650
// FUNCTION: fn_17231
void UIWindowMgr::InvalidateRegisteredWindows() {
  for (UIWindow* window : this->registeredWindows) {
    window->Invalidate();
  }

  this->registeredWindows.clear();
}
