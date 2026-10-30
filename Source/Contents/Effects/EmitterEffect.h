#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EMITTEREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EMITTEREFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/ParticlePool.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <string>
#include <vector>

class CParticle;

class CEmitterEffect : public CEffect {
public:
  CEmitterEffect();
  void Slot01() override;
  void Slot02() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot07() override;
  ~CEmitterEffect() override;
  virtual void InitParticle(CParticle* particle);
  virtual void UpdateParticle(CParticle* particle);

  void SetCount(int value);
  void SetParticleSpeed(float value);
  void SetTexture(std::string name);

  /* 00CC */ vector3d velocityMin;
  /* 00D8 */ vector3d velocityMax;
  /* 00E4 */ vector3d acceleration;
  /* 00F0 */ vector3d spread;
  /* 00FC */ float emitCarry;
  /* 0100 */ float timeScale;
  /* 0104 */ float scaleMin;
  /* 0108 */ float scaleMax;
  /* 010C */ float lifeMin;
  /* 0110 */ float lifeMax;
  /* 0114 */ float rateMin;
  /* 0118 */ float rateMax;
  /* 011C */ float width;
  /* 0120 */ float height;
  /* 0124 */ int red;
  /* 0128 */ int green;
  /* 012C */ int blue;
  /* 0130 */ int alpha;
  /* 0134 */ std::string texture;
  /* 014C */ int sourceBlend;
  /* 0150 */ int destinationBlend;
  /* 0154 */ int count;
  /* 0158 */ float particleSpeed;
  /* 015C */ unsigned char depth;
  /* 015D */ unsigned char billboard;
  /* 015E */ unsigned char unknown_015E[2];
  /* 0160 */ float rotation[3];
  /* 016C */ unsigned char fixedAlpha;
  /* 016D */ unsigned char unknown_016D[3];
  /* 0170 */ std::vector<CParticle*> particles;
  /* 017C */ ParticlePool pool;
  /* 0188 */ int duration;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEmitterEffect, velocityMin) == 0xCC);
static_assert(offsetof(CEmitterEffect, texture) == 0x134);
static_assert(offsetof(CEmitterEffect, rotation) == 0x160);
static_assert(offsetof(CEmitterEffect, particles) == 0x170);
static_assert(offsetof(CEmitterEffect, pool) == 0x17C);
static_assert(sizeof(CEmitterEffect) == 0x18C);
#endif

#endif
