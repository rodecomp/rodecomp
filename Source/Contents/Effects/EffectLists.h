#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTLISTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTLISTS_H

#include "Contents/Effects/Effect.h"

#include <map>

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

extern EffectLists* g_effectMgr;

#endif
