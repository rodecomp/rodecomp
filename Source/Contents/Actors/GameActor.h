#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H

#include "Contents/Effects/EffectActor.h"
#include "Contents/MovementRouteInterpolation.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <set>
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

struct ActorHitInfo {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int damage;
  /* 0008 */ int extraDamage;
  /* 000C */ int type;
  /* 0010 */ std::uint32_t targetAid;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int hitCount;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int attackSpeed;
  /* 0028 */ int motionSpeed;
  /* 002C */ unsigned char unknown_002C;
};

struct ActorDamageMessage {
  /* 0000 */ std::uint32_t targetAid;
  /* 0004 */ int action;
  /* 0008 */ int damage;
  /* 000C */ std::uint32_t time;
  /* 0010 */ std::uint32_t attackerAid;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
  /* 001C */ int unknown_001C;
  /* 0020 */ int effectId;
  /* 0024 */ int unknown_0024;
  /* 0028 */ float motionSpeed;
  /* 002C */ unsigned char unknown_002C;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int unknown_0034;
};

class CGameActor : public EffectActor {
public:
  CGameActor();
  ~CGameActor() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;

  /* 0158 */ int unknown_0158[6];
  /* 0170 */ unsigned char unknown_0170[0x08];
  /* 0178 */ int unknown_0178;
  /* 017C */ int unknown_017C;
  /* 0180 */ int unknown_0180;
  /* 0184 */ int unknown_0184;
  /* 0188 */ int unknown_0188;
  /* 018C */ int unknown_018C;
  /* 0190 */ unsigned char unknown_0190[0x80];
  /* 0210 */ std::uint16_t unknown_0210;
  /* 0212 */ unsigned char unknown_0212[0x02];
  /* 0214 */ vector3d unknown_0214;
  /* 0220 */ MovementRouteInterpolation route;
  /* 0238 */ unsigned char unknown_0238;
  /* 0239 */ unsigned char unknown_0239[0x03];
  /* 023C */ int unknown_023C;
  /* 0240 */ float unknown_0240;
  /* 0244 */ std::uint32_t unknown_0244;
  /* 0248 */ std::uint32_t unknown_0248;
  /* 024C */ std::uint32_t hitTargetAid;
  /* 0250 */ int unknown_0250;
  /* 0254 */ std::uint32_t routeStartTick;
  /* 0258 */ std::uint32_t unknown_0258;
  /* 025C */ int unknown_025C;
  /* 0260 */ int unknown_0260;
  /* 0264 */ int unknown_0264[5];
  /* 0278 */ int unknown_0278;
  /* 027C */ int unknown_027C;
  /* 0280 */ std::uint32_t unknown_0280;
  /* 0284 */ std::uint32_t unknown_0284;
  /* 0288 */ int unknown_0288;
  /* 028C */ int unknown_028C;
  /* 0290 */ int unknown_0290;
  /* 0294 */ int unknown_0294;
  /* 0298 */ int unknown_0298;
  /* 029C */ unsigned char unknown_029C[0x04];
  /* 02A0 */ std::list<ActorDamageMessage> unknown_02A0;
  /* 02A8 */ std::list<ActorDamageMessage> unknown_02A8;
  /* 02B0 */ int unknown_02B0;
  /* 02B4 */ int unknown_02B4;
  /* 02B8 */ unsigned char unknown_02B8;
  /* 02B9 */ unsigned char unknown_02B9[0x03];
  /* 02BC */ int bodyState;
  /* 02C0 */ std::uint32_t stateFlags;
  /* 02C4 */ int unknown_02C4;
  /* 02C8 */ int unknown_02C8;
  /* 02CC */ unsigned char unknown_02CC;
  /* 02CD */ unsigned char unknown_02CD[0x0B];
  /* 02D8 */ int unknown_02D8;
  /* 02DC */ int unknown_02DC;
  /* 02E0 */ int unknown_02E0;
  /* 02E4 */ int unknown_02E4;
  /* 02E8 */ int guildId;
  /* 02EC */ int unknown_02EC;
  /* 02F0 */ int unknown_02F0;
  /* 02F4 */ int unknown_02F4;
  /* 02F8 */ int unknown_02F8;
  /* 02FC */ int unknown_02FC;
  /* 0300 */ int unknown_0300;
  /* 0304 */ unsigned char unknown_0304;
  /* 0305 */ unsigned char unknown_0305;
  /* 0306 */ unsigned char unknown_0306[2];
  /* 0308 */ int unknown_0308;
  /* 030C */ int unknown_030C;
  /* 0310 */ int unknown_0310;
  /* 0314 */ unsigned char unknown_0314;
  /* 0315 */ unsigned char unknown_0315[3];
  /* 0318 */ float unknown_0318[6];
  /* 0330 */ float unknown_0330[6];
  /* 0348 */ float unknown_0348[6];
  /* 0360 */ float unknown_0360[6];
  /* 0378 */ float unknown_0378;
  /* 037C */ int unknown_037C[13];
  /* 03B0 */ std::list<CGameObject*> unknown_03B0;
  /* 03B8 */ int unknown_03B8[3];
  /* 03C4 */ int unknown_03C4;
  /* 03C8 */ int unknown_03C8;
  /* 03CC */ int charmType;
  /* 03D0 */ int unknown_03D0;
  /* 03D4 */ int unknown_03D4;
  /* 03D8 */ int unknown_03D8;
  /* 03DC */ int unknown_03DC;
  /* 03E0 */ std::uint16_t unknown_03E0;
  /* 03E2 */ unsigned char unknown_03E2[0x06];
  /* 03E8 */ int unknown_03E8;
  /* 03EC */ int unknown_03EC;
  /* 03F0 */ vector3d unknown_03F0;
  /* 03FC */ std::set<int> unknown_03FC;
  /* 0404 */ int unknown_0404;
  /* 0408 */ int unknown_0408;
  /* 040C */ unsigned char unknown_040C[0x04];

  int Get8Dir(float angle) override;
  void SetAction(unsigned animation, int motion, int repeat) override;

  virtual bool Slot09(
    int layer, int index, float distanceScale, const SpriteFrameCell& cell, float x, float y, const float* position, float scale,
    float* bottomRhw, float* topRhw, const SpriteRleImage* image, SpriteQuad* quad, int* box, std::uint32_t* color, int offsetX, int offsetY,
    int flags
  ) = 0;
  virtual void Slot10(
    const SpriteFrameCell* cell, const int* box, float shiftX, float bottomRhw, float topRhw, const std::uint32_t* palette,
    const SpriteRleImage* image, float scale, short hover
  ) = 0;
  virtual void Slot11(
    SpriteQuad* quad, const SpriteFrameCell* cell, const int* box, float shiftX, float bottomRhw, float topRhw,
    const std::uint32_t* palette, const SpriteRleImage* image, int blend, short hover
  ) = 0;
  virtual void Slot12(SpriteQuad* quad, const int* box, float shiftX, int first, int second) = 0;
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
  void sub_CC40D0();
  void sub_CAA4D0();
  void SetActorJob(int job);
  void RenderMercenary(const float* matrix, int job);
  void ProcessBodyState();
  int GetShield();
  int GetEffectiveJob(int job);
  void ShowPartyMemberEffect(std::uint32_t accountId);
  int GetUnknown0404Job() const;
  void SetUnknown03B8(int value, int slot);
  void SetTetraBall(int index, int on, int value);
  bool IsSkillWithoutAction(int skillId);
  void SetCharm(int type, int count);
  int ActionBase(int state);
  void SetUnknown03C4(int value);
  void SetUnknown0404(int value);
  void SetUnknown008C(const std::uint32_t* value);
  bool FindRoute(int startX, int startY, int fractionX, int fractionY, int destX, int destY, int speed);
  std::string GetShapeShiftName();
  int MountType();
  bool IsShapeShifted();
  bool HasShapeShift();
  bool ApplyEffectState(int effectId, bool on);
  void SetHatEffect(int effectId, bool on);
  void SetEffectState(int effectId, bool on);
  void SetEffectInstance(int effectId, int instance, bool on);
  void ResetJobEffects();
  void ClearUnknown037C(int value);
  int WeaponActionBase(int weaponType);
  int GetUnknown0404();
  int MountHeightOffset();
  float sub_CAD4B0();
  void ResetState();
  void UpdateGaugePositions();
  void UpdateHpGauge();
  void UpdateMotionFrame();
  void ProcessHitMessage(CGameObject* sender, const ActorHitInfo* info, int unused1, int unused2);
  int GetEffectCounter(int effectId);
  float GetNameOffset();
  float GetNameHeight();
  void SetStackedEffect(int effectId, bool on);
  CSprRes* GetSprite();
  float GetElapsedFrames();
  void ResetUnknown03B8();
  bool IsMonsterClass(int job);
  bool HasPendingDamage();
  bool UpdateUnknown03E8(std::int64_t value);
  int IsPlayerClass(int job);
};

int ShapeShiftJob(int form, int job);
int __stdcall MountedJob(int job, std::uint32_t flags);

extern const int g_maxMoveSpeed;
extern int g_multiHitInterval;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGameActor) == 0x410);
static_assert(offsetof(CGameActor, route) == 0x220);
static_assert(offsetof(CGameActor, unknown_0178) == 0x178);
static_assert(offsetof(CGameActor, hitTargetAid) == 0x24C);
static_assert(sizeof(ActorDamageMessage) == 0x38);
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
static_assert(offsetof(CGameActor, unknown_0158) == 0x158);
static_assert(offsetof(CGameActor, unknown_0190) == 0x190);
static_assert(offsetof(CGameActor, unknown_0210) == 0x210);
static_assert(offsetof(CGameActor, unknown_0214) == 0x214);
static_assert(offsetof(CGameActor, unknown_023C) == 0x23C);
static_assert(offsetof(CGameActor, unknown_0258) == 0x258);
static_assert(offsetof(CGameActor, unknown_0264) == 0x264);
static_assert(offsetof(CGameActor, unknown_0290) == 0x290);
static_assert(offsetof(CGameActor, unknown_0298) == 0x298);
static_assert(offsetof(CGameActor, unknown_02A0) == 0x2A0);
static_assert(offsetof(CGameActor, unknown_02A8) == 0x2A8);
static_assert(offsetof(CGameActor, unknown_02B8) == 0x2B8);
static_assert(offsetof(CGameActor, unknown_02D8) == 0x2D8);
static_assert(offsetof(CGameActor, unknown_02EC) == 0x2EC);
static_assert(offsetof(CGameActor, unknown_0304) == 0x304);
static_assert(offsetof(CGameActor, unknown_030C) == 0x30C);
static_assert(offsetof(CGameActor, unknown_037C) == 0x37C);
static_assert(offsetof(CGameActor, unknown_03B0) == 0x3B0);
static_assert(offsetof(CGameActor, unknown_03B8) == 0x3B8);
static_assert(offsetof(CGameActor, unknown_03D0) == 0x3D0);
static_assert(offsetof(CGameActor, unknown_03E0) == 0x3E0);
static_assert(offsetof(CGameActor, unknown_03E8) == 0x3E8);
static_assert(offsetof(CGameActor, unknown_03F0) == 0x3F0);
static_assert(offsetof(CGameActor, unknown_03FC) == 0x3FC);
static_assert(offsetof(CGameActor, unknown_0404) == 0x404);
#endif

#endif
