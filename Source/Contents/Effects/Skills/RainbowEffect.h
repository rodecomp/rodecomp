#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_RAINBOWEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_RAINBOWEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRainbowEffect : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

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
