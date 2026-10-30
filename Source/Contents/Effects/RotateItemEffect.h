#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ROTATEITEMEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ROTATEITEMEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRotateItemEffect : public CEffect {
public:
  CRotateItemEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ int alpha;
  /* 00D0 */ float angle;
  /* 00D4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRotateItemEffect) == 0xE0);
static_assert(offsetof(CRotateItemEffect, alpha) == 0xCC);
static_assert(offsetof(CRotateItemEffect, particles) == 0xD4);
#endif

#endif
