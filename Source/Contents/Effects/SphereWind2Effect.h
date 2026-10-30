#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SPHEREWIND2EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SPHEREWIND2EFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CSphereWind2Effect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ vector3d lastOuter;
  /* 00D8 */ vector3d lastInner;
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSphereWind2Effect, lastOuter) == 0xCC);
static_assert(offsetof(CSphereWind2Effect, lastInner) == 0xD8);
static_assert(offsetof(CSphereWind2Effect, particles) == 0xE4);
#endif

#endif
