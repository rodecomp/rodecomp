#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_FUUMASHOUAKUEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_FUUMASHOUAKUEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CEZ2STRParticle;

class CFuumaShouakuEffect : public CEffect {
public:
  CFuumaShouakuEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ CEZ2STRParticle* first;
  /* 00D0 */ CEZ2STRParticle* second;
  /* 00D4 */ float unknown_00D4;
  /* 00D8 */ float unknown_00D8;
  /* 00DC */ float unknown_00DC;
  /* 00E0 */ int unknown_00E0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CFuumaShouakuEffect) == 0xE4);
static_assert(offsetof(CFuumaShouakuEffect, second) == 0xD0);
#endif

#endif
