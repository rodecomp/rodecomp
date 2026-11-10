#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RECTUPEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RECTUPEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectPoint.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRectUpEffect : public CEffect {
public:
  CRectUpEffect();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void InitEvents() override;
  virtual void ProcessParticle(CParticle* particle);
  virtual void InitParticle(CParticle* particle);

  /* 00CC */ EffectPoint previousEdge[2];
  /* 00E4 */ int particleLifetime;
  /* 00E8 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRectUpEffect) == 0xF4);
static_assert(offsetof(CRectUpEffect, particleLifetime) == 0xE4);
static_assert(offsetof(CRectUpEffect, particles) == 0xE8);
#endif

#endif
