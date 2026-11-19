#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SAINTCASTINGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SAINTCASTINGEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CSaintCastingEffect : public CEffect {
public:
  CSaintCastingEffect();
  ~CSaintCastingEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void AddRing(const char* texture, int alpha);
  static void __stdcall SetRingColor(CParticle* particle, int effectId);
  static void __stdcall InitializeRing(CParticle* particle, int effectId);
  void UpdateParticle(CParticle* particle);
  void RenderParticle(CParticle* particle);

  /* 00CC */ EffectPoint lastPoints[2];
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSaintCastingEffect, lastPoints) == 0xCC);
static_assert(offsetof(CSaintCastingEffect, particles) == 0xE4);
#endif

#endif
