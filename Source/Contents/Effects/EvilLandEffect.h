#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EVILLANDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EVILLANDEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CEvilLandEffect : public CEffect {
public:
  CEvilLandEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEvilLandEffect, particles) == 0xCC);
#endif

#endif
