#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SOULCOLLECTEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SOULCOLLECTEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <list>

class CParticle;

class CSoulCollectEffect : public CEffect {
public:
  CSoulCollectEffect();
  ~CSoulCollectEffect() override;
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ std::list<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSoulCollectEffect) == 0xD4);
static_assert(offsetof(CSoulCollectEffect, particles) == 0xCC);
#endif

#endif
