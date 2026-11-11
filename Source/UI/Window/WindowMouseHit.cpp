#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"
#include "Client/Replay/Replay.h"
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

#include <algorithm>

// INLINED
static MessengerMember PartyMember(UIMiniPartyWnd* window) {
  return window->m_member;
}

// INLINED
static __forceinline void PromoteWindow(UIWindowMgr* manager, UIWindow* window) {
  if (!window || (manager->modal != nullptr && manager->navigationWnd != window)) {
    return;
  }

  if (manager->LookupWindow(WID_NEWMAKECHARWND) || manager->LookupWindow(WID_MAKECHARWND) || manager->LookupWindow(WID_SECONDPASSWORDINPUTWND) || manager->LookupWindow(WID_SECONDPASSWORDEDITWND)) {
    return;
  }

  int companion = -1;
  if (window->geometry.id == 12) {
    companion = 234;
  } else if (window->geometry.id == 82) {
    companion = 250;
  }

  if (companion != -1) {
    const auto entry = std::find_if(manager->rendered.begin(), manager->rendered.end(), [companion](const UIWindow* value) {
      return value->geometry.id == companion;
    });
    if (entry != manager->rendered.end()) {
      UIWindow* value = *entry;
      manager->rendered.erase(entry);
      manager->rendered.push_back(value);
    }
  }

  const auto entry = std::find(manager->rendered.begin(), manager->rendered.end(), window);
  if (entry != manager->rendered.end()) {
    manager->rendered.erase(entry);
    manager->rendered.push_back(window);
  }
}

// OFFSET: 0x00A91B30
// FUNCTION: fn_17168
__declspec(noinline) UIWindow* UIWindowMgr::FindRenderedUid(std::uint32_t uid) {
  for (UIWindow* window : this->rendered) {
    if (uid == window->m_uid) {
      return window;
    }
    if (UIWindow* found = window->FindUid(uid)) {
      return found;
    }
  }

  return nullptr;
}

// OFFSET: 0x00A91B70
// FUNCTION: fn_45428
void __stdcall WindowScreenPosition(const UIWindow* window, int& x, int& y) {
  int horizontal = 0;
  int vertical = 0;

  for (const UIWindow* node = window; node; node = node->parent) {
    horizontal = (horizontal + node->geometry.x);
    vertical = (vertical + node->geometry.y);
  }

  x = horizontal;
  y = vertical;
}

// OFFSET: 0x00A91BA0
// FUNCTION: fn_45429
void __stdcall WindowScreenBounds(const UIWindow* window, int& left, int& top, int& right, int& bottom) {
  bottom = 0;
  right = 0;
  top = 0;
  left = 0;

  int horizontal = 0;
  int vertical = 0;

  for (const UIWindow* node = window; node; node = node->parent) {
    horizontal = (horizontal + node->geometry.x);
    vertical = (vertical + node->geometry.y);
  }

  left = horizontal;
  top = vertical;
  if (window) {
    right = (window->geometry.width + left);
    bottom = (window->geometry.height + top);
  }
}

// OFFSET: 0x00A91C70
// FUNCTION: fn_17171
__declspec(noinline) UIWindow* UIWindowMgr::HitTestWindows(int x, int y) {
  for (auto entry = this->rendered.rbegin(); entry != this->rendered.rend(); ++entry) {
    UIWindow* hit = (*entry)->HitTest(x, y);

    if (hit && hit->geometry.visible != 0) {
      this->previousHitUid = this->currentHitUid;
      this->currentHitUid = hit->Uid();
      if (this->previousHitUid != this->currentHitUid) {
        hit->OnMouseEnter();

        if (UIWindow* old = this->FindRenderedUid(this->previousHitUid)) {
          old->OnMouseLeave();
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
      old->OnMouseLeave();
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
    for (auto entry = this->retired.begin(); entry != this->retired.end(); ++entry) {
      UIWindow* window = *entry;
      if (this->replayMessageBoxWnd == window) {
        this->replayMessageBoxWnd = nullptr;
      }
      if (this->inputWindow && (this->inputWindow == window || this->inputWindow->IsDescendantOf(window))) {
        this->inputWindow = nullptr;
      }
      if (this->hoverWindow && (this->hoverWindow == window || this->hoverWindow->IsDescendantOf(window))) {
        this->hoverWindow = nullptr;
      }
      if (this->previousMouseWnd && (this->previousMouseWnd == window || this->previousMouseWnd->IsDescendantOf(window))) {
        this->previousMouseWnd = nullptr;
      }
      this->Destroy(window);
    }
    this->retired.erase(this->retired.begin(), this->retired.end());
  }

  for (UIWindow* window : this->deferredDeletion) {
    if (window) {
      delete window;
    }
  }
  this->deferredDeletion.clear();

  for (auto account = this->closedMiniPartyIds.begin(); account != this->closedMiniPartyIds.end(); ++account) {
    const int accountId = *account;
    for (auto entry = this->snapExcluded.begin(); entry != this->snapExcluded.end();) {
      UIWindow* window = *entry;
      if (static_cast<int>(PartyMember(static_cast<UIMiniPartyWnd*>(window)).accountId) == accountId) {
        this->Destroy(window);
        entry = this->snapExcluded.erase(entry);
      } else {
        ++entry;
      }
    }
  }
  this->closedMiniPartyIds.clear();

  if (g_mouse.buttons[0] == 1 && this->chatWnd) {
    UINewChatWnd* suggestions = static_cast<UINewChatWnd*>(this->chatWnd);
    if (suggestions->SuggestionsVisible() == 1) {
      int left;
      int top;
      int right;
      int bottom;
      WindowScreenBounds(suggestions->m_suggestions, left, top, right, bottom);
      const bool outside = left > x || right < x || top > y || bottom < y;
      WindowScreenBounds(this->cashEmotionListWnd, left, top, right, bottom);
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
      if (root && root->IsFrameWnd()) {
        PromoteWindow(this, root);
      }

      this->inputWindow->OnLBtnDown(localX, localY);
    }

    if (g_mouse.buttons[0] == 4 && (g_replayAnimationScaling == 0 || g_modeMgr.state == 0)) {
      this->inputWindow->OnLBtnDown(localX, localY);
      this->inputWindow->OnLBtnDblClk(localX, localY);
    }

    this->inputWindow->ResetCursor(localX, localY);
    if (this->previousMouseX == x && this->previousMouseY == y) {
      this->inputWindow->OnMouseHover(localX, localY);
    } else {
      this->inputWindow->OnMouseMove(localX, localY);
    }

    if (g_mouse.wheel != 0) {
      this->inputWindow->OnWheel(g_mouse.wheel);
    }

    if (g_mouse.buttons[0] == 3) {
      this->inputWindow->OnLBtnUp(localX, localY);
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
      ::SetPressedWindowRoot(this->keyboardCommandRootWnd, window);
    }

    if (window) {
      handled = window->IsTransmitMouseInput() ? 2 : 1;
      int left;
      int top;
      WindowScreenPosition(window, left, top);
      const int localX = x - left;
      const int localY = y - top;

      if (g_mouse.buttons[0] == 4 && (g_replayAnimationScaling == 0 || g_modeMgr.state == 0)) {
        window->OnLBtnDblClk(localX, localY);
      }

      if (g_mouse.buttons[1] == 4) {
        window->OnRBtnDblClk(localX, localY);
      }

      if (g_mouse.buttons[0] == 1) {
        UIWindow* root = window->Root();
        if (root && root->IsFrameWnd()) {
          PromoteWindow(this, root);
        }

        CUIDropDownBox* popup = static_cast<CUIDropDownBox*>(this->dropDownBoxWnd);
        if (popup && window != popup && root != popup && !popup->HasOwnerChild(window)) {
          this->CloseDropDown();
        }

        window->OnLBtnDown(localX, localY);
      }

      if (g_mouse.buttons[1] == 1) {
        UIWindow* root = window->Root();
        if (root && root->IsFrameWnd()) {
          PromoteWindow(this, root);
        }

        window->OnRBtnDown(localX, localY);
      }

      window->ResetCursor(localX, localY);
      if (this->previousMouseX == x && this->previousMouseY == y) {
        window->OnMouseHover(localX, localY);
      } else {
        window->OnMouseMove(localX, localY);
      }

      if (g_mouse.wheel != 0) {
        window->OnWheel(g_mouse.wheel);
      }

      if (g_mouse.buttons[1] == 3) {
        window->OnRBtnUp(localX, localY);
      }

      if (g_mouse.buttons[0] == 3) {
        window->OnLBtnUp(localX, localY);
      }
    }

    if (g_mouse.buttons[0] == 1) {
      if (this->comboBoxWnd && (!window || (window != this->comboBoxWnd && !window->IsDescendantOf(this->comboBoxWnd)))) {
        this->CloseWindow(WID_COMBOBOXWND);
      }

      if (this->comboBoxIDWnd) {
        if (!window) {
          this->CloseWindow(WID_COMBOBOXIDWND);
          this->CloseDropDown();
        } else if (window != this->comboBoxIDWnd && !window->IsDescendantOf(this->comboBoxIDWnd)) {
          this->CloseWindow(WID_COMBOBOXIDWND);
        }
      } else if (!window) {
        this->CloseDropDown();
      }
    }

    if (this->newSelectCharWnd) {
      this->newSelectCharWnd->Invalidate();
    }
    if (this->equipWnd) {
      this->equipWnd->Invalidate();
    }
    if (this->nCashWnd) {
      this->nCashWnd->Invalidate();
    }
    if (this->otherEquipWnd) {
      this->otherEquipWnd->Invalidate();
    }
    if (this->selCharForUServerWnd) {
      this->selCharForUServerWnd->Invalidate();
    }
    if (this->skillListWnd && g_session.m_simplifiedSkillList == 0) {
      this->skillListWnd->Invalidate();
    }
    if (this->secondCostumeWnd) {
      this->secondCostumeWnd->Invalidate();
    }

    if (window) {
      if (this->previousMouseWnd && window != this->previousMouseWnd) {
        int left = 0;
        int top = 0;
        this->previousMouseWnd->ToScreen(left, top);
        this->previousMouseWnd->OnMouseMove(x - left, y - top);
        this->previousMouseWnd->ResetCursor(x - left, y - top);
      }

      this->previousMouseWnd = window;
    }
  }

  if (this->hoverWindow) {
    this->hoverWindow->Invalidate();
    UIWindow* root = this->hoverWindow->Root();
    if (root && root != this->guildPositionManageWnd) {
      root->Invalidate();
    }
  }

  this->previousMouseX = x;
  this->previousMouseY = y;

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
    (*entry)->OnMouseCheck(x, y);
  }

  return handled;
}
