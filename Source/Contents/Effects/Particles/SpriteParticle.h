#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLES_SPRITEPARTICLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLES_SPRITEPARTICLE_H

#include "Contents/Effects/Particles/Particle.h"

#include <cstddef>

class CActRes;
class CSprRes;

class CSpriteParticle : public CParticle {
public:
  CSpriteParticle(vector3d position, int flags);
  void Reset();
  void UpdateFrame();
  void SetAction(const char* name);
  void SetSprite(const char* name);
  void Draw(char mode);

  /* 02BC */ unsigned char applyActorExtent;
  /* 02BD */ unsigned char applyActorLift;
  /* 02BE */ unsigned char padding_02BE[2];
  /* 02C0 */ float screenOffsetX;
  /* 02C4 */ float screenOffsetY;
  /* 02C8 */ vector3d velocity;
  /* 02D4 */ int spinBits;
  /* 02D8 */ float frameDelay;
  /* 02DC */ unsigned char accelerationStorage[4];
  /* 02E0 */ CSprRes* sprite;
  /* 02E4 */ CActRes* action;
  /* 02E8 */ int animation;
  /* 02EC */ int frame;
  /* 02F0 */ int loopMode;
  /* 02F4 */ int actorId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSpriteParticle, velocity) == 0x2C8);
static_assert(offsetof(CSpriteParticle, frameDelay) == 0x2D8);
static_assert(offsetof(CSpriteParticle, loopMode) == 0x2F0);
static_assert(sizeof(CSpriteParticle) == 0x2F8);
#endif

#endif
