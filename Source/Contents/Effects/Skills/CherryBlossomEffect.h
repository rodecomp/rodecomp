#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CHERRYBLOSSOMEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CHERRYBLOSSOMEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CCherryBlossomEffect : public CEffect {
public:
  CCherryBlossomEffect();
  ~CCherryBlossomEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override {
    return this->ActorExists();
  }
  void UpdatePetal(CParticle* particle);
  void RenderPetal(CParticle* particle);

  /* 00CC */ EffectPoint offsets[16];
  /* 018C */ std::vector<CParticle*> petals;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCherryBlossomEffect, offsets) == 0xCC);
static_assert(offsetof(CCherryBlossomEffect, petals) == 0x18C);
#endif

#endif
