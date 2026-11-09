#include "UI/Window/UIRoulletteWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

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
void UIWindowMgr::Promote(UIWindow* window) {
  if (!window || (this->modal != nullptr && this->navigationWnd != window)) {
    return;
  }

  if (this->LookupWindow(WID_NEWMAKECHARWND) || this->LookupWindow(WID_MAKECHARWND) || this->LookupWindow(WID_SECONDPASSWORDINPUTWND) || this->LookupWindow(WID_SECONDPASSWORDEDITWND)) {
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
