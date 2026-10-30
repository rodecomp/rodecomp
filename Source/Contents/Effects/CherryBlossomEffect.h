#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHERRYBLOSSOMEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHERRYBLOSSOMEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CCherryBlossomEffect : public CEffect {
public:
  CCherryBlossomEffect();
  void Slot01() override;
  void Slot03() override;
  void Close() override;
  bool Slot06() override {
    return this->ActorExists();
  }

  /* 00CC */ vector3d offsets[16];
  /* 018C */ std::vector<CParticle*> petals;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCherryBlossomEffect, offsets) == 0xCC);
static_assert(offsetof(CCherryBlossomEffect, petals) == 0x18C);
#endif

#endif
