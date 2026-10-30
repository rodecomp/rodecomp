#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECTEX_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECTEX_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CParticle;

class CEZ2STREffectEx : public CEffect {
public:
  CEZ2STREffectEx();
  void Close() override;
  void Slot31() override;

  /* 00CC */ CParticle* particle;
  /* 00D0 */ unsigned char finished;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ unsigned char unknown_00D8;
};

float ScreenAngle(const float* from, const float* to, const float* matrix);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEZ2STREffectEx, particle) == 0xCC);
static_assert(offsetof(CEZ2STREffectEx, unknown_00D8) == 0xD8);
#endif

#endif
