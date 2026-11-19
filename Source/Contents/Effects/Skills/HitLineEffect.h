#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_HITLINEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_HITLINEEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

struct HitLineVector3d : vector3d {
  // INLINED
  HitLineVector3d() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
  }
};

class CHitLineEffect : public CEffect {
public:
  CHitLineEffect();
  ~CHitLineEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ HitLineVector3d previous[2];

private:
  void CreateLines();
  short LineDirection(float dx, float dz);
  void UpdateParticle(CParticle* particle);
  void RenderParticle(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CHitLineEffect, particles) == 0xCC);
static_assert(offsetof(CHitLineEffect, previous) == 0xD8);
static_assert(sizeof(CHitLineEffect) == 0xF0);
#endif

#endif
