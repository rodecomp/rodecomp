#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_WINDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_WINDEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <string>
#include <vector>

class CParticle;

class CWindEffect : public CEffect {
public:
  CWindEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot07() override;

  void Init();

  /* 00CC */ unsigned short ringCount = 4;
  /* 00CE */ unsigned char maxAlpha = 200;
  /* 00CF */ unsigned char red = 255;
  /* 00D0 */ unsigned char green = 100;
  /* 00D1 */ unsigned char blue = 100;
  /* 00D4 */ float particleScale = 12.0f;
  /* 00D8 */ float width = 0.5f;
  /* 00DC */ float height = 50.0f;
  /* 00E0 */ float angleSpeed = 5.0f;
  /* 00E4 */ unsigned short arcLength = 200;
  /* 00E8 */ vector3d rotation = {};
  /* 00F4 */ int sourceBlend = 5;
  /* 00F8 */ int destinationBlend = 7;
  /* 00FC */ std::string textureName = "effect\\cloud11.tga";
  /* 0114 */ vector3d lastBottom = {};
  /* 0120 */ vector3d lastTop = {};
  /* 012C */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CWindEffect) == 0x138);
static_assert(offsetof(CWindEffect, maxAlpha) == 0xCE);
static_assert(offsetof(CWindEffect, particleScale) == 0xD4);
static_assert(offsetof(CWindEffect, rotation) == 0xE8);
static_assert(offsetof(CWindEffect, textureName) == 0xFC);
static_assert(offsetof(CWindEffect, particles) == 0x12C);
#endif

#endif
