#include "UI/Window/WindowId.h"
#include "Client/Replay.h"
#include "UI/Window/WindowId.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindowMgr.inl"

#include <algorithm>

namespace {
  // INLINED
  __forceinline void PromoteWindow(UIWindow* window) {
    if ((g_windowMgr.modal != nullptr && g_windowMgr.window_4F174 != window) || g_windowMgr.LookupWindow(WID_NEWMAKECHARWND) || g_windowMgr.LookupWindow(WID_MAKECHARWND) ||
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

    this->unknown_003C = visible;

    if (!this->window_01C8) {
      g_windowMgr.EnsureWindow(WID_CHATWND);
    }

    this->window_01C8->Show(visible);
    UIWindow* window = this->window_01C8;

    if (visible == 0) {
      window->StoreInfo();
      return true;
    }

    PromoteWindow(window);
    return true;
  }

  case WID_ITEMDROPCNTWND:
    g_windowMgr.EnsureWindow(WID_ITEMDROPCNTWND);

    if (this->window_01EC->geometry.visible != visible) {
      this->window_01EC->Show(visible);
    }

    if (visible != 0) {
      PromoteWindow(this->window_01EC);
    }

    return false;

  case WID_SHORTCUTWND:
    if (g_replayAnimationScaling && !Replay::Instance().header.options.unknown_0007) {
      return false;
    }

    this->m_normalItemSlotWindow.show = visible;
    g_windowMgr.EnsureWindow(WID_SHORTCUTWND);
    this->window_01E8->Show(visible);

    if (visible != 0) {
      PromoteWindow(this->window_01E8);
    }

    if (g_serviceOptions.Get(0x10E)) {
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
    this->unknown_0048 = visible;

    if (!this->window_0230) {
      if (visible == 0) {
        return true;
      }

      g_windowMgr.EnsureWindow(WID_SKILLDESCRIBEWND);

      if (!this->window_0230) {
        return true;
      }
    }

    this->window_0230->Show(visible);
    UIWindow* window = this->window_0230;

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

    if (!this->window_4F1E4) {
      if (visible == 0) {
        return true;
      }

      g_windowMgr.EnsureWindow(WID_ROUNDBOXTOOLTIPWND);

      if (!this->window_4F1E4) {
        return true;
      }
    }

    this->window_4F1E4->Show(visible);
    return true;
  }

  return false;
}
