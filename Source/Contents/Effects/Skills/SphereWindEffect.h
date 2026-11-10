#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SPHEREWINDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SPHEREWINDEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CSphereWindEffect : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void UpdateParticle(CParticle* particle);

  /* 00CC */ vector3d lastInner;
  /* 00D8 */ vector3d lastOuter;
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSphereWindEffect, lastInner) == 0xCC);
static_assert(offsetof(CSphereWindEffect, particles) == 0xE4);
#endif

#endif
