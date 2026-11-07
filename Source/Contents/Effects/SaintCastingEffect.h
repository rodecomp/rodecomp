#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SAINTCASTINGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SAINTCASTINGEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CSaintCastingEffect : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void AddRing(const char* texture, int alpha);

  /* 00CC */ vector3d lastInner;
  /* 00D8 */ vector3d lastOuter;
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSaintCastingEffect, lastInner) == 0xCC);
static_assert(offsetof(CSaintCastingEffect, particles) == 0xE4);
#endif

#endif
