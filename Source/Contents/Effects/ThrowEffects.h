#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_THROWEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_THROWEFFECTS_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CThrowItemEffect : public CEffect {
public:
  void Slot12() override;
};

class CVolcanoEffect : public CEffect {
public:
  bool Slot06() override;

  /* 00CC */ unsigned char unknown_00CC[0x18];
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CVolcanoEffect, particles) == 0xE4);
#endif

#endif
