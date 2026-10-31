#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ANIMATEDEMITTEREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ANIMATEDEMITTEREFFECT_H

#include "Contents/Effects/EmitterEffect.h"

#include <cstddef>
#include <string>
#include <vector>

class CParticle;

class CAnimatedEmitterEffect : public CEmitterEffect {
public:
  CAnimatedEmitterEffect();
  void Slot01() override;
  void InitParticle(CParticle* particle) override;
  void UpdateParticle(CParticle* particle) override;

  /* 018C */ std::vector<std::string> frames[2];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CAnimatedEmitterEffect, frames) == 0x18C);
static_assert(sizeof(CAnimatedEmitterEffect) == 0x1A4);
#endif

#endif
