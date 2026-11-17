#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIWINDOWMGR_INL
#define RAGNAROK_SOURCE_UI_WINDOW_UIWINDOWMGR_INL

#include "Client/Replay/Replay.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Shortcuts/UIShortCutWnd.h"

// INLINED
inline bool UIWindowMgr::ShowShortcutWindow(int value) {
  if (g_replayAnimationScaling && !Replay::Instance().header.options.showItemSlots) {
    return false;
  }

  this->m_normalItemSlotWindow.show = value;
  g_windowMgr.EnsureWindow(WID_SHORTCUTWND);
  this->shortCutWnd->Show(value);

  if (value != 0) {
    g_windowMgr.Promote(this->shortCutWnd);
  }

  if (g_serviceOptions.Get(TT_QUICK_SLOT_ON_OFF)) {
    g_windowMgr.EnsureWindow(WID_QUIKSLOTWND);
  }

  UIWindow* companion = g_windowMgr.LookupWindow(WID_QUIKSLOTWND);

  if (companion) {
    companion->Show(value);

    if (value != 0) {
      g_windowMgr.Promote(companion);
    }
  }

  return true;
}

#endif
