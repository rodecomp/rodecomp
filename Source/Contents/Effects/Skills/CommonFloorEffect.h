#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_COMMONFLOOREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_COMMONFLOOREFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <string>
#include <vector>

class CParticle;

class CCommonFloorEffect : public CEffect {
public:
  CCommonFloorEffect();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void ResetParticles() override;
  void SetColor(int red, int green, int blue) override;
  void SetParticleTexture(std::string name) override;

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCommonFloorEffect) == 0xD8);
static_assert(offsetof(CCommonFloorEffect, particles) == 0xCC);
#endif

#endif
