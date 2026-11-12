#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_STORMCANNONEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_STORMCANNONEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CEZ2STRParticle;

class CStormCannonEffect : public CEffect {
public:
  CStormCannonEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  // INLINED
  int& Alpha() {
    return this->baseAlpha;
  }

  /* 00CC */ std::vector<CEZ2STRParticle*> emitters;
  /* 00D8 */ vector3d direction = {};
  /* 00E4 */ vector3d origin = {};
  /* 00F0 */ double speed;
  /* 00F8 */ double distance;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CStormCannonEffect) == 0x100);
static_assert(offsetof(CStormCannonEffect, direction) == 0xD8);
static_assert(offsetof(CStormCannonEffect, speed) == 0xF0);
static_assert(offsetof(CStormCannonEffect, distance) == 0xF8);
#endif

#endif
