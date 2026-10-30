#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_STAREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_STAREFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CStarEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void SetupParticle(CParticle* particle);
  void UpdateParticle(CParticle* particle);

  /* 00CC */ unsigned char unknown_00CC[0x34];
  /* 0100 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CStarEffect, particles) == 0x100);
static_assert(sizeof(CStarEffect) == 0x10C);
#endif

#endif
