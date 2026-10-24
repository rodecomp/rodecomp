#ifndef RAGNAROK_SOURCE_UI_WINDOWREGISTRATION_H
#define RAGNAROK_SOURCE_UI_WINDOWREGISTRATION_H

#include <list>

struct WindowAncestry;

// UIWindowMgr's contiguous list at +0x18C. Borrowed windows; destruction
// releases list nodes only. This does not replace the manager or its owners.
struct WindowRegistration {
  std::list<WindowAncestry*> windows;

  void Add(WindowAncestry* window);
  void Remove(WindowAncestry* window);
  // Entries are UIWindow parent subobjects; invalidates each, then releases
  // every node.
  void InvalidateAll();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WindowRegistration) == 8);
#endif

#endif
