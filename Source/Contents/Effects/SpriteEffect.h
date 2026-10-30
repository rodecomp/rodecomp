#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SPRITEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SPRITEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CSpriteParticle;

class CSpriteEffect : public CEffect {
public:
  CSpriteEffect();

  void Slot01() override;
  void Close() override;

  void SetupParticle(CSpriteParticle* particle);

  /* 00CC */ std::vector<CSpriteParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSpriteEffect, particles) == 0xCC);
static_assert(sizeof(CSpriteEffect) == 0xD8);
#endif

#endif
