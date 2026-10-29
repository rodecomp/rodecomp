#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_POWDERINGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_POWDERINGEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CPowderingEffect : public CEffect {
public:
  CPowderingEffect();

  void Slot12() override;

  /* 00CC */ std::vector<CParticle*> smokeParticles;
  /* 00D8 */ std::vector<CParticle*> spriteParticles;
  /* 00E4 */ int unknown_00E4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPowderingEffect) == 0xE8);
static_assert(offsetof(CPowderingEffect, spriteParticles) == 0xD8);
static_assert(offsetof(CPowderingEffect, unknown_00E4) == 0xE4);
#endif

#endif
