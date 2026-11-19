#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_STAREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_STAREFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CStarEffect : public CEffect {
public:
  CStarEffect();
  ~CStarEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void SetupParticle(CParticle* particle);
  void UpdateParticle(CParticle* particle);

  /* 00CC */ EffectPoint unknown_00CC[4];
  /* 00FC */ int unknown_00FC;
  /* 0100 */ std::vector<CParticle*> particles;

private:
  void AdvanceTexture(CParticle* particle);
  void UpdateAlpha(CParticle* particle);
  void UpdatePosition(CParticle* particle);
  void UpdateScale(CParticle* particle);
  void UpdateTransform(CParticle* particle);
  void RenderParticle(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CStarEffect, unknown_00CC) == 0xCC);
static_assert(offsetof(CStarEffect, unknown_00FC) == 0xFC);
static_assert(offsetof(CStarEffect, particles) == 0x100);
static_assert(sizeof(CStarEffect) == 0x10C);
#endif

#endif
