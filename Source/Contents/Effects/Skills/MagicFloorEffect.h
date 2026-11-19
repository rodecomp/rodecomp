#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MAGICFLOOREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MAGICFLOOREFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CMagicFloorEffect : public CEffect {
public:
  CMagicFloorEffect();
  ~CMagicFloorEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void ResetParticles() override;
  void AddFloors();
  void AddClouds(int layer);

  /* 00CC */ EffectPoint lastPoints[2];
  /* 00E4 */ EffectPoint unknown_00E4[2];
  /* 00FC */ std::vector<CParticle*> floors;
  /* 0108 */ std::vector<CParticle*> particles;

private:
  void UpdateRingVertices(CParticle* particle);
  void ResetFloor(CParticle* particle);
  void UpdateFloor(CParticle* particle);
  void UpdateCloud(CParticle* particle);
  void RenderFloor(CParticle* particle);
  void RenderCloud(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMagicFloorEffect, lastPoints) == 0xCC);
static_assert(offsetof(CMagicFloorEffect, unknown_00E4) == 0xE4);
static_assert(offsetof(CMagicFloorEffect, floors) == 0xFC);
static_assert(offsetof(CMagicFloorEffect, particles) == 0x108);
static_assert(sizeof(CMagicFloorEffect) == 0x114);
#endif

#endif
