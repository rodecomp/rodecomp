#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CLIMAXEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CLIMAXEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CParticle;

class CClimaxEffect : public CEffect {
public:
  CClimaxEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ CParticle* particles[2];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CClimaxEffect) == 0xD4);
static_assert(offsetof(CClimaxEffect, particles) == 0xCC);
#endif

#endif
