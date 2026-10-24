#include "Client/Mouse.h"
#include "UI/InputFocus.h"
#include "UI/UIDropDownBox.h"
#include "UI/UIWindow.h"
#include "UI/WindowCoordinates.h"
#include "UI/WindowManagerWindows.h"
#include "UI/WindowMouseHit.h"
#include "UI/WindowOrdering.h"

#include <bit>

// OFFSET: 0x00A91B30
// FUNCTION: fn_17168
UIWindow* WindowMouseHit::FindUid(std::uint32_t uid) {
  for (UIWindow* window : this->rendered) {
    if (UIWindow* found = window->FindUid(uid)) {
      return found;
    }
  }
  return nullptr;
}

// OFFSET: 0x00A91C70
// FUNCTION: fn_17171
UIWindow* WindowMouseHit::HitTest(int x, int y) {
  // The reference keeps a pointer to the list sentinel's previous link; it
  // never advances that link on rejection. EDI also becomes the hit result,
  // so a miss retries the same top window with y=0. Do not replace this with
  // conventional reverse traversal: a top window that keeps missing can loop.
  while (!this->rendered.empty()) {
    UIWindow* hit = this->rendered.back()->HitTest(x, y);
    y = static_cast<int>(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(hit)));
    if (hit && hit->geometry.visible != 0) {
      this->previous = this->current;
      this->current = hit->Uid();
      if (this->previous != this->current) {
        hit->Slot47();
        // Enter callbacks may mutate the previous UID or rendered tree.
        if (UIWindow* old = this->FindUid(this->previous)) {
          old->Slot48();
        }
      }
      return hit;
    }
  }

  for (auto entry = this->overlays.rbegin(); entry != this->overlays.rend(); ++entry) {
    UIWindow* hit = entry->second->HitTest(x, y);
    if (hit && hit->geometry.visible != 0) {
      return hit;
    }
  }

  this->previous = this->current;
  this->current = 0;
  if (this->previous != 0) {
    if (UIWindow* old = this->FindUid(this->previous)) {
      old->Slot48();
    }
  }
  return nullptr;
}

// OFFSET: 0x00AA4730
// FUNCTION: fn_17222
bool WindowMouseCapture::Dispatch() {
  if (!this->capture) {
    return false;
  }

  const int x = this->mouse.x;
  const int y = this->mouse.y;
  int left;
  int top;
  UIWindow* window = static_cast<UIWindow*>(this->capture);
  WindowScreenPosition(window, left, top);
  const int localX = std::bit_cast<int>(static_cast<unsigned>(x) - static_cast<unsigned>(left));
  const int localY = std::bit_cast<int>(static_cast<unsigned>(y) - static_cast<unsigned>(top));

  if (this->mouse.buttons[0] == 1) {
    UIWindow* root = window->Root();
    if (root && root->Slot02()) {
      this->ordering.Promote(root);
    }

    static_cast<UIWindow*>(this->capture)->Slot25(localX, localY);
  }

  if (this->mouse.buttons[0] == 4 && (this->doubleClickGate == 0 || this->alternateDoubleClickGate == 0)) {
    static_cast<UIWindow*>(this->capture)->Slot25(localX, localY);
    static_cast<UIWindow*>(this->capture)->Slot26(localX, localY);
  }

  static_cast<UIWindow*>(this->capture)->ResetCursor(localX, localY);

  if (this->previousX == x && this->previousY == y) {
    static_cast<UIWindow*>(this->capture)->Slot28(localX, localY);
  } else {
    static_cast<UIWindow*>(this->capture)->Slot27(localX, localY);
  }

  if (this->mouse.wheel != 0) {
    static_cast<UIWindow*>(this->capture)->Slot35(this->mouse.wheel);
  }

  if (this->mouse.buttons[0] == 3) {
    static_cast<UIWindow*>(this->capture)->Slot31(localX, localY);
  }

  return true;
}

// OFFSET: 0x00AA4730
// FUNCTION: fn_17222
// Modal filtering follows hit enter/leave callbacks. A miss retains hover;
// right release precedes left release. Close requests require the real manager.
int WindowMouseUncaptured::Dispatch() {
  const int x = this->mouse.x;
  const int y = this->mouse.y;
  UIWindow* window = this->hit.HitTest(x, y);

  if (
    !this->modal.empty() && this->modal.back() && window && window != this->modal.back() &&
    !window->IsDescendantOf(this->modal.back())
  ) {
    window = nullptr;
  }

  if (this->mouse.buttons[0] == 1) {
    SetPressedWindowRoot(this->pressedRoot, window);
  }

  int handled = 0;
  if (window) {
    handled = window->Slot08() ? 2 : 1;
    int left;
    int top;
    WindowScreenPosition(window, left, top);
    const int localX = std::bit_cast<int>(static_cast<unsigned>(x) - static_cast<unsigned>(left));
    const int localY = std::bit_cast<int>(static_cast<unsigned>(y) - static_cast<unsigned>(top));

    if (this->mouse.buttons[0] == 4 && (this->doubleClickGate == 0 || this->alternateDoubleClickGate == 0)) {
      window->Slot26(localX, localY);
    }

    if (this->mouse.buttons[1] == 4) {
      window->Slot34(localX, localY);
    }

    if (this->mouse.buttons[0] == 1) {
      UIWindow* root = window->Root();
      if (root && root->Slot02()) {
        this->ordering.Promote(root);
      }

      CUIDropDownBox* popup = this->dropdown.popup;
      if (popup && window != popup && root != popup && !popup->HasOwnerChild(window)) {
        this->dropdown.Close();
      }

      window->Slot25(localX, localY);
    }

    if (this->mouse.buttons[1] == 1) {
      UIWindow* root = window->Root();
      if (root && root->Slot02()) {
        this->ordering.Promote(root);
      }

      window->Slot32(localX, localY);
    }

    window->ResetCursor(localX, localY);
    if (this->previousX == x && this->previousY == y) {
      window->Slot28(localX, localY);
    } else {
      window->Slot27(localX, localY);
    }

    if (this->mouse.wheel != 0) {
      window->Slot35(this->mouse.wheel);
    }

    if (this->mouse.buttons[1] == 3) {
      window->Slot33(localX, localY);
    }

    if (this->mouse.buttons[0] == 3) {
      window->Slot31(localX, localY);
    }
  }

  if (this->mouse.buttons[0] == 1) {
    if (this->dismiss26 && (!window || (window != this->dismiss26 && !window->IsDescendantOf(this->dismiss26)))) {
      this->windows.CloseWindow(26);
    }

    if (this->dismiss260) {
      if (!window || (window != this->dismiss260 && !window->IsDescendantOf(this->dismiss260))) {
        this->windows.CloseWindow(260);
        if (!window) {
          this->dropdown.Close();
        }
      }
    } else if (!window) {
      this->dropdown.Close();
    }
  }

  for (unsigned index = 0; index < this->refresh.size(); ++index) {
    UIWindow* refreshed = *this->refresh[index];
    if (refreshed && (index != 5 || this->refreshGate == 0)) {
      refreshed->Invalidate();
    }
  }

  if (window) {
    if (this->hover && this->hover != window) {
      int left;
      int top;
      WindowScreenPosition(static_cast<UIWindow*>(this->hover), left, top);
      const int localX = std::bit_cast<int>(static_cast<unsigned>(x) - static_cast<unsigned>(left));
      const int localY = std::bit_cast<int>(static_cast<unsigned>(y) - static_cast<unsigned>(top));
      static_cast<UIWindow*>(this->hover)->Slot27(localX, localY);
      static_cast<UIWindow*>(this->hover)->ResetCursor(localX, localY);
    }

    this->hover = window;
  }

  return handled;
}
