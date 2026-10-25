#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDOBJECTS_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDOBJECTS_H

#include <list>

class CRenderObject;

struct WorldObjects {
  std::list<CRenderObject*>& objects;

  void Remove(CRenderObject* object);
  void Clear();
};

#endif
