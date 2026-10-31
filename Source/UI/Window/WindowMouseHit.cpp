#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"
#include "Client/Replay.h"
#include "Contents/Messenger/MessengerMember.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Layout/WindowCoordinates.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowMouseHit.h"
#include "UI/Window/WindowOrdering.h"

// GLOBAL: gl_24656
int g_alternateDoubleClickGate;
// GLOBAL: gl_24657
int g_mouseRefreshGate;

// INLINED
static void PromoteWindow(UIWindowMgr& mgr, UIWindow* window) {
  if (mgr.modal != 0 && mgr.window_4F174 != window) {
    return;
  }

  if (mgr.LookupWindow(278) || mgr.LookupWindow(4) || mgr.LookupWindow(199) || mgr.LookupWindow(200)) {
    return;
  }

  int companion = window->geometry.id == 12 ? 234 : -1;
  if (window->geometry.id == 82) {
    companion = 250;
  }

  if (window->geometry.id == 82 || window->geometry.id == 12) {
    for (auto entry = mgr.rendered.begin(); entry != mgr.rendered.end(); ++entry) {
      UIWindow* value = *entry;
      if (value->geometry.id == companion) {
        mgr.rendered.erase(entry);
        mgr.rendered.push_back(value);
        break;
      }
    }
  }

  for (auto entry = mgr.rendered.begin(); entry != mgr.rendered.end(); ++entry) {
    if (*entry == window) {
      mgr.rendered.erase(entry);
      mgr.rendered.push_back(window);
      break;
    }
  }
}

// INLINED
static void CloseDropDown(UIWindowMgr& mgr) {
  CUIDropDownBox* popup = static_cast<CUIDropDownBox*>(mgr.window_0A30);
  if (popup && popup->geometry.visible != 0) {
    popup->Detach();
    popup->Show(0);
  }
}

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

// OFFSET: 0x00AA4730
// FUNCTION: fn_17222
int UIWindowMgr::ProcessMouse() {
  int handled = 0;
  const int x = g_mouse.x;
  const int y = g_mouse.y;

  if (this->modalWindows.empty() || !this->modalWindows.back()) {
    for (UIWindow* window : this->retired) {
      if (this->window_4F16C == window) {
        this->window_4F16C = nullptr;
      }
      if (this->inputWindow && (this->inputWindow == window || this->inputWindow->IsDescendantOf(window))) {
        this->inputWindow = nullptr;
      }
      if (this->hoverWindow && (this->hoverWindow == window || this->hoverWindow->IsDescendantOf(window))) {
        this->hoverWindow = nullptr;
      }
      if (this->window_01B8 && (this->window_01B8 == window || this->window_01B8->IsDescendantOf(window))) {
        this->window_01B8 = nullptr;
      }
      this->Destroy(window);
    }
    this->retired.clear();
  }

  for (UIWindow* window : this->deferredDeletion) {
    if (window) {
      delete window;
    }
  }
  this->deferredDeletion.clear();

  for (int accountId : this->unknown_4F248) {
    for (auto entry = this->snapExcluded.begin(); entry != this->snapExcluded.end();) {
      UIWindow* window = *entry;
      if (static_cast<int>(static_cast<UIMiniPartyWnd*>(window)->GetMember().accountId) == accountId) {
        this->Destroy(window);
        entry = this->snapExcluded.erase(entry);
      } else {
        ++entry;
      }
    }
  }
  this->unknown_4F248.clear();

  if (g_mouse.buttons[0] == 1 && this->window_01C8) {
    ChatSuggestions* suggestions = reinterpret_cast<ChatSuggestions*>(this->window_01C8);
    if (suggestions->Visible() == 1) {
      int left;
      int top;
      int right;
      int bottom;
      WindowScreenBounds(suggestions->unknown_013C, left, top, right, bottom);
      const bool outside = left > x || right < x || top > y || bottom < y;
      WindowScreenBounds(this->unknown_0384, left, top, right, bottom);
      const bool outsideCompanion = left > x || right < x || top > y || bottom < y;
      if (outside && outsideCompanion) {
        suggestions->Show(0);
        if (suggestions->unknown_00BC) {
          static_cast<UIEditCtrl*>(suggestions->unknown_00BC)->SetText("");
        }
        reinterpret_cast<ChatInputFocus*>(suggestions)->Toggle();
      }
    }
  }

  if (this->inputWindow) {
    handled = 1;
    int left;
    int top;
    WindowScreenPosition(this->inputWindow, left, top);
    const int localX = x - left;
    const int localY = y - top;

    if (g_mouse.buttons[0] == 1) {
      UIWindow* root = this->inputWindow->Root();
      if (root && root->Slot02()) {
        PromoteWindow(*this, root);
      }

      this->inputWindow->Slot25(localX, localY);
    }

    if (g_mouse.buttons[0] == 4 && (g_replayAnimationScaling == 0 || g_alternateDoubleClickGate == 0)) {
      this->inputWindow->Slot25(localX, localY);
      this->inputWindow->Slot26(localX, localY);
    }

    this->inputWindow->ResetCursor(localX, localY);
    if (this->unknown_046C == x && this->unknown_0470 == y) {
      this->inputWindow->Slot28(localX, localY);
    } else {
      this->inputWindow->Slot27(localX, localY);
    }

    if (g_mouse.wheel != 0) {
      this->inputWindow->Slot35(g_mouse.wheel);
    }

    if (g_mouse.buttons[0] == 3) {
      this->inputWindow->Slot31(localX, localY);
    }
  } else {
    UIWindow* window = reinterpret_cast<WindowMouseHit*>(this)->HitTest(x, y);

    if (!this->modalWindows.empty() && this->modalWindows.back() && window) {
      UIWindow* modalWindow = this->modalWindows.back();
      if (modalWindow != window && !window->IsDescendantOf(modalWindow)) {
        window = nullptr;
      }
    }

    if (g_mouse.buttons[0] == 1) {
      SetPressedWindowRoot(this->window_4F1D4, window);
    }

    if (window) {
      handled = window->Slot08() ? 2 : 1;
      int left;
      int top;
      WindowScreenPosition(window, left, top);
      const int localX = x - left;
      const int localY = y - top;

      if (g_mouse.buttons[0] == 4 && (g_replayAnimationScaling == 0 || g_alternateDoubleClickGate == 0)) {
        window->Slot26(localX, localY);
      }

      if (g_mouse.buttons[1] == 4) {
        window->Slot34(localX, localY);
      }

      if (g_mouse.buttons[0] == 1) {
        UIWindow* root = window->Root();
        if (root && root->Slot02()) {
          PromoteWindow(*this, root);
        }

        CUIDropDownBox* popup = static_cast<CUIDropDownBox*>(this->window_0A30);
        if (popup && window != popup && root != popup && !popup->HasOwnerChild(window)) {
          CloseDropDown(*this);
        }

        window->Slot25(localX, localY);
      }

      if (g_mouse.buttons[1] == 1) {
        UIWindow* root = window->Root();
        if (root && root->Slot02()) {
          PromoteWindow(*this, root);
        }

        window->Slot32(localX, localY);
      }

      window->ResetCursor(localX, localY);
      if (this->unknown_046C == x && this->unknown_0470 == y) {
        window->Slot28(localX, localY);
      } else {
        window->Slot27(localX, localY);
      }

      if (g_mouse.wheel != 0) {
        window->Slot35(g_mouse.wheel);
      }

      if (g_mouse.buttons[1] == 3) {
        window->Slot33(localX, localY);
      }

      if (g_mouse.buttons[0] == 3) {
        window->Slot31(localX, localY);
      }
    }

    if (g_mouse.buttons[0] == 1) {
      if (this->unknown_0214 && (!window || (window != this->unknown_0214 && !window->IsDescendantOf(this->unknown_0214)))) {
        this->CloseWindow(26);
      }

      if (this->unknown_021C) {
        if (!window) {
          this->CloseWindow(260);
          CloseDropDown(*this);
        } else if (window != this->unknown_021C && !window->IsDescendantOf(this->unknown_021C)) {
          this->CloseWindow(260);
        }
      } else if (!window) {
        CloseDropDown(*this);
      }
    }

    if (this->window_03D4) {
      this->window_03D4->Invalidate();
    }
    if (this->unknown_01E0) {
      this->unknown_01E0->Invalidate();
    }
    if (this->unknown_01E4) {
      this->unknown_01E4->Invalidate();
    }
    if (this->unknown_03FC) {
      this->unknown_03FC->Invalidate();
    }
    if (this->window_03D8) {
      this->window_03D8->Invalidate();
    }
    if (this->window_02C4 && g_mouseRefreshGate == 0) {
      this->window_02C4->Invalidate();
    }
    if (this->window_4F170) {
      this->window_4F170->Invalidate();
    }

    if (window) {
      if (this->window_01B8 && window != this->window_01B8) {
        int left = 0;
        int top = 0;
        this->window_01B8->ToScreen(left, top);
        this->window_01B8->Slot27(x - left, y - top);
        this->window_01B8->ResetCursor(x - left, y - top);
      }

      this->window_01B8 = window;
    }
  }

  if (this->hoverWindow) {
    this->hoverWindow->Invalidate();
    UIWindow* root = this->hoverWindow->Root();
    if (root && root != this->unknown_0328) {
      root->Invalidate();
    }
  }

  this->unknown_046C = x;
  this->unknown_0470 = y;

  if (!this->modalWindows.empty() && this->modalWindows.back()) {
    handled = 1;
  }

  for (auto entry = this->browsers.begin(); entry != this->browsers.end(); ++entry) {
    if (entry->second && entry->second->cursorState != 0) {
      handled = 1;
      break;
    }
  }

  for (auto entry = this->rendered.rbegin(); entry != this->rendered.rend(); ++entry) {
    (*entry)->Slot29(x, y);
  }

  return handled;
}
