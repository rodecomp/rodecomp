#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_TETRABALLEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_TETRABALLEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CEZ2STRParticle;

class CTetraBallEffect : public CEffect {
public:
  CTetraBallEffect();

  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  unsigned char ActiveCount();
  void Spread();
  void MoveFirstTo(int index);

  /* 00CC */ CEZ2STRParticle* balls[5];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CTetraBallEffect, balls) == 0xCC);
static_assert(sizeof(CTetraBallEffect) == 0xE0);
#endif

#endif
