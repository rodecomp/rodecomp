#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SKILLEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SKILLEFFECTS_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <list>
#include <vector>

class CEZ2STRParticle;
class CParticle;
class CSpriteParticle;

class CMeteorEffect : public CEffect {
public:
  CMeteorEffect() : CEffect(-1), quakeFrameLow(12), quakeFrameHigh(0) {}

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  ~CMeteorEffect() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ int quakeFrameLow;
  /* 00DC */ int quakeFrameHigh;
};

class CShadowExceedNew : public CEffect {
public:
  CShadowExceedNew() : CEffect(2467), drawBackEmitters(0) {}

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  ~CShadowExceedNew() override;

  /* 00CC */ unsigned char drawBackEmitters;
  /* 00D0 */ std::list<CEZ2STRParticle*> frontEmitters;
  /* 00D8 */ std::list<CEZ2STRParticle*> backEmitters;
};

class CSprArrowEffect : public CEffect {
public:
  CSprArrowEffect()
      : CEffect(2404), spriteParticle(nullptr), startTick(0), startX(0), startY(0), startZ(0), velocityX(0),
        velocityY(0), velocityZ(0), travelSpeedScale(1.0f), heading(0), delayFrames(0), spriteFrame(0), launched(0),
        alive(1) {}

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  ~CSprArrowEffect() override;
  void SetBulletSprite(int job);
  /* 00CC */ CSpriteParticle* spriteParticle;
  /* 00D0 */ int startTick;
  /* 00D4 */ float startX;
  /* 00D8 */ float startY;
  /* 00DC */ float startZ;
  /* 00E0 */ float velocityX;
  /* 00E4 */ float velocityY;
  /* 00E8 */ float velocityZ;
  /* 00EC */ float travelSpeedScale;
  /* 00F0 */ float heading;
  /* 00F4 */ float delayFrames;
  /* 00F8 */ int spriteFrame;
  /* 00FC */ unsigned char launched;
  /* 00FD */ bool alive;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMeteorEffect) == 0xE0);
static_assert(offsetof(CMeteorEffect, quakeFrameLow) == 0xD8);
static_assert(sizeof(CShadowExceedNew) == 0xE0);
static_assert(offsetof(CShadowExceedNew, backEmitters) == 0xD8);
static_assert(sizeof(CSprArrowEffect) == 0x100);
static_assert(offsetof(CSprArrowEffect, alive) == 0xFD);
#endif

#endif
