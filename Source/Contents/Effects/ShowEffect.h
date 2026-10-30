#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SHOWEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SHOWEFFECT_H

#include "Contents/Effects/Effect.h"

#include <map>

class CShowEffect : public CEffect {
public:
  /* 00CC */ std::map<int, int> effects;

  CShowEffect();
  void Close() override;
  void Add(int tag, int effectId);
  void Remove(int tag);
  unsigned Count() const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CShowEffect, effects) == 0xCC);
static_assert(sizeof(CShowEffect) == 0xD4);
#endif

#endif
