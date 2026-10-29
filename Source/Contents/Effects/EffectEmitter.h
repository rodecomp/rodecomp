#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTEMITTER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTEMITTER_H

#include "Utility/Math/Vector3d.h"

#include <cstddef>

struct EZ2STREmitter {
  /* 0000 */ unsigned char unknown_0000[0x24];
  /* 0024 */ vector3d position;
  /* 0030 */ unsigned char unknown_0030[0x14];
  /* 0044 */ int alpha;
  /* 0048 */ unsigned char unknown_0048[0x8];
  /* 0050 */ int baseAngle;
  /* 0054 */ unsigned char unknown_0054[0x70];
  /* 00C4 */ float angle;
  /* 00C8 */ unsigned char unknown_00C8[0x4];
  /* 00CC */ float lifetime;
  /* 00D0 */ unsigned char unknown_00D0[0x18];
  /* 00E8 */ int elapsed;
  /* 00EC */ unsigned char unknown_00EC[0x1DC];
  /* 02C8 */ vector3d velocity;
  /* 02D4 */ unsigned char unknown_02D4[0x4];
  /* 02D8 */ float speed;
  /* 02DC */ unsigned char unknown_02DC[0x39E8];
  /* 3CC4 */ int currentFrame;
  /* 3CC8 */ int frameCount;
  /* 3CCC */ unsigned char unknown_3CCC[0x290];
  /* 3F5C */ unsigned char looping;
  /* 3F5D */ unsigned char fadeOut;
  /* 3F5E */ unsigned char unknown_3F5E[0x3A];
  /* 3F98 */ int unknown_3F98;
  /* 3F9C */ int unknown_3F9C;
  /* 3FA0 */ int duration;
  /* 3FA4 */ float unknown_3FA4;
  /* 3FA8 */ float unknown_3FA8;
  /* 3FAC */ float unknown_3FAC;
  /* 3FB0 */ float unknown_3FB0;
  /* 3FB4 */ float remainingTime;
  /* 3FB8 */ unsigned char unknown_3FB8[0x30];
  /* 3FE8 */ unsigned char unknown_3FE8;
  /* 3FE9 */ unsigned char unknown_3FE9;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(EZ2STREmitter, position) == 0x24);
static_assert(offsetof(EZ2STREmitter, alpha) == 0x44);
static_assert(offsetof(EZ2STREmitter, angle) == 0xC4);
static_assert(offsetof(EZ2STREmitter, elapsed) == 0xE8);
static_assert(offsetof(EZ2STREmitter, velocity) == 0x2C8);
static_assert(offsetof(EZ2STREmitter, speed) == 0x2D8);
static_assert(offsetof(EZ2STREmitter, currentFrame) == 0x3CC4);
#endif

#endif
