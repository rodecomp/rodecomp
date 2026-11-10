#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SCATTEREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SCATTEREFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CSpriteParticle;

class CScatterEffect : public CEffect {
public:
  CScatterEffect();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::vector<CSpriteParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CScatterEffect) == 0xD8);
static_assert(offsetof(CScatterEffect, particles) == 0xCC);
#endif

#endif
