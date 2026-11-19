#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_STEALEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_STEALEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <list>

class CSpriteParticle;

class CSteal : public CEffect {
public:
  CSteal();
  ~CSteal() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void SpawnParticles();

  /* 00CC */ std::list<CSpriteParticle*> particles;
  /* 00D4 */ EffectPoint offsets[10];

private:
  __declspec(noinline) void UpdateParticle(CSpriteParticle* particle);
  __declspec(noinline) void RenderParticle(CSpriteParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSteal, offsets) == 0xD4);
static_assert(sizeof(CSteal) == 0x14C);
#endif

#endif
