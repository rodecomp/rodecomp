#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ROTATEBGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ROTATEBGEFFECT_H

#include "Contents/Effects/Effect.h"

#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRotateBgEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ float height;
  /* 00D0 */ float radius;
  /* 00D4 */ float segmentScale;
  /* 00D8 */ int angle;
  /* 00DC */ int segmentAngle;
  /* 00E0 */ int segmentSpan;
  /* 00E4 */ int segmentAlpha;
  /* 00E8 */ unsigned char segmentReady;
  /* 00EC */ vector3d lastInner;
  /* 00F8 */ vector3d lastOuter;
  /* 0104 */ std::vector<CParticle*> particles;
  /* 0110 */ std::vector<CParticle*> segmentParticles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CRotateBgEffect, segmentReady) == 0xE8);
static_assert(offsetof(CRotateBgEffect, lastInner) == 0xEC);
static_assert(offsetof(CRotateBgEffect, particles) == 0x104);
static_assert(offsetof(CRotateBgEffect, segmentParticles) == 0x110);
static_assert(sizeof(CRotateBgEffect) == 0x11C);
#endif

#endif
