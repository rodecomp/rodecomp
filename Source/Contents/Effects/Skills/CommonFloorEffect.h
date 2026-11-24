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
  ~CCommonFloorEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void ResetParticles() override;
  void SetColor(short red, short green, short blue) override;
  void SetParticleTexture(std::string name) override;

  /* 00CC */ std::vector<CParticle*> particles;

private:
  void SpawnParticles();
  void ResetParticle(CParticle* particle);
  void UpdateParticle(CParticle* particle);
  __declspec(noinline) void RenderParticle(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCommonFloorEffect) == 0xD8);
static_assert(offsetof(CCommonFloorEffect, particles) == 0xCC);
#endif

#endif
