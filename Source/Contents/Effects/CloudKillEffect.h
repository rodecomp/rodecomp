#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CLOUDKILLEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CLOUDKILLEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <list>

class CParticle;

class CCloudKillEffect : public CEffect {
public:
  CCloudKillEffect();
  ~CCloudKillEffect() override;
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void AddSprites();

  /* 00CC */ std::list<CParticle*> particles;
  /* 00D4 */ std::list<CParticle*> sprites;
  /* 00DC */ int unknown_00DC;
  /* 00E0 */ int unknown_00E0;
  /* 00E4 */ unsigned char unknown_00E4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCloudKillEffect, particles) == 0xCC);
static_assert(offsetof(CCloudKillEffect, unknown_00E4) == 0xE4);
#endif

#endif
