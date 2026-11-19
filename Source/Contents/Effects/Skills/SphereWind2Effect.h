#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SPHEREWIND2EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SPHEREWIND2EFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CSphereWind2Effect : public CEffect {
public:
  CSphereWind2Effect();
  ~CSphereWind2Effect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ EffectPoint lastPoints[2];
  /* 00E4 */ std::vector<CParticle*> particles;

private:
  void CreateRings(int angle);
  void UpdateSegmentVertices(CParticle* particle);
  void UpdateSegment(CParticle* particle);
  void RenderSegment(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSphereWind2Effect, lastPoints) == 0xCC);
static_assert(offsetof(CSphereWind2Effect, particles) == 0xE4);
#endif

#endif
