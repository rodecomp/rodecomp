#ifndef RAGNAROK_SOURCE_UI_LAYOUT_WINDOWSNAPPING_H
#define RAGNAROK_SOURCE_UI_LAYOUT_WINDOWSNAPPING_H

#include "UI/Layout/SnapRectangle.h"

#include <list>
#include <map>

class CSnapMgr;
class UIWindow;

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
