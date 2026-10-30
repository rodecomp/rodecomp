#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_BINDTRAPEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_BINDTRAPEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CBindTrapEffect : public CEffect {
public:
  CBindTrapEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void AddParticle(int index);

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CBindTrapEffect, particles) == 0xCC);
#endif

#endif
