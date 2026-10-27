#include "UI/Window/UIWindow.h"
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
void WindowRegistration::Remove(UIWindow* window) {
  for (auto entry = this->windows.begin(); entry != this->windows.end(); ++entry) {
    if (*entry == window) {
      this->windows.erase(entry);
      return;
    }
  }
}

// OFFSET: 0x00AA6650
// FUNCTION: fn_17231
void WindowRegistration::InvalidateAll() {
  for (UIWindow* window : this->windows) {
    window->Invalidate();
  }

  this->windows.clear();
}
