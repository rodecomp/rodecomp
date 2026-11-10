#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_HITLINEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_HITLINEEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CHitLineEffect : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ vector3d previous[2];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CHitLineEffect, particles) == 0xCC);
static_assert(offsetof(CHitLineEffect, previous) == 0xD8);
static_assert(sizeof(CHitLineEffect) == 0xF0);
#endif

#endif
