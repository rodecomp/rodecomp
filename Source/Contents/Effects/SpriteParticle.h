#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SPRITEPARTICLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SPRITEPARTICLE_H

#include "Contents/Effects/Particle.h"

#include <cstddef>

class CActRes;
class CSprRes;

class CSpriteParticle : public CParticle {
public:
  void UpdateFrame();
  void SetAction(const char* name);
  void SetSprite(const char* name);
  void sub_B40F30(char mode);

  /* 02BC */ unsigned char unknown_02BC;
  /* 02BD */ unsigned char unknown_02BD;
  /* 02BE */ unsigned char unknown_02BE[2];
  /* 02C0 */ float unknown_02C0;
  /* 02C4 */ float unknown_02C4;
  /* 02C8 */ vector3d velocity;
  /* 02D4 */ int unknown_02D4;
  /* 02D8 */ float frameDelay;
  /* 02DC */ unsigned char unknown_02DC[4];
  /* 02E0 */ CSprRes* sprite;
  /* 02E4 */ CActRes* action;
  /* 02E8 */ int animation;
  /* 02EC */ int frame;
  /* 02F0 */ int loopMode;
  /* 02F4 */ int unknown_02F4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSpriteParticle, velocity) == 0x2C8);
static_assert(offsetof(CSpriteParticle, frameDelay) == 0x2D8);
static_assert(offsetof(CSpriteParticle, loopMode) == 0x2F0);
static_assert(sizeof(CSpriteParticle) == 0x2F8);
#endif

#endif
