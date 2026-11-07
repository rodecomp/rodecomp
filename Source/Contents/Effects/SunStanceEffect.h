#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SUNSTANCEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SUNSTANCEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <list>

class CParticle;

class CSunStanceEffect : public CEffect {
public:
  CSunStanceEffect();
  ~CSunStanceEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::list<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSunStanceEffect) == 0xD4);
static_assert(offsetof(CSunStanceEffect, particles) == 0xCC);
#endif

#endif
