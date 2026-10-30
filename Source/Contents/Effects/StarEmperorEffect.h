#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_STAREMPEROREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_STAREMPEROREFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CStarEmperorEffect : public CEffect {
public:
  CStarEmperorEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot12() override;

  void BuildSphere();
  void UpdateRing(CParticle* particle);

  /* 00CC */ std::vector<CParticle*> sphere;
  /* 00D8 */ std::vector<CParticle*> rings;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CStarEmperorEffect, sphere) == 0xCC);
static_assert(offsetof(CStarEmperorEffect, rings) == 0xD8);
static_assert(sizeof(CStarEmperorEffect) == 0xE4);
#endif

#endif
