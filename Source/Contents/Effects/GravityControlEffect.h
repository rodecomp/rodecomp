#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_GRAVITYCONTROLEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_GRAVITYCONTROLEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CGravityControlEffect : public CEffect {
public:
  CGravityControlEffect() : CEffect(-1), target(0), heightOffset(0), minimumHeightOffset(-20.0f) {}

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  ~CGravityControlEffect() override;

  /* 00CC */ int target;
  /* 00D0 */ float heightOffset;
  /* 00D4 */ float minimumHeightOffset;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGravityControlEffect) == 0xD8);
static_assert(offsetof(CGravityControlEffect, target) == 0xCC);
#endif

#endif
