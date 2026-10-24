#ifndef RAGNAROK_SOURCE_UI_FRAMEFOCUS_H
#define RAGNAROK_SOURCE_UI_FRAMEFOCUS_H

#include "UI/WindowAncestry.h"

#include <list>

class UIWindow;
class WindowManagerWindows;

// Borrowed actual manager order list+24 and reference+412; no extra owner.
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

// STATIC: 0x014576A0
// GLOBAL: gl_24367
extern int g_dragWindowId;

#endif
