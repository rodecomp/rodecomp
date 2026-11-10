#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_LEVEL99ORB1EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_LEVEL99ORB1EFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CLevel99Orb1Effect : public CEffect {
public:
  CLevel99Orb1Effect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ EffectPoint offsets[16];
  /* 018C */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLevel99Orb1Effect, offsets) == 0xCC);
static_assert(offsetof(CLevel99Orb1Effect, particles) == 0x18C);
static_assert(sizeof(CLevel99Orb1Effect) == 0x198);
#endif

#endif
