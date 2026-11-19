#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_RAINBOWEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_RAINBOWEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRainbowEffect : public CEffect {
public:
  CRainbowEffect();
  ~CRainbowEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ EffectPoint previousPoints[2];
  /* 00E4 */ std::vector<CParticle*> bands;

private:
  void CreateBands();
  void UpdateBandVertices(CParticle* particle);
  void UpdateBand(CParticle* particle);
  void RenderBand(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CRainbowEffect, previousPoints) == 0xCC);
static_assert(offsetof(CRainbowEffect, bands) == 0xE4);
static_assert(sizeof(CRainbowEffect) == 0xF0);
#endif

#endif
