#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MAPPILLAREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MAPPILLAREFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CMappillarEffect : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ vector3d lastInner;
  /* 00D8 */ vector3d lastOuter;
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMappillarEffect, particles) == 0xE4);
static_assert(sizeof(CMappillarEffect) == 0xF0);
#endif

#endif
