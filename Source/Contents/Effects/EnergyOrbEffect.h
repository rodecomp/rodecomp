#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ENERGYORBEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ENERGYORBEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CEnergyOrbEffect : public CEffect {
public:
  CEnergyOrbEffect();
  void Slot01() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  ~CEnergyOrbEffect() override;

  void SetCount(int value);

  /* 00CC */ int count;
  /* 00D0 */ int mode;
  /* 00D4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEnergyOrbEffect, particles) == 0xD4);
static_assert(sizeof(CEnergyOrbEffect) == 0xE0);
#endif

#endif
