#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>

class CGameActor : public CRenderObject {
public:
  /* 0138 */ unsigned char unknown_0138[0x128];
  /* 0260 */ int unknown_0260;
  /* 0264 */ unsigned char unknown_0264[0x64];
  /* 02C8 */ int unknown_02C8;
  /* 02CC */ unsigned char unknown_02CC[0x18];
  /* 02E4 */ int unknown_02E4;
  /* 02E8 */ unsigned char unknown_02E8[0x1D];
  /* 0305 */ unsigned char unknown_0305;
  /* 0306 */ unsigned char unknown_0306[2];
  /* 0308 */ int unknown_0308;
  /* 030C */ unsigned char unknown_030C[0xBC];
  /* 03C8 */ int unknown_03C8;

  void SetUnknown0305(unsigned char value, int extra);
  void RaiseUnknown03C8(int value);
  void SetUnknown02E4(int value);
  void SetUnknown02C8(int value);
  void SetUnknown0260(int value);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CGameActor, unknown_0260) == 0x260);
static_assert(offsetof(CGameActor, unknown_02C8) == 0x2C8);
static_assert(offsetof(CGameActor, unknown_02E4) == 0x2E4);
static_assert(offsetof(CGameActor, unknown_0305) == 0x305);
static_assert(offsetof(CGameActor, unknown_0308) == 0x308);
static_assert(offsetof(CGameActor, unknown_03C8) == 0x3C8);
#endif

#endif
