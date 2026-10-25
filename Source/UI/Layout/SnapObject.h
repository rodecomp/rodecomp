#ifndef RAGNAROK_SOURCE_UI_LAYOUT_SNAPOBJECT_H
#define RAGNAROK_SOURCE_UI_LAYOUT_SNAPOBJECT_H

#include <list>

class UIWindow;

class SnapObject {
public:
  SnapObject();
  virtual ~SnapObject();
  int AdjacentSide(const SnapObject& other) const;
  void Remove(SnapObject* object);
  bool Contains(SnapObject* target);
  bool Connected(SnapObject* target);
  void AccumulateHeight(int& height, int side) const;

  /* 0004 */ UIWindow* window;
  /* 0008 */ std::list<SnapObject*> edges[4];
  /* 0028 */ int visited;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SnapObject) == 44);
#endif

#endif
