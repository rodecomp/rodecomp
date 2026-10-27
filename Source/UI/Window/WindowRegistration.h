#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWREGISTRATION_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWREGISTRATION_H

#include <list>

struct WindowAncestry;

struct WindowRegistration {
  /* 018C */ std::list<WindowAncestry*> windows;

  void Add(WindowAncestry* window);
  void Remove(WindowAncestry* window);

  void InvalidateAll();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WindowRegistration) == 8);
#endif

#endif
