#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAPPILLAREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAPPILLAREFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CMappillarEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ vector3d lastInner;
  /* 00D8 */ vector3d lastOuter;
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMappillarEffect, particles) == 0xE4);
static_assert(sizeof(CMappillarEffect) == 0xF0);
#endif

#endif
