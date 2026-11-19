#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_REDLINEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_REDLINEEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRedLineEffect : public CEffect {
public:
  CRedLineEffect();
  ~CRedLineEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void CreateLines(int layer);

  /* 00CC */ EffectPoint previousPoints[2];
  /* 00E4 */ std::vector<CParticle*> lines;

private:
  void UpdateSegmentVertices(CParticle* particle);
  void UpdateSegment(CParticle* particle);
  void RenderSegment(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CRedLineEffect, previousPoints) == 0xCC);
static_assert(offsetof(CRedLineEffect, lines) == 0xE4);
static_assert(sizeof(CRedLineEffect) == 0xF0);
#endif

#endif
