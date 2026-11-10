#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_AMACHINEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_AMACHINEEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <map>

class CEZ2STRParticle;

class CAMachineEffect : public CEffect {
public:
  CAMachineEffect();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void PlayAttack();
  __declspec(noinline) void SetState(int state);

  /* 00CC */ std::map<int, CEZ2STRParticle*> stateParticles;
  /* 00D4 */ int state;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAMachineEffect) == 0xD8);
static_assert(offsetof(CAMachineEffect, state) == 0xD4);
#endif

#endif
