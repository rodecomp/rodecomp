#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_FRIGGSONGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_FRIGGSONGEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CFriggSongEffect : public CEffect {
public:
  CFriggSongEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot12() override;

  /* 00CC */ vector3d lastInner[2];
  /* 00E4 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CFriggSongEffect) == 0xF0);
static_assert(offsetof(CFriggSongEffect, particles) == 0xE4);
#endif

#endif
