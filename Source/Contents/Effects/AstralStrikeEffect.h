#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ASTRALSTRIKEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ASTRALSTRIKEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CEZ2STRParticle;

class CAstralStrikeEffect : public CEffect {
public:
  CAstralStrikeEffect();

  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void Init();
  void SpawnStrike();

  /* 00CC */ std::vector<CEZ2STRParticle*> emitters;
  /* 00D8 */ std::vector<CEZ2STRParticle*> strikes;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAstralStrikeEffect) == 0xE4);
static_assert(offsetof(CAstralStrikeEffect, strikes) == 0xD8);
#endif

#endif
