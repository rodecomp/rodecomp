#include "UI/Events/UIRoulletteWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Clan/UIClanWnd.h"
#include "UI/Guild/UIGuildWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"

#include <algorithm>

// OFFSET: 0x00A966E0
// FUNCTION: fn_17184
void UIWindowMgr::InvalidateAll() {
  for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    (*it)->InvalidateChildren();
  }
}

// OFFSET: 0x00A96710
// FUNCTION: fn_17185
void UIWindowMgr::RefreshClanWnd() {
  if (this->clanInfoManageWnd) {
    this->clanInfoManageWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A96730
// FUNCTION: fn_17186
void UIWindowMgr::RefreshGuildWindows() {
  if (this->guildInfoManageWnd) {
    this->guildInfoManageWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  if (this->guildMemberManageWnd) {
    this->guildMemberManageWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  if (this->guildPositionManageWnd) {
    this->guildPositionManageWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  if (this->guildSkillWnd) {
    this->guildSkillWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  if (this->guildBanishedMemberWnd) {
    this->guildBanishedMemberWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  if (this->guildNoticeWnd) {
    this->guildNoticeWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  if (this->guildTotalInfoWnd) {
    this->guildTotalInfoWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A96810
// FUNCTION: fn_17187
void UIWindowMgr::RefreshRouletteWnd() {
  if (this->roulletteWnd) {
    this->roulletteWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A96830
// FUNCTION: fn_17188
void UIWindowMgr::EnableRouletteWnd() {
  if (this->roulletteWnd) {
    this->roulletteWnd->m_enabled = 1;
  }
}

// OFFSET: 0x00A96850
// FUNCTION: fn_17189
void UIWindowMgr::UpdateSnapRectangles() {
  this->snapRectangles.clear();

  if (ServiceOptions::Get(141) == 0) {
    return;
  }

  for (auto windowIt = this->rendered.begin(); windowIt != this->rendered.end(); ++windowIt) {
    if ((*windowIt)->geometry.visible == 0) {
      continue;
    }

    UIWindow* window = *windowIt;

    if (std::find(this->snapExcluded.begin(), this->snapExcluded.end(), window) != this->snapExcluded.end()) {
      continue;
    }

    if (ServiceOptions::Get(141) != 0 && window) {
      this->snapRectangles[window].Set(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height);
    }
  }
}

// INLINED
static void RaiseEntry(std::list<UIWindow*>& windows, std::list<UIWindow*>::iterator entry) {
  UIWindow* window = *entry;
  windows.erase(entry);
  windows.push_back(window);
}

// OFFSET: 0x00A976C0
// FUNCTION: fn_17206
void UIWindowMgr::Promote(UIWindow* window) {
  if (!window || (this->modal != nullptr && this->navigationWnd != window)) {
    return;
  }

  if (this->LookupWindow(WID_NEWMAKECHARWND) || this->LookupWindow(WID_MAKECHARWND) || this->LookupWindow(WID_SECONDPASSWORDINPUTWND) || this->LookupWindow(WID_SECONDPASSWORDEDITWND)) {
    return;
  }

  int companion = -1;

  if (window->geometry.id == WID_ITEMCOLLECTIONWND) {
    companion = 234;
  }
  if (window->geometry.id == WID_COMBINEDCARDITEMCOLLECTIONWND) {
    companion = 250;
  }

  if (companion != -1) {
    for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
      if ((*it)->geometry.id == companion) {
        RaiseEntry(this->rendered, it);
        break;
      }
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
void UIWindowMgr::InvalidateFrames() {
  if (this->framesInvalidated != 0) {
    return;
  }

  this->framesInvalidated = 1;

  for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    if ((*it)->NeedsFrameRefresh()) {
      (*it)->InvalidateChildren();
    }
  }
}
