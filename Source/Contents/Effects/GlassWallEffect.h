#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_GLASSWALLEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_GLASSWALLEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CGlassWall : public CEffect {
public:
  ~CGlassWall() override;
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void AddPanel(float angle, float scale, float x, float y, float z);

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ CEffect* effect;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CGlassWall, particles) == 0xCC);
static_assert(offsetof(CGlassWall, effect) == 0xD8);
static_assert(sizeof(CGlassWall) == 0xDC);
#endif

#endif
