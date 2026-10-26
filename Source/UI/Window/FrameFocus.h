#ifndef RAGNAROK_SOURCE_UI_WINDOW_FRAMEFOCUS_H
#define RAGNAROK_SOURCE_UI_WINDOW_FRAMEFOCUS_H

#include "UI/Window/WindowAncestry.h"

#include <list>

class UIWindow;
class WindowManagerWindows;

struct FrameFocus {
  std::list<int>& order;
  WindowAncestry*& focused;
  WindowManagerWindows& windows;

  bool IsOrderFixed(int id);
  void RemoveOrder(int id);
  void BringToFront(int id);
  void Set(UIWindow* window);
  void Clear();
  UIWindow* Get() const;
};

extern int g_dragWindowId;

#endif
