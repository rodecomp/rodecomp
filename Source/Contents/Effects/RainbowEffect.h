#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAINBOWEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAINBOWEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRainbowEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ vector3d previousOuter;
  /* 00D8 */ vector3d previousInner;
  /* 00E4 */ std::vector<CParticle*> bands;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CRainbowEffect, previousOuter) == 0xCC);
static_assert(offsetof(CRainbowEffect, previousInner) == 0xD8);
static_assert(offsetof(CRainbowEffect, bands) == 0xE4);
static_assert(sizeof(CRainbowEffect) == 0xF0);
#endif

#endif
