#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_FLOWERCASTINGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_FLOWERCASTINGEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CFlowerCastingEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void AddRing(int kind);

  /* 00CC */ vector3d lastInner;
  /* 00D8 */ vector3d lastOuter;
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CFlowerCastingEffect) == 0xF0);
static_assert(offsetof(CFlowerCastingEffect, lastInner) == 0xCC);
static_assert(offsetof(CFlowerCastingEffect, lastOuter) == 0xD8);
static_assert(offsetof(CFlowerCastingEffect, particles) == 0xE4);
#endif

#endif
