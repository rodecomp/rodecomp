#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLEFFECTS_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <list>
#include <vector>

class CEZ2STRParticle;
class CParticle;

class CMeteorEffect : public CEffect {
public:
  CMeteorEffect() : CEffect(-1), unknown_00D8(12), unknown_00DC(0) {}

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  ~CMeteorEffect() override;

  /* 00CC */ std::vector<CParticle*> unknown_00CC;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ int unknown_00DC;
};

class CShadowExceedNew : public CEffect {
public:
  CShadowExceedNew() : CEffect(2467), unknown_00CC(0) {}

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  ~CShadowExceedNew() override;

  /* 00CC */ unsigned char unknown_00CC;
  /* 00D0 */ std::list<CEZ2STRParticle*> frontEmitters;
  /* 00D8 */ std::list<CEZ2STRParticle*> backEmitters;
};

class CSprArrowEffect : public CEffect {
public:
  CSprArrowEffect()
      : CEffect(2404), unknown_00CC(0), unknown_00D0(0), unknown_00D4(0), unknown_00D8(0), unknown_00DC(0), unknown_00E0(0),
        unknown_00E4(0), unknown_00E8(0), unknown_00EC(1.0f), unknown_00F0(0), unknown_00F4(0), unknown_00F8(0), unknown_00FC(0),
        unknown_00FD(1) {}

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  ~CSprArrowEffect() override;

  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ float unknown_00D4;
  /* 00D8 */ float unknown_00D8;
  /* 00DC */ float unknown_00DC;
  /* 00E0 */ float unknown_00E0;
  /* 00E4 */ float unknown_00E4;
  /* 00E8 */ float unknown_00E8;
  /* 00EC */ float unknown_00EC;
  /* 00F0 */ float unknown_00F0;
  /* 00F4 */ float unknown_00F4;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ unsigned char unknown_00FC;
  /* 00FD */ bool unknown_00FD;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMeteorEffect) == 0xE0);
static_assert(offsetof(CMeteorEffect, unknown_00D8) == 0xD8);
static_assert(sizeof(CShadowExceedNew) == 0xE0);
static_assert(offsetof(CShadowExceedNew, backEmitters) == 0xD8);
static_assert(sizeof(CSprArrowEffect) == 0x100);
static_assert(offsetof(CSprArrowEffect, unknown_00FD) == 0xFD);
#endif

#endif
