#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_FLOWERCASTINGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_FLOWERCASTINGEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CFlowerCastingEffect : public CEffect {
public:
  CFlowerCastingEffect();
  ~CFlowerCastingEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void AddRing(int kind);
  void UpdateSegmentVertices(CParticle* particle);
  void UpdateSegment(CParticle* particle);
  void RenderSegment(CParticle* particle);

  /* 00CC */ EffectPoint lastPoints[2];
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CFlowerCastingEffect) == 0xF0);
static_assert(offsetof(CFlowerCastingEffect, lastPoints) == 0xCC);
static_assert(offsetof(CFlowerCastingEffect, particles) == 0xE4);
#endif

#endif
