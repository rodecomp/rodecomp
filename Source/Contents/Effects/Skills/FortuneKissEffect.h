#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_FORTUNEKISSEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_FORTUNEKISSEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectPoint.h"

#include <cstddef>
#include <vector>

class CParticle;

class CFortuneKiss : public CEffect {
public:
  CFortuneKiss();
  __declspec(noinline) ~CFortuneKiss() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ unsigned char padding_00CC[4];
  /* 00D0 */ EffectPoint unknown_00D0[2];
  /* 00E8 */ EffectPoint unknown_00E8[2];
  /* 0100 */ unsigned char padding_0100[4];
  /* 0104 */ int angle;
  /* 0108 */ std::vector<CParticle*> particles;

private:
  void InitializeParticle(CParticle* particle);
  void UpdateParticle(CParticle* particle);
  void RenderParticle(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CFortuneKiss) == 0x114);
static_assert(offsetof(CFortuneKiss, particles) == 0x108);
#endif

#endif
