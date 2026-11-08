#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_THROWEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_THROWEFFECTS_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CThrowItemEffect : public CEffect {
public:
  CThrowItemEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void InitEvents() override;
  void InitFlameParticle(CParticle* particle);
  void InitPotionParticle(CParticle* particle, int kind);
  void InitSpinParticle(CParticle* particle);
  void InitTrajectory(CParticle* particle);

  /* 00CC */ int travelFrames;
  /* 00D0 */ int launchDelay;
  /* 00D4 */ int projectileCount;
  /* 00D8 */ int trailCount;
  /* 00DC */ float heightDelta;
  /* 00E0 */ float arcHeight;
  /* 00E4 */ int trailAlpha;
  /* 00E8 */ float trailRotation;
  /* 00EC */ float trailX;
  /* 00F0 */ float trailY;
  /* 00F4 */ float trailZ;
  /* 00F8 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CThrowItemEffect, particles) == 0xF8);
#endif

class CVolcanoEffect : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void AddRings();

  /* 00CC */ vector3d lastInner;
  /* 00D8 */ vector3d lastOuter;
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CVolcanoEffect, particles) == 0xE4);
#endif

#endif
