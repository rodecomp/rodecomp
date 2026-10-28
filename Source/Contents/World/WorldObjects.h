#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDOBJECTS_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDOBJECTS_H

#include <cstdint>
#include <list>

class CGameObject;
class CRenderObject;

struct WorldObjects {
  std::list<CRenderObject*>& objects;

  void Remove(CRenderObject* object);
  void SendObjectMessage(CGameObject* sender, std::uint64_t identity, int message, int a, int b, int c, int d);
};

#endif
