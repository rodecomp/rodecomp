#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIWINDOWMGR_INL
#define RAGNAROK_SOURCE_UI_WINDOW_UIWINDOWMGR_INL

#include "Client/Replay.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// INLINED
inline bool UIWindowMgr::ShowShortcutWindow(int value) {
  if (g_replayAnimationScaling && !Replay::Instance().header.options.unknown_0007) {
    return false;
  }

  this->m_normalItemSlotWindow.show = value;
  g_windowMgr.EnsureWindow(36);
  this->window_01E8->Show(value);

  if (value != 0) {
    g_windowMgr.Promote(this->window_01E8);
  }

  if (g_serviceOptions.Get(0x10E)) {
    g_windowMgr.EnsureWindow(159);
  }

  UIWindow* companion = g_windowMgr.LookupWindow(159);

  if (companion) {
    companion->Show(value);

    if (value != 0) {
      g_windowMgr.Promote(companion);
    }
  }

  return true;
}

#endif
