#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_LIGHTSPHEREEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_LIGHTSPHEREEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CLightSphereEffect : public CEffect {
public:
  CLightSphereEffect();
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ std::vector<CParticle*> spheres;
  /* 00D8 */ std::vector<CParticle*> rays;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLightSphereEffect, spheres) == 0xCC);
static_assert(offsetof(CLightSphereEffect, rays) == 0xD8);
static_assert(sizeof(CLightSphereEffect) == 0xE4);
#endif

#endif
