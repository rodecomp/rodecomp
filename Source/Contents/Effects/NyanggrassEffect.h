#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_NYANGGRASSEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_NYANGGRASSEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CNyanggrassEffect : public CEffect {
public:
  CNyanggrassEffect();

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ int unknown_00D8;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CNyanggrassEffect) == 0xDC);
static_assert(offsetof(CNyanggrassEffect, unknown_00D8) == 0xD8);
#endif

#endif
