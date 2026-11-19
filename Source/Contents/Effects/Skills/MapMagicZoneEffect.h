#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MAPMAGICZONEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MAPMAGICZONEEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CMapMagicZoneEffect : public CEffect {
public:
  CMapMagicZoneEffect();
  ~CMapMagicZoneEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void AddSpinner(int kind);
  void AddPetals();
  void AddRibbon();
  void UpdateRibbonVertices(CParticle* particle);
  void UpdatePetal(CParticle* particle);
  __declspec(noinline) void UpdateSpinner(CParticle* particle);
  void UpdateRibbon(CParticle* particle);
  void RenderSpinner(CParticle* particle);
  void RenderPetal(CParticle* particle);
  void RenderRibbon(CParticle* particle);

  /* 00CC */ EffectPoint previousPoints[2];
  /* 00E4 */ std::vector<CParticle*> spinners;
  /* 00F0 */ std::vector<CParticle*> petals;
  /* 00FC */ std::vector<CParticle*> ribbon;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMapMagicZoneEffect, previousPoints) == 0xCC);
static_assert(offsetof(CMapMagicZoneEffect, spinners) == 0xE4);
static_assert(offsetof(CMapMagicZoneEffect, petals) == 0xF0);
static_assert(offsetof(CMapMagicZoneEffect, ribbon) == 0xFC);
static_assert(sizeof(CMapMagicZoneEffect) == 0x108);
#endif

#endif
