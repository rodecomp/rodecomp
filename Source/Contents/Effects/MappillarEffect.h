#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAPPILLAREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAPPILLAREFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CMappillarEffect : public CEffect {
public:
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ unsigned char unknown_00CC[0x18];
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMappillarEffect, particles) == 0xE4);
static_assert(sizeof(CMappillarEffect) == 0xF0);
#endif

#endif
