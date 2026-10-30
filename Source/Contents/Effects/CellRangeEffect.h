#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CELLRANGEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CELLRANGEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CCellRangeEffect : public CEffect {
public:
  CCellRangeEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override {
    return this->ActorExists();
  }
  __declspec(noinline) void SetCellTexture(CParticle* particle);

  /* 00CC */ float cellSize;
  /* 00D0 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCellRangeEffect, cellSize) == 0xCC);
static_assert(offsetof(CCellRangeEffect, particles) == 0xD0);
static_assert(sizeof(CCellRangeEffect) == 0xDC);
#endif

#endif
