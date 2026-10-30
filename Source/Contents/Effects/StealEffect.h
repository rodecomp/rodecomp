#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_STEALEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_STEALEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <list>

class CSpriteParticle;

class CSteal : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void SpawnParticles();

  /* 00CC */ std::list<CSpriteParticle*> particles;
  /* 00D4 */ vector3d offsets[10];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSteal, offsets) == 0xD4);
static_assert(sizeof(CSteal) == 0x14C);
#endif

#endif
