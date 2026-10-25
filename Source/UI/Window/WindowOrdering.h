#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWORDERING_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWORDERING_H

#include <list>

class UIWindow;
class WindowManagerWindows;

struct WindowOrdering {
  std::list<UIWindow*>& rendered;
  WindowManagerWindows& windows;
  const int& modal;
  UIWindow* const& modalWindow;

  void Promote(UIWindow* window);

  void InvalidateFrames(int& latch);
};

#endif
