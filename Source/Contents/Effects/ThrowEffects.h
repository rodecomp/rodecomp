#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_THROWEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_THROWEFFECTS_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CThrowItemEffect : public CEffect {
public:
  CThrowItemEffect();
  void Slot03() override;
  void Close() override;
  void Slot12() override;
  void InitSpinParticle(CParticle* particle);

  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ float unknown_00DC;
  /* 00E0 */ float unknown_00E0;
  /* 00E4 */ int unknown_00E4;
  /* 00E8 */ float unknown_00E8;
  /* 00EC */ float unknown_00EC;
  /* 00F0 */ float unknown_00F0;
  /* 00F4 */ float unknown_00F4;
  /* 00F8 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CThrowItemEffect, particles) == 0xF8);
#endif

class CVolcanoEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void AddRings();

  /* 00CC */ vector3d lastInner;
  /* 00D8 */ vector3d lastOuter;
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CVolcanoEffect, particles) == 0xE4);
#endif

#endif
