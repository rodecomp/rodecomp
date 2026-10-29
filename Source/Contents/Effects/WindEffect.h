#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_WINDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_WINDEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CWindEffect : public CEffect {
public:
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ unsigned char unknown_00CC[0x60];
  /* 012C */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CWindEffect) == 0x138);
static_assert(offsetof(CWindEffect, particles) == 0x12C);
#endif

#endif
