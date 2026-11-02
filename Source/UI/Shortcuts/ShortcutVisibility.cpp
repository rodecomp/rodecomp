#include "Client/Replay.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowOrdering.h"

// OFFSET: 0x00AAA310
// FUNCTION: fn_17269
bool UIWindowMgr::ShowWindowById(int id, int visible) {
  switch (id) {
  case 1: {
    if (g_replayAnimationScaling && !Replay::Instance().header.options.recordRestricted) {
      return false;
    }

    this->unknown_003C = visible;

    if (!this->window_01C8) {
      g_windowMgr.EnsureWindow(1);
    }

    this->window_01C8->Show(visible);
    UIWindow* window = this->window_01C8;

    if (visible == 0) {
      window->Slot11();
      return true;
    }

    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(window);
    return true;
  }

  case 15:
    g_windowMgr.EnsureWindow(15);

    if (this->window_01EC->geometry.visible != visible) {
      this->window_01EC->Show(visible);
    }

    if (visible != 0) {
      reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this->window_01EC);
    }

    return false;

  case 36:
    return this->ShowShortcutWindow(visible);

  case 46: {
    this->unknown_0048 = visible;

    if (!this->window_0230) {
      if (visible == 0) {
        return true;
      }

      g_windowMgr.EnsureWindow(46);

      if (!this->window_0230) {
        return true;
      }
    }

    this->window_0230->Show(visible);
    UIWindow* window = this->window_0230;

    if (visible == 0) {
      window->Slot11();
      return true;
    }

    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(window);
    return true;
  }

  case 273:
    g_windowMgr.LookupWindow(273)->Show(visible);
    return true;

  case 276:
    this->visible_276 = visible;

    if (!this->window_4F1E4) {
      if (visible == 0) {
        return true;
      }

      g_windowMgr.EnsureWindow(276);

      if (!this->window_4F1E4) {
        return true;
      }
    }

    this->window_4F1E4->Show(visible);
    return true;
  }

  return false;
}
