#include "Client/Input/Mouse.h"
#include "UI/Window/InputFocus.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Window/UIWindow.h"
#include "UI/Layout/WindowCoordinates.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowMouseHit.h"
#include "UI/Window/WindowOrdering.h"

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
  while (!this->rendered.empty()) {
    UIWindow* hit = this->rendered.back()->HitTest(x, y);
    y = static_cast<int>(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(hit)));
    if (hit && hit->geometry.visible != 0) {
      this->previous = this->current;
      this->current = hit->Uid();
      if (this->previous != this->current) {
        hit->Slot47();

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

// INLINED
bool WindowMouseCapture::Dispatch() {
  if (!this->capture) {
    return false;
  }

  const int x = this->mouse.x;
  const int y = this->mouse.y;
  int left;
  int top;
  UIWindow* window = this->capture;
  WindowScreenPosition(window, left, top);
  const int localX = x - left;
  const int localY = y - top;

  if (this->mouse.buttons[0] == 1) {
    UIWindow* root = window->Root();
    if (root && root->Slot02()) {
      this->ordering.Promote(root);
    }

    this->capture->Slot25(localX, localY);
  }

  if (this->mouse.buttons[0] == 4 && (this->doubleClickGate == 0 || this->alternateDoubleClickGate == 0)) {
    this->capture->Slot25(localX, localY);
    this->capture->Slot26(localX, localY);
  }

  this->capture->ResetCursor(localX, localY);

  if (this->previousX == x && this->previousY == y) {
    this->capture->Slot28(localX, localY);
  } else {
    this->capture->Slot27(localX, localY);
  }

  if (this->mouse.wheel != 0) {
    this->capture->Slot35(this->mouse.wheel);
  }

  if (this->mouse.buttons[0] == 3) {
    this->capture->Slot31(localX, localY);
  }

  return true;
}

// INLINED
int WindowMouseUncaptured::Dispatch() {
  const int x = this->mouse.x;
  const int y = this->mouse.y;
  UIWindow* window = this->hit.HitTest(x, y);

  if (!this->modal.empty() && this->modal.back() && window && window != this->modal.back() && !window->IsDescendantOf(this->modal.back())) {
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
    const int localX = x - left;
    const int localY = y - top;

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
      WindowScreenPosition(this->hover, left, top);
      const int localX = x - left;
      const int localY = y - top;
      this->hover->Slot27(localX, localY);
      this->hover->ResetCursor(localX, localY);
    }

    this->hover = window;
  }

  return handled;
}
