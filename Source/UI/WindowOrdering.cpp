#include "UI/UIWindow.h"
#include "UI/WindowManagerWindows.h"
#include "UI/WindowOrdering.h"

#include <algorithm>

// OFFSET: 0x00A976C0
// FUNCTION: fn_17206
void WindowOrdering::Promote(UIWindow* window) {
  if (!window || (this->modal != 0 && this->modalWindow != window)) {
    return;
  }

  if (
    this->windows.LookupWindow(278) || this->windows.LookupWindow(4) || this->windows.LookupWindow(199) ||
    this->windows.LookupWindow(200)
  ) {
    return;
  }

  int companion = -1;

  if (window->geometry.id == 12) {
    companion = 234;
  } else if (window->geometry.id == 82) {
    companion = 250;
  }

  if (companion != -1) {
    const auto entry = std::find_if(this->rendered.begin(), this->rendered.end(), [&](const UIWindow* value) {
      return value->geometry.id == companion;
    });

    if (entry != this->rendered.end()) {
      UIWindow* value = *entry;
      this->rendered.erase(entry);
      this->rendered.push_back(value);
    }
  }

  const auto entry = std::find(this->rendered.begin(), this->rendered.end(), window);

  if (entry != this->rendered.end()) {
    this->rendered.erase(entry);
    this->rendered.push_back(window);
  }
}

// OFFSET: 0x00A969D0
// FUNCTION: fn_17190
void WindowOrdering::InvalidateFrames(int& latch) {
  if (latch != 0) {
    return;
  }

  latch = 1;
  for (UIWindow* window : this->rendered) {
    if (window->Slot03()) {
      window->InvalidateChildren();
    }
  }
}
