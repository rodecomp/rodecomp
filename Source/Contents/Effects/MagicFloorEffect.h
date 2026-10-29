#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAGICFLOOREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAGICFLOOREFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CMagicFloorEffect : public CEffect {
public:
  void Slot04() override;

  /* 00CC */ unsigned char unknown_00CC[0x30];
  /* 00FC */ std::vector<CParticle*> floors;
  /* 0108 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMagicFloorEffect, floors) == 0xFC);
static_assert(offsetof(CMagicFloorEffect, particles) == 0x108);
#endif

#endif
