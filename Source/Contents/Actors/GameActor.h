#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H

#include "Contents/Actors/RenderObject.h"
#include "Contents/MovementRouteInterpolation.h"

#include <cstddef>

enum {
  BODYSTATE_NORMAL,
  BODYSTATE_STONECURSE,
  BODYSTATE_FREEZING,
  BODYSTATE_STUN,
  BODYSTATE_SLEEP,
  BODYSTATE_UNDEAD,
  BODYSTATE_STONECURSE_ING,
};

class CGameActor : public CRenderObject {
public:
  ~CGameActor() override;

  /* 0138 */ unsigned char unknown_0138[0xE8];
  /* 0220 */ MovementRouteInterpolation route;
  /* 0238 */ unsigned char unknown_0238;
  /* 0239 */ unsigned char unknown_0239[0x1B];
  /* 0254 */ std::uint32_t routeStartTick;
  /* 0258 */ unsigned char unknown_0258[0x04];
  /* 025C */ int unknown_025C;
  /* 0260 */ int unknown_0260;
  /* 0264 */ unsigned char unknown_0264[0x58];
  /* 02BC */ int bodyState;
  /* 02C0 */ unsigned char unknown_02C0[0x08];
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
  /* 03CC */ unsigned char unknown_03CC[0x38];
  /* 0404 */ int unknown_0404;
  /* 0408 */ int unknown_0408;
  /* 040C */ unsigned char unknown_040C[0x04];

  void SetAnimation(unsigned animation, int motion, int repeat) override;

  virtual void Slot08() = 0;
  virtual void Slot09(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh, int twelfth, int thirteenth, int fourteenth, int fifteenth, int sixteenth, int seventeenth) = 0;
  virtual void Slot10(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth) = 0;
  virtual void Slot11(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth) = 0;
  virtual void Slot12(int first, int second, int third, int fourth, int fifth) = 0;
  virtual void Slot13() = 0;
  virtual void Slot14() = 0;
  virtual void Slot15(int first, int second) = 0;
  virtual void Slot16() = 0;
  virtual void Slot17() = 0;
  virtual void Slot18(int first, int second, int third);
  virtual void Slot19(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) = 0;
  virtual void Slot20(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) = 0;
  virtual void Slot21(int value);
  virtual void Slot22(int value);
  virtual void Slot23(int first, int second);
  virtual void Slot24(int first, int second) = 0;
  virtual void Slot25(int value);
  virtual void Slot26(int value) = 0;
  virtual void Slot27(int value) = 0;
  virtual void Slot28(int value) = 0;
  virtual void Slot29(int value) = 0;
  virtual void Slot30();
  virtual void Slot31() = 0;
  virtual void Slot32() = 0;
  virtual void Slot33(int value) = 0;
  virtual void Slot34(int value) = 0;
  virtual void Slot35() = 0;
  virtual void Slot36() = 0;
  virtual int Slot37();
  virtual void Slot38();
  virtual void Slot39(int value);

  void SetUnknown0305(unsigned char value, int extra);
  void SetUnknown0318(const float* value, int index);
  void SetUnknown0330(const float* value, int index);
  void RaiseUnknown03C8(int value);
  void SetUnknown02E4(int value);
  void SetUnknown02C8(int value);
  void SetUnknown0260(int value);
  void ApplyPendingUnknown0408();
  void ProcessBodyState();
  int GetShield();
  bool FindRoute(int startX, int startY, int fractionX, int fractionY, int destX, int destY, int speed);
};

extern const int g_maxMoveSpeed;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGameActor) == 0x410);
static_assert(offsetof(CGameActor, route) == 0x220);
static_assert(offsetof(CGameActor, unknown_0238) == 0x238);
static_assert(offsetof(CGameActor, routeStartTick) == 0x254);
static_assert(offsetof(CGameActor, unknown_0260) == 0x260);
static_assert(offsetof(CGameActor, bodyState) == 0x2BC);
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
static_assert(offsetof(CGameActor, unknown_025C) == 0x25C);
static_assert(offsetof(CGameActor, unknown_0408) == 0x408);
#endif

#endif
