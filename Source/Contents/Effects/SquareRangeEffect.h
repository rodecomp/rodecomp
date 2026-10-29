#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SQUARERANGEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SQUARERANGEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CSquareRangeEffect : public CEffect {
public:
  CSquareRangeEffect();
  void Slot03() override;
  void Slot04() override;

  /* 00CC */ float size;
  /* 00D0 */ unsigned char resize;
  /* 00D4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSquareRangeEffect, particles) == 0xD4);
static_assert(sizeof(CSquareRangeEffect) == 0xE0);
#endif

#endif
