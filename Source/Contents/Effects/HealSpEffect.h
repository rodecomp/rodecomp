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
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void AddRing(float inner, float outer, float height, int duration);

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ std::list<CSpriteParticle*> sprites;
  /* 00E0 */ int unknown_00E0;
  /* 00E4 */ int unknown_00E4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CHealSp, particles) == 0xCC);
static_assert(offsetof(CHealSp, sprites) == 0xD8);
static_assert(sizeof(CHealSp) == 0xE8);
#endif

#endif
