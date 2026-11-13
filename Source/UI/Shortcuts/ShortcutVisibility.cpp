#include "UI/Window/WindowId.h"
#include "UI/GameSettings/TalkType.h"
#include "Client/Replay/Replay.h"
#include "UI/Window/WindowId.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindowMgr.inl"

#include <algorithm>

namespace {
  // INLINED
  __forceinline void PromoteWindow(UIWindow* window) {
    if ((g_windowMgr.modal != nullptr && g_windowMgr.navigationWnd != window) || g_windowMgr.LookupWindow(WID_NEWMAKECHARWND) || g_windowMgr.LookupWindow(WID_MAKECHARWND) ||
        g_windowMgr.LookupWindow(WID_SECONDPASSWORDINPUTWND) || g_windowMgr.LookupWindow(WID_SECONDPASSWORDEDITWND)) {
      return;
    }

    int companion = -1;
    if (window->geometry.id == 12) {
      companion = 234;
    } else if (window->geometry.id == 82) {
      companion = 250;
    }

    if (companion != -1) {
      const auto found = std::find_if(g_windowMgr.rendered.begin(), g_windowMgr.rendered.end(), [&](const UIWindow* value) {
        return value->geometry.id == companion;
      });
      if (found != g_windowMgr.rendered.end()) {
        UIWindow* value = *found;
        g_windowMgr.rendered.erase(found);
        g_windowMgr.rendered.push_back(value);
      }
    }

    const auto found = std::find(g_windowMgr.rendered.begin(), g_windowMgr.rendered.end(), window);
    if (found != g_windowMgr.rendered.end()) {
      g_windowMgr.rendered.erase(found);
      g_windowMgr.rendered.push_back(window);
    }
  }
}

// OFFSET: 0x00AAA310
// FUNCTION: fn_17269
bool UIWindowMgr::ShowWindowById(int id, int visible) {
  switch (id) {
  case WID_CHATWND: {
    if (g_replayAnimationScaling && !Replay::Instance().header.options.recordRestricted) {
      return false;
    }

    this->chatWndVisible = visible;

    if (!this->chatWnd) {
      g_windowMgr.EnsureWindow(WID_CHATWND);
    }

    this->chatWnd->Show(visible);
    UIWindow* window = this->chatWnd;

    if (visible == 0) {
      window->StoreInfo();
      return true;
    }

    PromoteWindow(window);
    return true;
  }

  case WID_ITEMDROPCNTWND:
    g_windowMgr.EnsureWindow(WID_ITEMDROPCNTWND);

    if (this->itemDropCntWnd->geometry.visible != visible) {
      this->itemDropCntWnd->Show(visible);
    }

    if (visible != 0) {
      PromoteWindow(this->itemDropCntWnd);
    }

    return false;

  case WID_SHORTCUTWND:
    if (g_replayAnimationScaling && !Replay::Instance().header.options.showItemSlots) {
      return false;
    }

    this->m_normalItemSlotWindow.show = visible;
    g_windowMgr.EnsureWindow(WID_SHORTCUTWND);
    this->shortCutWnd->Show(visible);

    if (visible != 0) {
      PromoteWindow(this->shortCutWnd);
    }

    if (g_serviceOptions.Get(TT_QUICK_SLOT_ON_OFF)) {
      g_windowMgr.EnsureWindow(WID_QUIKSLOTWND);
    }

    if (UIWindow* companion = g_windowMgr.LookupWindow(WID_QUIKSLOTWND)) {
      companion->Show(visible);

      if (visible != 0) {
        PromoteWindow(companion);
      }
    }

    return true;

  case WID_SKILLDESCRIBEWND: {
    this->skillDescriptionVisible = visible;

    if (!this->skillDescribeWnd) {
      if (visible == 0) {
        return true;
      }

      g_windowMgr.EnsureWindow(WID_SKILLDESCRIBEWND);

      if (!this->skillDescribeWnd) {
        return true;
      }
    }

    this->skillDescribeWnd->Show(visible);
    UIWindow* window = this->skillDescribeWnd;

    if (visible == 0) {
      window->StoreInfo();
      return true;
    }

    PromoteWindow(window);
    return true;
  }

  case WID_MINIMAPWND:
    g_windowMgr.LookupWindow(WID_MINIMAPWND)->Show(visible);
    return true;

  case WID_ROUNDBOXTOOLTIPWND:
    this->visible_276 = visible;

    if (!this->roundBoxTooltipWnd) {
      if (visible == 0) {
        return true;
      }

      g_windowMgr.EnsureWindow(WID_ROUNDBOXTOOLTIPWND);

      if (!this->roundBoxTooltipWnd) {
        return true;
      }
    }

    this->roundBoxTooltipWnd->Show(visible);
    return true;
  }

  return false;
}
