#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTVISIBILITY_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTVISIBILITY_H

#include "Client/Replay.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowOrdering.h"

// INLINED
inline bool UIWindowMgr::ShowShortcutWindow(int value) {
  if (g_replayAnimationScaling && !Replay::Instance().header.options.unknown_0007) {
    return false;
  }

  this->unknown_0758 = value;
  g_windowMgr.EnsureWindow(36);
  this->window_01E8->Show(value);

  if (value != 0) {
    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this->window_01E8);
  }

  if (g_serviceOptions.Get(0x10E)) {
    g_windowMgr.EnsureWindow(159);
  }

  UIWindow* companion = g_windowMgr.LookupWindow(159);

  if (companion) {
    companion->Show(value);

    if (value != 0) {
      reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(companion);
    }
  }

  return true;
}

struct ShortcutVisibility {
  WindowManagerWindows& windows;
  WindowOrdering& ordering;
  UIWindow*& shortcut;
  int& visible;
  const bool& restricted;
  const bool& replayAllowsShortcut;
  const bool& showCompanion;

  bool Show(int value);
};

// INLINED
inline bool ShortcutVisibility::Show(int value) {
  if (this->restricted && !this->replayAllowsShortcut) {
    return false;
  }

  this->visible = value;
  this->windows.EnsureWindow(36);
  this->shortcut->Show(value);

  if (value != 0) {
    this->ordering.Promote(this->shortcut);
  }

  if (this->showCompanion) {
    this->windows.EnsureWindow(159);
  }

  UIWindow* companion = this->windows.LookupWindow(159);

  if (companion) {
    companion->Show(value);

    if (value != 0) {
      this->ordering.Promote(companion);
    }
  }

  return true;
}

#endif
