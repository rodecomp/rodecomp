#include "Client/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"
#include "Client/Replay.h"
#include "Contents/Messenger/MessengerMember.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Layout/WindowCoordinates.h"
#include "UI/Window/WindowManagerWindows.h"

// OFFSET: 0x00A91B30
// FUNCTION: fn_17168
__declspec(noinline) UIWindow* UIWindowMgr::FindRenderedUid(std::uint32_t uid) {
  for (UIWindow* window : this->rendered) {
    if (UIWindow* found = window->FindUid(uid)) {
      return found;
    }
  }

  return nullptr;
}

// OFFSET: 0x00A91C70
// FUNCTION: fn_17171
UIWindow* UIWindowMgr::HitTestWindows(int x, int y) {
  for (auto entry = this->rendered.rbegin(); entry != this->rendered.rend(); ++entry) {
    UIWindow* hit = (*entry)->HitTest(x, y);

    if (hit && hit->geometry.visible != 0) {
      this->previousHitUid = this->currentHitUid;
      this->currentHitUid = hit->Uid();
      if (this->previousHitUid != this->currentHitUid) {
        hit->Slot47();

        if (UIWindow* old = this->FindRenderedUid(this->previousHitUid)) {
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

  this->previousHitUid = this->currentHitUid;
  this->currentHitUid = 0;
  if (this->previousHitUid != 0) {
    if (UIWindow* old = this->FindRenderedUid(this->previousHitUid)) {
      old->Slot48();
    }
  }

  return nullptr;
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
    UINewChatWnd* suggestions = static_cast<UINewChatWnd*>(this->window_01C8);
    if (suggestions->SuggestionsVisible() == 1) {
      int left;
      int top;
      int right;
      int bottom;
      WindowScreenBounds(suggestions->m_suggestions, left, top, right, bottom);
      const bool outside = left > x || right < x || top > y || bottom < y;
      WindowScreenBounds(this->unknown_0384, left, top, right, bottom);
      const bool outsideCompanion = left > x || right < x || top > y || bottom < y;
      if (outside && outsideCompanion) {
        suggestions->ShowSuggestions(0);
        if (suggestions->m_chatEdit) {
          static_cast<UIEditCtrl*>(suggestions->m_chatEdit)->SetText("");
        }
        suggestions->ToggleInput();
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
        this->Promote(root);
      }

      this->inputWindow->Slot25(localX, localY);
    }

    if (g_mouse.buttons[0] == 4 && (g_replayAnimationScaling == 0 || g_modeMgr.state == 0)) {
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
    UIWindow* window = this->HitTestWindows(x, y);

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

      if (g_mouse.buttons[0] == 4 && (g_replayAnimationScaling == 0 || g_modeMgr.state == 0)) {
        window->Slot26(localX, localY);
      }

      if (g_mouse.buttons[1] == 4) {
        window->Slot34(localX, localY);
      }

      if (g_mouse.buttons[0] == 1) {
        UIWindow* root = window->Root();
        if (root && root->Slot02()) {
          this->Promote(root);
        }

        CUIDropDownBox* popup = static_cast<CUIDropDownBox*>(this->window_0A30);
        if (popup && window != popup && root != popup && !popup->HasOwnerChild(window)) {
          this->CloseDropDown();
        }

        window->Slot25(localX, localY);
      }

      if (g_mouse.buttons[1] == 1) {
        UIWindow* root = window->Root();
        if (root && root->Slot02()) {
          this->Promote(root);
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
          this->CloseDropDown();
        } else if (window != this->unknown_021C && !window->IsDescendantOf(this->unknown_021C)) {
          this->CloseWindow(260);
        }
      } else if (!window) {
        this->CloseDropDown();
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
    if (this->window_02C4 && g_session.m_simplifiedSkillList == 0) {
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
