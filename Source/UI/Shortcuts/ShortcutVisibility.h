#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTVISIBILITY_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTVISIBILITY_H

class UIWindow;
class WindowManagerWindows;
struct WindowOrdering;

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

#endif
