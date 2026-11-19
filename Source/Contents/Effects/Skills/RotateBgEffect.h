#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_ROTATEBGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_ROTATEBGEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRotateBgEffect : public CEffect {
public:
  CRotateBgEffect();
  ~CRotateBgEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ float height;
  /* 00D0 */ float radius;
  /* 00D4 */ float segmentScale;
  /* 00D8 */ int angle;
  /* 00DC */ int segmentAngle;
  /* 00E0 */ int segmentSpan;
  /* 00E4 */ int segmentAlpha;
  /* 00E8 */ unsigned char segmentReady;
  /* 00EC */ EffectPoint previousPoints[2];
  /* 0104 */ std::vector<CParticle*> particles;
  /* 0110 */ std::vector<CParticle*> segmentParticles;

private:
  void CreateRings();
  void CreateSegments();
  void UpdateRing(CParticle* particle);
  void UpdateSegment(CParticle* particle);
  void UpdateSegmentVertices(CParticle* particle);
  void RenderRing(CParticle* particle);
  void RenderSegment(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CRotateBgEffect, segmentReady) == 0xE8);
static_assert(offsetof(CRotateBgEffect, previousPoints) == 0xEC);
static_assert(offsetof(CRotateBgEffect, particles) == 0x104);
static_assert(offsetof(CRotateBgEffect, segmentParticles) == 0x110);
static_assert(sizeof(CRotateBgEffect) == 0x11C);
#endif

#endif
