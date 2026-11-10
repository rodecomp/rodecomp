#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99EFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectPoint.h"

#include <cstddef>
#include <string>
#include <vector>

class CParticle;

class CLevel99Effect : public CEffect {
public:
  CLevel99Effect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void SetColor(int red, int green, int blue) override;
  void SetParticleTexture(std::string name) override;

  void AddRings();

  /* 00CC */ EffectPoint previousRingPoints[2];
  /* 00E4 */ std::vector<CParticle*> sparkles;
  /* 00F0 */ std::vector<CParticle*> sprites;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLevel99Effect, previousRingPoints) == 0xCC);
static_assert(offsetof(CLevel99Effect, sparkles) == 0xE4);
static_assert(offsetof(CLevel99Effect, sprites) == 0xF0);
#endif

#endif
