#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDOBJECTS_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDOBJECTS_H

#include <list>

class CRenderObject;

// Borrowed CWorld owning list at +0x08. Actor/spatial indexes are separate
// owners and must be updated by their original caller, not by this operation.
struct WorldObjects {
  std::list<CRenderObject*>& objects;

  void Remove(CRenderObject* object);
  void Clear();
};

#endif
