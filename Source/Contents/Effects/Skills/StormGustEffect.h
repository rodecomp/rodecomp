#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_STORMGUSTEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_STORMGUSTEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CStormGustEffect : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CStormGustEffect, particles) == 0xCC);
static_assert(sizeof(CStormGustEffect) == 0xD8);
#endif

#endif
