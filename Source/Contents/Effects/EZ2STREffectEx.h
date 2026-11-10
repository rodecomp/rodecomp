#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECTEX_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECTEX_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

struct EZ2STREmitter;

class CEZ2STREffectEx : public CEffect {
public:
  CEZ2STREffectEx();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void SetLifeTime(int argument) override;
  void SetDelay(int argument) override;
  void SetAngle(float argument) override;
  void SetPitch(float argument) override;
  void SetYaw(float argument) override;
  void SetScale(float argument) override;
  void SetAlpha(float argument) override;
  void SetFlipX(int argument) override;
  void SetFlipY(int argument) override;
  void EnableTimeLimit() override;
  void EnableGroundRender() override;

  bool IsFinished();
  void EndLoop(unsigned char fadeOut);

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ unsigned char timed;
  /* 00D1 */ unsigned char padding_00D1[3];
  /* 00D4 */ int frameStep;
  /* 00D8 */ unsigned char alternateRender;
};

float ScreenAngle(const float* from, const float* to, const float* matrix);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEZ2STREffectEx, emitter) == 0xCC);
static_assert(offsetof(CEZ2STREffectEx, alternateRender) == 0xD8);
#endif

#endif
