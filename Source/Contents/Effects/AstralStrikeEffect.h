#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ASTRALSTRIKEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ASTRALSTRIKEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

struct EZ2STREmitter;

class CAstralStrikeEffect : public CEffect {
public:
  CAstralStrikeEffect();

  /* 00CC */ std::vector<EZ2STREmitter*> unknown_00CC;
  /* 00D8 */ std::vector<EZ2STREmitter*> unknown_00D8;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAstralStrikeEffect) == 0xE4);
static_assert(offsetof(CAstralStrikeEffect, unknown_00D8) == 0xD8);
#endif

#endif
