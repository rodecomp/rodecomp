#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>

class CGameActor : public CRenderObject {
public:
  /* 0138 */ unsigned char unknown_0138[0x100];
  /* 0238 */ unsigned char unknown_0238;
  /* 0239 */ unsigned char unknown_0239[0x27];
  /* 0260 */ int unknown_0260;
  /* 0264 */ unsigned char unknown_0264[0x64];
  /* 02C8 */ int unknown_02C8;
  /* 02CC */ unsigned char unknown_02CC[0x18];
  /* 02E4 */ int unknown_02E4;
  /* 02E8 */ unsigned char unknown_02E8[0x1D];
  /* 0305 */ unsigned char unknown_0305;
  /* 0306 */ unsigned char unknown_0306[2];
  /* 0308 */ int unknown_0308;
  /* 030C */ unsigned char unknown_030C[8];
  /* 0314 */ unsigned char unknown_0314;
  /* 0315 */ unsigned char unknown_0315[3];
  /* 0318 */ float unknown_0318[6];
  /* 0330 */ float unknown_0330[6];
  /* 0348 */ float unknown_0348[6];
  /* 0360 */ float unknown_0360[6];
  /* 0378 */ float unknown_0378;
  /* 037C */ unsigned char unknown_037C[0x4C];
  /* 03C8 */ int unknown_03C8;

  void SetAnimation(unsigned animation, int motion, int repeat) override;

  virtual void Slot18(int first, int second, int third);
  virtual void Slot21(int value);
  virtual void Slot22(int value);
  virtual void Slot23(int first, int second);
  virtual void Slot25(int value);
  virtual void Slot30();

  void SetUnknown0305(unsigned char value, int extra);
  void SetUnknown0318(const float* value, int index);
  void SetUnknown0330(const float* value, int index);
  void RaiseUnknown03C8(int value);
  void SetUnknown02E4(int value);
  void SetUnknown02C8(int value);
  void SetUnknown0260(int value);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CGameActor, unknown_0238) == 0x238);
static_assert(offsetof(CGameActor, unknown_0260) == 0x260);
static_assert(offsetof(CGameActor, unknown_02C8) == 0x2C8);
static_assert(offsetof(CGameActor, unknown_02E4) == 0x2E4);
static_assert(offsetof(CGameActor, unknown_0305) == 0x305);
static_assert(offsetof(CGameActor, unknown_0308) == 0x308);
static_assert(offsetof(CGameActor, unknown_0314) == 0x314);
static_assert(offsetof(CGameActor, unknown_0318) == 0x318);
static_assert(offsetof(CGameActor, unknown_0330) == 0x330);
static_assert(offsetof(CGameActor, unknown_0348) == 0x348);
static_assert(offsetof(CGameActor, unknown_0360) == 0x360);
static_assert(offsetof(CGameActor, unknown_0378) == 0x378);
static_assert(offsetof(CGameActor, unknown_03C8) == 0x3C8);
#endif

#endif
