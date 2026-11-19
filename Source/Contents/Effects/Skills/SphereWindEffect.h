#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SPHEREWINDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SPHEREWINDEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CSphereWindEffect : public CEffect {
public:
  CSphereWindEffect();
  ~CSphereWindEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void UpdateParticle(CParticle* particle);

  /* 00CC */ EffectPoint lastEdges[2];
  /* 00E4 */ std::vector<CParticle*> particles;

private:
  void CreateRings(int start);
  void UpdateVertices(CParticle* particle);
  void RenderSegment(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSphereWindEffect, lastEdges) == 0xCC);
static_assert(offsetof(CSphereWindEffect, particles) == 0xE4);
#endif

#endif
