#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_POWDERINGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_POWDERINGEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;
class CSpriteParticle;

class CPowderingEffect : public CEffect {
public:
  CPowderingEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot12() override;

  void InitSpriteParticle(CParticle* particle);
  void InitSmokeParticle(CParticle* particle);

  /* 00CC */ std::vector<CParticle*> smokeParticles;
  /* 00D8 */ std::vector<CSpriteParticle*> spriteParticles;
  /* 00E4 */ int unknown_00E4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPowderingEffect) == 0xE8);
static_assert(offsetof(CPowderingEffect, spriteParticles) == 0xD8);
static_assert(offsetof(CPowderingEffect, unknown_00E4) == 0xE4);
#endif

#endif
