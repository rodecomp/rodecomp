#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_CELLRANGEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_CELLRANGEEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CCellRangeEffect : public CEffect {
public:
  CCellRangeEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override {
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
