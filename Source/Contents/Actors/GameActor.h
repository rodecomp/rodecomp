#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H

#include "Contents/Effects/EffectActor.h"
#include "Contents/MovementRouteInterpolation.h"

#include <cstddef>
#include <cstdint>
#include <string>

enum {
  BODYSTATE_NORMAL,
  BODYSTATE_STONECURSE,
  BODYSTATE_FREEZING,
  BODYSTATE_STUN,
  BODYSTATE_SLEEP,
  BODYSTATE_UNDEAD,
  BODYSTATE_STONECURSE_ING,
};

class CGameActor : public EffectActor {
public:
  ~CGameActor() override;

  /* 0158 */ unsigned char unknown_0158[0xC8];
  /* 0220 */ MovementRouteInterpolation route;
  /* 0238 */ unsigned char unknown_0238;
  /* 0239 */ unsigned char unknown_0239[0x1B];
  /* 0254 */ std::uint32_t routeStartTick;
  /* 0258 */ unsigned char unknown_0258[0x04];
  /* 025C */ int unknown_025C;
  /* 0260 */ int unknown_0260;
  /* 0264 */ unsigned char unknown_0264[0x58];
  /* 02BC */ int bodyState;
  /* 02C0 */ std::uint32_t stateFlags;
  /* 02C4 */ unsigned char unknown_02C4[0x04];
  /* 02C8 */ int unknown_02C8;
  /* 02CC */ unsigned char unknown_02CC;
  /* 02CD */ unsigned char unknown_02CD[0x17];
  /* 02E4 */ int unknown_02E4;
  /* 02E8 */ int guildId;
  /* 02EC */ unsigned char unknown_02EC[0x19];
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
  /* 037C */ int unknown_037C[15];
  /* 03B8 */ int unknown_03B8[3];
  /* 03C4 */ int unknown_03C4;
  /* 03C8 */ int unknown_03C8;
  /* 03CC */ int charmType;
  /* 03D0 */ unsigned char unknown_03D0[0x0C];
  /* 03DC */ int unknown_03DC;
  /* 03E0 */ unsigned char unknown_03E0[0x24];
  /* 0404 */ int unknown_0404;
  /* 0408 */ int unknown_0408;
  /* 040C */ unsigned char unknown_040C[0x04];

  void SetAction(unsigned animation, int motion, int repeat) override;

  virtual void Slot08() = 0;
  virtual void Slot09(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh, int twelfth, int thirteenth, int fourteenth, int fifteenth, int sixteenth, int seventeenth) = 0;
  virtual void Slot10(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth) = 0;
  virtual void Slot11(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth) = 0;
  virtual void Slot12(int first, int second, int third, int fourth, int fifth) = 0;
  virtual std::uint32_t Slot13();
  virtual void Slot14() = 0;
  virtual void Slot15(int first, int second) = 0;
  virtual void Slot16() = 0;
  virtual void Slot17();
  virtual void Slot18(int first, int second, int third);
  virtual void Slot19(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh);
  virtual void Slot20(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh);
  virtual void Slot21(int value);
  virtual void Slot22(int value);
  virtual void Slot23(int first, int second);
  virtual void Slot24(int first, int second) = 0;
  virtual void Slot25(int value);
  virtual void Slot26(int value);
  virtual void Slot27(int value);
  virtual void Slot28(int value) = 0;
  virtual void Slot29(int value) = 0;
  virtual void Slot30();
  virtual void Slot31();
  virtual void Slot32() = 0;
  virtual void Slot33(int value) = 0;
  virtual void Slot34(int value);
  virtual std::int64_t Slot35();
  virtual void Slot36() = 0;
  virtual int Slot37();
  virtual void Slot38();
  virtual void Slot39(int value);
  virtual void Slot40(int value);
  virtual void Slot41(int first, int second);
  virtual int Slot42();
  virtual void Slot43(int first, int second);
  virtual void Slot44(int first, int second);
  virtual void Slot45();
  virtual int Slot46();
  virtual void Slot47(unsigned char value);
  virtual void Slot48(int value);
  virtual void Slot49(int guildId, int version);
  virtual int Slot50();
  virtual float* Slot51(int index);
  virtual void SetUnknown0318(const float* value, int index);
  virtual float* Slot53(int index);
  virtual void SetUnknown0330(const float* value, int index);
  virtual int Slot55();
  virtual int Slot56();
  virtual void Slot57(int value);
  virtual bool Slot58();
  virtual void Slot59(bool value);
  virtual void Slot60();
  virtual float* GetExtent(float* extent) const;
  virtual void Slot62(int value);

  void SetUnknown0305(unsigned char value, int extra);
  void RaiseUnknown03C8(int value);
  void SetUnknown02E4(int value);
  void SetUnknown02C8(int value);
  void SetUnknown0260(int value);
  void ApplyPendingUnknown0408();
  void ProcessBodyState();
  int GetShield();
  int GetEffectiveJob(int job);
  int GetUnknown0404Job() const;
  int ActionBase(int state);
  void SetUnknown03C4(int value);
  void SetUnknown0404(int value);
  void SetUnknown008C(const std::uint32_t* value);
  bool FindRoute(int startX, int startY, int fractionX, int fractionY, int destX, int destY, int speed);
  std::string GetShapeShiftName();
  int MountType();
  bool ApplyEffectState(int effectId, bool on);
  void SetHatEffect(int effectId, bool on);
  void SetEffectState(int effectId, bool on);
  void SetEffectInstance(int effectId, int instance, bool on);
  void ClearUnknown037C(int value);
  int WeaponActionBase(int weaponType);
  int GetUnknown0404();
  int MountHeightOffset();
};

int ShapeShiftJob(int form, int job);

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
static_assert(offsetof(CGameActor, charmType) == 0x3CC);
static_assert(offsetof(CGameActor, unknown_025C) == 0x25C);
static_assert(offsetof(CGameActor, unknown_0408) == 0x408);
#endif

#endif
