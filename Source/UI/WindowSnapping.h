#ifndef RAGNAROK_SOURCE_UI_WINDOWSNAPPING_H
#define RAGNAROK_SOURCE_UI_WINDOWSNAPPING_H

#include "UI/SnapRectangle.h"

#include <list>
#include <map>

class CSnapMgr;
class UIWindow;

// Borrowed UIWindowMgr map+404, excluded list+324160 and screen extent+1148.
// Option141 is supplied from the actual service option owner by the caller.
struct WindowSnapping {
  CSnapMgr& graph;
  std::map<UIWindow*, SnapRectangle>& rectangles;
  const std::list<UIWindow*>& excluded;
  const bool& enabled;
  const int& screenWidth;
  const int& screenHeight;

  void Store(UIWindow* window);
  void Constrain(UIWindow* window, int& dx, int& dy, int groupMember);
};

#endif
