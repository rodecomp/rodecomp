#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_AMACHINEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_AMACHINEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <map>

class CEZ2STRParticle;

class CAMachineEffect : public CEffect {
public:
  CAMachineEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void PlayAttack();
  void SetState(int state);

  /* 00CC */ std::map<int, CEZ2STRParticle*> unknown_00CC;
  /* 00D4 */ int unknown_00D4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAMachineEffect) == 0xD8);
static_assert(offsetof(CAMachineEffect, unknown_00D4) == 0xD4);
#endif

#endif
