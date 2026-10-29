#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RECTUPEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RECTUPEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRectUpEffect : public CEffect {
public:
  CRectUpEffect();

  void Slot12() override;

  /* 00CC */ vector3d unknown_00CC[2];
  /* 00E4 */ int unknown_00E4;
  /* 00E8 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRectUpEffect) == 0xF4);
static_assert(offsetof(CRectUpEffect, unknown_00E4) == 0xE4);
static_assert(offsetof(CRectUpEffect, particles) == 0xE8);
#endif

#endif
