#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_ENERGYORBEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_ENERGYORBEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CEnergyOrbEffect : public CEffect {
public:
  CEnergyOrbEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  __forceinline ~CEnergyOrbEffect() override;

  void SetCount(int value);

  /* 00CC */ int count;
  /* 00D0 */ int mode;
  /* 00D4 */ std::vector<CParticle*> particles;

private:
  void ConfigureParticle(CParticle* particle);
  void UpdateParticle(CParticle* particle);
  void RenderParticle(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEnergyOrbEffect, particles) == 0xD4);
static_assert(sizeof(CEnergyOrbEffect) == 0xE0);
#endif

#endif
