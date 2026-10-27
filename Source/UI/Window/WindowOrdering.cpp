#include "UI/Window/UIRoulletteWnd.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"

#include <algorithm>

// OFFSET: 0x00A966E0
// FUNCTION: fn_17184
void UIWindowMgr::InvalidateAll() {
  for (UIWindow* window : this->rendered) {
    window->InvalidateChildren();
  }
}

// OFFSET: 0x00A96710
// FUNCTION: fn_17185
void UIWindowMgr::RefreshClanWnd() {
  if (this->window_0444) {
    this->window_0444->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A96730
// FUNCTION: fn_17186
void UIWindowMgr::RefreshGuildWindows() {
  if (this->window_0320) {
    this->window_0320->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
  if (this->window_0324) {
    this->window_0324->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
  if (this->unknown_0328) {
    this->unknown_0328->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
  if (this->window_032C) {
    this->window_032C->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
  if (this->unknown_0330) {
    this->unknown_0330->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
  if (this->unknown_0334) {
    this->unknown_0334->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
  if (this->unknown_0338) {
    this->unknown_0338->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A96810
// FUNCTION: fn_17187
void UIWindowMgr::RefreshRouletteWnd() {
  if (this->unknown_0464) {
    this->unknown_0464->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A96830
// FUNCTION: fn_17188
void UIWindowMgr::EnableRouletteWnd() {
  if (this->unknown_0464) {
    this->unknown_0464->m_enabled = 1;
  }
}

// OFFSET: 0x00A96850
// FUNCTION: fn_17189
char UIWindowMgr::UpdateSnapRectangles() {
  this->snapRectangles.clear();

  const ServiceOptions* options = reinterpret_cast<const ServiceOptions*>(this);
  if (options->Get(141) == 0) {
    return 0;
  }

  for (UIWindow* window : this->rendered) {
    if (window->geometry.visible == 0) {
      continue;
    }

    bool excluded = false;
    for (UIWindow* ex : this->snapExcluded) {
      if (ex == window) {
        excluded = true;
        break;
      }
    }
    if (excluded) {
      continue;
    }

    if (options->Get(141) != 0 && window) {
      this->snapRectangles[window].Set(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height);
    }
  }

  return 1;
}

// OFFSET: 0x00A976C0
// FUNCTION: fn_17206
void WindowOrdering::Promote(UIWindow* window) {
  if (!window || (this->modal != 0 && this->modalWindow != window)) {
    return;
  }

  if (this->windows.LookupWindow(278) || this->windows.LookupWindow(4) || this->windows.LookupWindow(199) || this->windows.LookupWindow(200)) {
    return;
  }

  int companion = -1;

  if (window->geometry.id == 12) {
    companion = 234;
  } else if (window->geometry.id == 82) {
    companion = 250;
  }

  if (companion != -1) {
    const auto entry = std::find_if(this->rendered.begin(), this->rendered.end(), [&](const UIWindow* value) { return value->geometry.id == companion; });

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
