#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_EVILLANDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_EVILLANDEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;
class CMultiTexParticle;

class CEvilLandEffect : public CEffect {
public:
  CEvilLandEffect();
  ~CEvilLandEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::vector<CParticle*> particles;

private:
  void ScrollParticleTexture(CMultiTexParticle* particle);
  void RenderParticle(CMultiTexParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEvilLandEffect, particles) == 0xCC);
#endif

#endif
