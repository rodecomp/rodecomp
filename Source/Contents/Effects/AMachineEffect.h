#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_AMACHINEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_AMACHINEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <map>

struct EZ2STREmitter;

class CAMachineEffect : public CEffect {
public:
  CAMachineEffect();

  /* 00CC */ std::map<int, EZ2STREmitter*> unknown_00CC;
  /* 00D4 */ int unknown_00D4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAMachineEffect) == 0xD8);
static_assert(offsetof(CAMachineEffect, unknown_00D4) == 0xD4);
#endif

#endif
