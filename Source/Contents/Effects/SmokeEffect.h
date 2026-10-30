#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SMOKEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SMOKEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CSpriteParticle;

class CSmokeEffect : public CEffect {
public:
  CSmokeEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ int visibleCount;
  /* 00D0 */ std::vector<CSpriteParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSmokeEffect) == 0xDC);
static_assert(offsetof(CSmokeEffect, visibleCount) == 0xCC);
static_assert(offsetof(CSmokeEffect, particles) == 0xD0);
#endif

#endif
