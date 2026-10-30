#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ROTATELINEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ROTATELINEEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRotateLineEffect : public CEffect {
public:
  CRotateLineEffect();

  void Slot03() override;
  void Slot04() override;

  /* 00CC */ float scale;
  /* 00D0 */ int angle;
  /* 00D4 */ int spread;
  /* 00D8 */ int alpha;
  /* 00DC */ bool started;
  /* 00E0 */ vector3d previous[2];
  /* 00F8 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRotateLineEffect) == 0x104);
static_assert(offsetof(CRotateLineEffect, started) == 0xDC);
static_assert(offsetof(CRotateLineEffect, previous) == 0xE0);
static_assert(offsetof(CRotateLineEffect, particles) == 0xF8);
#endif

#endif
