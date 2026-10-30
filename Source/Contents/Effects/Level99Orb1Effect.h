#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99ORB1EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99ORB1EFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CLevel99Orb1Effect : public CEffect {
public:
  CLevel99Orb1Effect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ vector3d offsets[16];
  /* 018C */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLevel99Orb1Effect, offsets) == 0xCC);
static_assert(offsetof(CLevel99Orb1Effect, particles) == 0x18C);
static_assert(sizeof(CLevel99Orb1Effect) == 0x198);
#endif

#endif
