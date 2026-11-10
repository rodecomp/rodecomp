#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_GLASSWALLEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_GLASSWALLEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CGlassWall : public CEffect {
public:
  ~CGlassWall() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void AddPanel(float angle, float scale, vector3d offset);

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ CEffect* effect;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CGlassWall, particles) == 0xCC);
static_assert(offsetof(CGlassWall, effect) == 0xD8);
static_assert(sizeof(CGlassWall) == 0xDC);
#endif

#endif
