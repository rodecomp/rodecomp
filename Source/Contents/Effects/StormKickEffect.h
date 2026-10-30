#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_STORMKICKEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_STORMKICKEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CStormKickEffect : public CEffect {
public:
  CStormKickEffect();

  void Slot01() override;
  void Slot03() override;
  void Close() override;
  bool Slot06() override;
  void Slot12() override;
  void CreateStorms(int layer);
  void CreateClouds(int layer);
  void UpdateStorm(CParticle* particle);

  /* 00CC */ EffectPoint lastPoints[2];
  /* 00E4 */ std::vector<CParticle*> storms;
  /* 00F0 */ std::vector<CParticle*> clouds;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CStormKickEffect, lastPoints) == 0xCC);
static_assert(offsetof(CStormKickEffect, storms) == 0xE4);
static_assert(offsetof(CStormKickEffect, clouds) == 0xF0);
static_assert(sizeof(CStormKickEffect) == 0xFC);
#endif

#endif
