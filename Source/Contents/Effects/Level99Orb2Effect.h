#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99ORB2EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99ORB2EFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CLevel99Orb2Effect : public CEffect {
public:
  CLevel99Orb2Effect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  ~CLevel99Orb2Effect() override;

  /* 00CC */ EffectPoint offsets[4][4];
  /* 018C */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLevel99Orb2Effect, offsets) == 0xCC);
static_assert(offsetof(CLevel99Orb2Effect, particles) == 0x18C);
static_assert(sizeof(CLevel99Orb2Effect) == 0x198);
#endif

#endif
