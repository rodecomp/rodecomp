#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99ORB2EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99ORB2EFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CLevel99Orb2Effect : public CEffect {
public:
  void Slot04() override;

  /* 00CC */ unsigned char unknown_00CC[0xC0];
  /* 018C */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLevel99Orb2Effect, particles) == 0x18C);
static_assert(sizeof(CLevel99Orb2Effect) == 0x198);
#endif

#endif
