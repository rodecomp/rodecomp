#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_STORMGUSTEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_STORMGUSTEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CStormGustEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CStormGustEffect, particles) == 0xCC);
static_assert(sizeof(CStormGustEffect) == 0xD8);
#endif

#endif
