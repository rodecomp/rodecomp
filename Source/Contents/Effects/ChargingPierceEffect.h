#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHARGINGPIERCEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHARGINGPIERCEEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <list>

class CParticle;
class CEZ2STRParticle;

class CChargingPierceEffect : public CEffect {
public:
  CChargingPierceEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  int ActiveCount();

  /* 00CC */ vector3d offsets[10];
  /* 0144 */ CParticle* sparks[10];
  /* 016C */ CEZ2STRParticle* emitters[10];
};

class CChargingPierceNewEffect : public CEffect {
public:
  CChargingPierceNewEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void SetCount(int count);

  /* 00CC */ std::list<CEZ2STRParticle*> emitters;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CChargingPierceEffect) == 0x194);
static_assert(offsetof(CChargingPierceEffect, sparks) == 0x144);
static_assert(offsetof(CChargingPierceEffect, emitters) == 0x16C);
static_assert(sizeof(CChargingPierceNewEffect) == 0xD4);
#endif

#endif
