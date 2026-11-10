#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_HEALSPEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_HEALSPEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <list>
#include <vector>

class CParticle;
class CSpriteParticle;

class CHealSp : public CEffect {
public:
  ~CHealSp() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void AddRing(float inner, float outer, float height, int duration);

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ std::list<CSpriteParticle*> sprites;
  /* 00E0 */ int holdFrame;
  /* 00E4 */ int alphaLevel;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CHealSp, particles) == 0xCC);
static_assert(offsetof(CHealSp, sprites) == 0xD8);
static_assert(sizeof(CHealSp) == 0xE8);
#endif

#endif
