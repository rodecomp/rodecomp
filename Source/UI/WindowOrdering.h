#ifndef RAGNAROK_SOURCE_UI_WINDOWORDERING_H
#define RAGNAROK_SOURCE_UI_WINDOWORDERING_H

#include <list>

class UIWindow;
class WindowManagerWindows;

// Original rendered-window list and modal policy remain owned by UIWindowMgr.
struct WindowOrdering {
  std::list<UIWindow*>& rendered;
  WindowManagerWindows& windows;
  const int& modal;
  UIWindow* const& modalWindow;

  void Promote(UIWindow* window);
  // Borrowed manager+0x4FC latch. Set before virtual callbacks to suppress
  // reentrant passes; only windows whose Slot03 returns true are invalidated.
  void InvalidateFrames(int& latch);
};

#endif
