#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_LEVEL150EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_LEVEL150EFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CLevel150Effect : public CEffect {
public:
  CLevel150Effect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void SetColor(int red, int green, int blue) override;
  void SetDestinationBlend(int blend) override;
  void AddBubbleRing(int alphaOffset);
  void AddShock(int kind);

  /* 00CC */ EffectPoint lastPoints[2];
  /* 00E4 */ std::vector<CParticle*> bubbles;
  /* 00F0 */ std::vector<CParticle*> shocks;
};

class CLevel150SubEffect : public CEffect {
public:
  CLevel150SubEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void SetColor(int red, int green, int blue) override;
  void SetDestinationBlend(int blend) override;
  void Init();

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLevel150Effect, lastPoints) == 0xCC);
static_assert(offsetof(CLevel150Effect, bubbles) == 0xE4);
static_assert(offsetof(CLevel150Effect, shocks) == 0xF0);
static_assert(sizeof(CLevel150Effect) == 0xFC);
static_assert(offsetof(CLevel150SubEffect, particles) == 0xCC);
static_assert(sizeof(CLevel150SubEffect) == 0xD8);
#endif

#endif
