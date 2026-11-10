#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_FUUMASHOUAKUEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_FUUMASHOUAKUEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>

class CEZ2STRParticle;

class CFuumaShouakuEffect : public CEffect {
public:
  CFuumaShouakuEffect();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ CEZ2STRParticle* first;
  /* 00D0 */ CEZ2STRParticle* second;
  /* 00D4 */ float velocityX;
  /* 00D8 */ float velocityY;
  /* 00DC */ float velocityZ;
  /* 00E0 */ int travelFrames;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CFuumaShouakuEffect) == 0xE4);
static_assert(offsetof(CFuumaShouakuEffect, second) == 0xD0);
#endif

#endif
