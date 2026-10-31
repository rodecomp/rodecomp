#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_GRAVITYCONTROLEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_GRAVITYCONTROLEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CGravityControlEffect : public CEffect {
public:
  CGravityControlEffect() : CEffect(-1), target(0), unknown_00D0(0), unknown_00D4(-20.0f) {}

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  ~CGravityControlEffect() override;

  /* 00CC */ int target;
  /* 00D0 */ float unknown_00D0;
  /* 00D4 */ float unknown_00D4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGravityControlEffect) == 0xD8);
static_assert(offsetof(CGravityControlEffect, target) == 0xCC);
#endif

#endif
