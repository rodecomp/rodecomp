#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ROTATEBGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ROTATEBGEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRotateBgEffect : public CEffect {
public:
  void Slot04() override;

  /* 00CC */ unsigned char unknown_00CC[0x38];
  /* 0104 */ std::vector<CParticle*> particles;
  /* 0110 */ std::vector<CParticle*> segmentParticles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CRotateBgEffect, particles) == 0x104);
static_assert(offsetof(CRotateBgEffect, segmentParticles) == 0x110);
static_assert(sizeof(CRotateBgEffect) == 0x11C);
#endif

#endif
