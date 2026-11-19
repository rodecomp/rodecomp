#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CLOUDKILLEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CLOUDKILLEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <list>

class CParticle;
class CSpriteParticle;

class CCloudKillEffect : public CEffect {
public:
  CCloudKillEffect();
  ~CCloudKillEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void AddParticle();
  void AddSprites();
  void UpdateSprite(CSpriteParticle* particle);
  void RenderParticle(CParticle* particle);
  void RenderSprite(CParticle* particle);

  /* 00CC */ std::list<CParticle*> particles;
  /* 00D4 */ std::list<CParticle*> sprites;
  /* 00DC */ int particleLifetimeFrames;
  /* 00E0 */ int spriteLifetimeFrames;
  /* 00E4 */ unsigned char emitting;

private:
  void UpdateParticle(CSpriteParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCloudKillEffect, particles) == 0xCC);
static_assert(offsetof(CCloudKillEffect, emitting) == 0xE4);
#endif

#endif
