#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_JUPITELTHUNDERSTORMEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_JUPITELTHUNDERSTORMEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CJupitelThunderStormEffect : public CEffect {
public:
  CJupitelThunderStormEffect();

  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ CParticle* particle;
  /* 00D0 */ std::vector<CParticle*> particles;
  /* 00DC */ vector3d step;
  /* 00E8 */ int duration;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CJupitelThunderStormEffect) == 0xEC);
static_assert(offsetof(CJupitelThunderStormEffect, particles) == 0xD0);
static_assert(offsetof(CJupitelThunderStormEffect, duration) == 0xE8);
#endif

#endif
