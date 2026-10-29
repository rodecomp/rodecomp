#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTVISIBILITY_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTVISIBILITY_H

#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"

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

struct WindowVisibility {
  WindowManagerWindows& windows;
  WindowOrdering& ordering;
  ShortcutVisibility& shortcut;
  int& basicInfoVisible;
  UIWindow*& basicInfo;
  UIWindow*& window15;
  int& window46Visible;
  UIWindow*& window46;
  int& window276Visible;
  UIWindow*& window276;
  const bool& restricted;
  const bool& replayAllowsBasicInfo;

  bool Show(int id, int visible);
};

#endif
