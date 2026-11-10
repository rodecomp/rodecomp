#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_BINDTRAPEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_BINDTRAPEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CBindTrapEffect : public CEffect {
public:
  CBindTrapEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void AddParticle(int index);

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CBindTrapEffect, particles) == 0xCC);
#endif

#endif
