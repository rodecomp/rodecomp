#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAGNUMBREAK_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAGNUMBREAK_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CMagnumBreak : public CEffect {
public:
  void Slot03() override;
  void Slot04() override;

  static void UpdateWave(CParticle* particle);

  /* 00CC */ std::vector<CParticle*> rings;
  /* 00D8 */ std::vector<CParticle*> waves;
  /* 00E4 */ float fadeStep;
  /* 00E8 */ int fadeFrame;
  /* 00EC */ int alpha;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMagnumBreak, waves) == 0xD8);
static_assert(offsetof(CMagnumBreak, alpha) == 0xEC);
static_assert(sizeof(CMagnumBreak) == 0xF0);
#endif

#endif
