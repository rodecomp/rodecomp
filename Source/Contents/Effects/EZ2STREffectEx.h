#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECTEX_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECTEX_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

struct EZ2STREmitter;

class CEZ2STREffectEx : public CEffect {
public:
  CEZ2STREffectEx();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot14(int argument) override;
  void Slot16(int argument) override;
  void Slot17(float argument) override;
  void Slot18(float argument) override;
  void Slot19(float argument) override;
  void Slot22(float argument) override;
  void Slot23(float argument) override;
  void Slot26(int argument) override;
  void Slot27(int argument) override;
  void Slot31() override;
  void Slot32() override;

  bool IsFinished();
  void EndLoop(unsigned char fadeOut);

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ unsigned char timed;
  /* 00D1 */ unsigned char unknown_00D1[3];
  /* 00D4 */ int frameStep;
  /* 00D8 */ unsigned char alternateRender;
};

float ScreenAngle(const float* from, const float* to, const float* matrix);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEZ2STREffectEx, emitter) == 0xCC);
static_assert(offsetof(CEZ2STREffectEx, alternateRender) == 0xD8);
#endif

#endif
