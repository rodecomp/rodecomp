#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SCATTEREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SCATTEREFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CSpriteParticle;

class CScatterEffect : public CEffect {
public:
  CScatterEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ std::vector<CSpriteParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CScatterEffect) == 0xD8);
static_assert(offsetof(CScatterEffect, particles) == 0xCC);
#endif

#endif
