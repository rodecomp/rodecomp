#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ELECTRICEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ELECTRICEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CElectricEffect : public CEffect {
public:
  void Slot04() override;

  /* 00CC */ unsigned char unknown_00CC[0x38];
  /* 0104 */ std::vector<CParticle*> bolts;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CElectricEffect, bolts) == 0x104);
#endif

#endif
