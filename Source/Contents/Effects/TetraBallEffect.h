#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_TETRABALLEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_TETRABALLEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CEZ2STRParticle;

class CTetraBallEffect : public CEffect {
public:
  CTetraBallEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void AddBall(int id, int index);
  void RemoveBall(int index);
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
