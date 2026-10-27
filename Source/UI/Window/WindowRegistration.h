#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWREGISTRATION_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWREGISTRATION_H

#include <list>

class UIWindow;

struct WindowRegistration {
  /* 018C */ std::list<UIWindow*> windows;

  void Add(UIWindow* window);
  void Remove(UIWindow* window);

  void InvalidateAll();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WindowRegistration) == 8);
#endif

#endif
