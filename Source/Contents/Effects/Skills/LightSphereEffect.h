#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_LIGHTSPHEREEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_LIGHTSPHEREEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CLightSphereEffect : public CEffect {
public:
  CLightSphereEffect();
  ~CLightSphereEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void CreateRayParticles(int param);
  void UpdateRayVertices(CParticle* particle);
  void UpdateRay(CParticle* particle);

  /* 00CC */ std::vector<CParticle*> spheres;
  /* 00D8 */ std::vector<CParticle*> rays;

private:
  void CreateSphereParticles();
  void UpdateSphere(CParticle* particle);
  void RenderRay(CParticle* particle);
  void RenderSphere(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLightSphereEffect, spheres) == 0xCC);
static_assert(offsetof(CLightSphereEffect, rays) == 0xD8);
static_assert(sizeof(CLightSphereEffect) == 0xE4);
#endif

#endif
