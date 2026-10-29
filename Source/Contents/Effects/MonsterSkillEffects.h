#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MONSTERSKILLEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MONSTERSKILLEFFECTS_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectEmitter.h"

#include <cstddef>
#include <vector>

class CCrimsonArrowEffect : public CEffect {
public:
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CRootTwistEffect : public CEffect {
public:
  void Slot03() override;
  bool Slot06() override;
  void Slot12() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CSpriteMableEffect : public CEffect {
public:
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CStemSpearEffect : public CEffect {
public:
  void Slot03() override;
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CSvgSpiritEffect : public CEffect {
public:
  void Slot03() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CTunaPartyEffect : public CEffect {
public:
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CCurseOfCubeEffect : public CEffect {
public:
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CWarningPlaneEffect : public CEffect {
public:
  void Slot01() override;
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ unsigned char fadingIn;
  /* 00D9 */ unsigned char unknown_00D9[3];
  /* 00DC */ unsigned int expireTime;
};

class CCylinderEffect : public CEffect {
public:
  bool Slot06() override;

  /* 00CC */ unsigned int startTime;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCrimsonArrowEffect, emitter) == 0xCC);
static_assert(sizeof(CRootTwistEffect) == 0xD8);
static_assert(sizeof(CSvgSpiritEffect) == 0xD0);
static_assert(offsetof(CWarningPlaneEffect, expireTime) == 0xDC);
static_assert(offsetof(CCylinderEffect, emitters) == 0xD0);
#endif

#endif
