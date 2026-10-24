#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTLISTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTLISTS_H

#include "Contents/Effect.h"

#include <map>

// Borrowed CEffectMgr active map (+0x10), retirement list (+0x18) and
// selected effect-id index (+0x20). The index borrows active effect pointers.
// Map keys use signed actor-id ordering. Moving to retirement does not delete
// an effect: queued geometry may still borrow its storage until frame cleanup.
struct EffectLists {
  std::map<int, std::list<CEffect*>>& active;
  EffectRetirement& retirement;
  std::map<int, std::vector<CEffect*>>& indexed;

  CEffect* Find(int actor, int id) const;
  CEffect* Find(int actor, int instance, int id) const;
  CEffect* At(int id, unsigned index) const;
  void Add(int actor, CEffect* effect);
  void Retire(CEffect* effect, unsigned char removeActive);
  void Retire(int actor, int id);
  void Retire(int actor, int instance, int id);
  void RetireActor(int actor);
  void Clear();

private:
  void RemoveActive(CEffect* effect);
};

#endif
