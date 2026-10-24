#ifndef RAGNAROK_SOURCE_UI_SHORTCUTVISIBILITY_H
#define RAGNAROK_SOURCE_UI_SHORTCUTVISIBILITY_H

class UIWindow;
class WindowManagerWindows;
struct WindowOrdering;

// Original UIWindowMgr shortcut pointer, visibility value and replay policy.
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
