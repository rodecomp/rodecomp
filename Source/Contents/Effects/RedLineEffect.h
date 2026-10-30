#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_REDLINEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_REDLINEEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRedLineEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void CreateLines(int layer);

  /* 00CC */ vector3d previousInner;
  /* 00D8 */ vector3d previousOuter;
  /* 00E4 */ std::vector<CParticle*> lines;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CRedLineEffect, previousInner) == 0xCC);
static_assert(offsetof(CRedLineEffect, previousOuter) == 0xD8);
static_assert(offsetof(CRedLineEffect, lines) == 0xE4);
static_assert(sizeof(CRedLineEffect) == 0xF0);
#endif

#endif
