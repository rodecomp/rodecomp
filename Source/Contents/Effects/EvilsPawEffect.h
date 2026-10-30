#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EVILSPAWEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EVILSPAWEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <string>
#include <vector>

class CParticle;

class CEvilsPawEffect : public CEffect {
public:
  CEvilsPawEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot07() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ int frameDelay;
  /* 00DC */ int size;
  /* 00E0 */ int delayCounter;
  /* 00E4 */ vector3d offset;
  /* 00F0 */ std::vector<std::string> textures;
  /* 00FC */ int textureIndex;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CEvilsPawEffect) == 0x100);
static_assert(offsetof(CEvilsPawEffect, particles) == 0xCC);
static_assert(offsetof(CEvilsPawEffect, offset) == 0xE4);
static_assert(offsetof(CEvilsPawEffect, textures) == 0xF0);
static_assert(offsetof(CEvilsPawEffect, textureIndex) == 0xFC);
#endif

#endif
