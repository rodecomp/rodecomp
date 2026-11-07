#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EVILLANDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EVILLANDEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CEvilLandEffect : public CEffect {
public:
  CEvilLandEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEvilLandEffect, particles) == 0xCC);
#endif

#endif
