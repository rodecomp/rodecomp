#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_HEALSPEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_HEALSPEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <list>
#include <vector>

class CParticle;
class CSpriteParticle;

class CHealSp : public CEffect {
public:
  void Slot04() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ std::list<CSpriteParticle*> sprites;
  /* 00E0 */ unsigned char unknown_00E0[8];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CHealSp, particles) == 0xCC);
static_assert(offsetof(CHealSp, sprites) == 0xD8);
static_assert(sizeof(CHealSp) == 0xE8);
#endif

#endif
