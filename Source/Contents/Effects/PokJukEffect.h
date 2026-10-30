#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_POKJUKEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_POKJUKEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CPokJukEffect : public CEffect {
public:
  bool Slot06() override;

  /* 00CC */ vector3d trails[640][4];
  /* 78CC */ int states[640];
  /* 82CC */ int beats[640];
  /* 8CCC */ int delays[640];
  /* 96CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CPokJukEffect, trails) == 0xCC);
static_assert(offsetof(CPokJukEffect, states) == 0x78CC);
static_assert(offsetof(CPokJukEffect, beats) == 0x82CC);
static_assert(offsetof(CPokJukEffect, delays) == 0x8CCC);
static_assert(offsetof(CPokJukEffect, particles) == 0x96CC);
static_assert(sizeof(CPokJukEffect) == 0x96D8);
#endif

#endif
