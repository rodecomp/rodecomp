#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GAMEACTOR_H

#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Movement/MovementRouteInterpolation.h"

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

class UIWindow;

struct ActorHitInfo {
  /* 0000 */ int padding_0000;
  /* 0004 */ int damage;
  /* 0008 */ int extraDamage;
  /* 000C */ int type;
  /* 0010 */ std::uint32_t targetAid;
  /* 0014 */ int startTime;
  /* 0018 */ int hitCount;
  /* 001C */ int cellX;
  /* 0020 */ int cellY;
  /* 0024 */ int attackSpeed;
  /* 0028 */ int motionSpeed;
  /* 002C */ unsigned char blueDamage;
};

struct ActorSkillInfo {
  /* 0000 */ int skillId;
  /* 0004 */ int damage;
  /* 0008 */ int type;
  /* 000C */ std::uint32_t targetAid;
  /* 0010 */ int startTime;
  /* 0014 */ int level;
  /* 0018 */ int hitCount;
  /* 001C */ int cellX;
  /* 0020 */ int cellY;
  /* 0024 */ int attackSpeed;
  /* 0028 */ int motionSpeed;
};

struct ActorDamageMessage {
  /* 0000 */ std::uint32_t targetAid;
  /* 0004 */ int action;
  /* 0008 */ int damage;
  /* 000C */ std::uint32_t time;
  /* 0010 */ std::uint32_t attackerAid;
  /* 0014 */ int cellX;
  /* 0018 */ int cellY;
  /* 001C */ int hitEffect;
  /* 0020 */ int effectId;
  /* 0024 */ int monsterValue;
  /* 0028 */ float motionSpeed;
  /* 002C */ unsigned char blueDamage;
  /* 0030 */ int skillId;
  /* 0034 */ int skillTarget;
};

class CMercenaryAI;

class CGameActor : public EffectActor {
public:
  CGameActor();
  ~CGameActor() override;
  bool OnProcess() override = 0;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;

  /* 0158 */ int movementState[6];
  /* 0170 */ int destCellY;
  /* 0174 */ int moveSpeed;
  /* 0178 */ int isCounter;
  /* 017C */ int isTrickDead;
  /* 0180 */ int isPlayHitWave;
  /* 0184 */ int isAsuraAttack;
  /* 0188 */ int sceneNameWindow;
  /* 018C */ int actorTitleWindow;
  /* 0190 */ unsigned char hitWaveName[0x80];
  /* 0210 */ std::uint16_t characterLevel;
  /* 0212 */ unsigned char padding_0212[0x02];
  /* 0214 */ vector3d accel;
  /* 0220 */ MovementRouteInterpolation route;
  /* 0238 */ unsigned char isNeverAnimation;
  /* 0239 */ unsigned char padding_0239[0x03];
  /* 023C */ int pathStartCell;
  /* 0240 */ float dist;
  /* 0244 */ std::uint32_t lastProcessStateTime;
  /* 0248 */ std::uint32_t chatTick;
  /* 024C */ std::uint32_t hitTargetAid;
  /* 0250 */ int isBladeStop;
  /* 0254 */ std::uint32_t routeStartTick;
  /* 0258 */ std::uint32_t lastServerTime;
  /* 025C */ int job;
  /* 0260 */ int sex;
  /* 0264 */ UIWindow* actorWindows[5];
  /* 0278 */ int freezeEndTick;
  /* 027C */ int petEmotionStartTick;
  /* 0280 */ std::uint32_t skillRechargeEndTick;
  /* 0284 */ std::uint32_t skillRechargeStartTick;
  /* 0288 */ int chatWidth;
  /* 028C */ int chatHeight;
  /* 0290 */ int nameWidth;
  /* 0294 */ int xSize;
  /* 0298 */ int ySize;
  /* 029C */ int headType;
  /* 02A0 */ std::list<ActorDamageMessage> willBeAttackList;
  /* 02A8 */ std::list<ActorDamageMessage> willBeAttackedList;
  /* 02B0 */ int willBeDead;
  /* 02B4 */ int hasLevelAura;
  /* 02B8 */ unsigned char levelAuraType;
  /* 02B9 */ unsigned char padding_02B9[0x03];
  /* 02BC */ int bodyState;
  /* 02C0 */ std::uint32_t stateFlags;
  /* 02C4 */ int effectState;
  /* 02C8 */ int healthState;
  /* 02CC */ unsigned char isRiding;
  /* 02CD */ unsigned char padding_02CD[0x03];
  /* 02D0 */ float damageDestX;
  /* 02D4 */ float damageDestZ;
  /* 02D8 */ int effectLaunchCnt;
  /* 02DC */ int vanishTime;
  /* 02E0 */ int actorType;
  /* 02E4 */ int isGroupMemberVisible;
  /* 02E8 */ int guildId;
  /* 02EC */ int emblemVersion;
  /* 02F0 */ int damageNumberCount;
  /* 02F4 */ int lastDamage;
  /* 02F8 */ int hasHp;
  /* 02FC */ int maxHp;
  /* 0300 */ int hpGaugeWindow;
  /* 0304 */ unsigned char hpGaugeFlags;
  /* 0305 */ unsigned char hasSp;
  /* 0306 */ unsigned char padding_0306[2];
  /* 0308 */ int spGaugeValue;
  /* 030C */ int homunAI;
  /* 0310 */ CMercenaryAI* merAI;
  /* 0314 */ unsigned char objectType;
  /* 0315 */ unsigned char padding_0315[3];
  /* 0318 */ float homunMsg[6];
  /* 0330 */ float homunResMsg[6];
  /* 0348 */ float merMsg[6];
  /* 0360 */ float merResMsg[6];
  /* 0378 */ float attackMotion;
  /* 037C */ CGameObject* statusEffects[13];
  /* 03B0 */ std::list<CGameObject*> msgEffectList;
  /* 03B8 */ int transformJobs[3];
  /* 03C4 */ int monsterTransformJob;
  /* 03C8 */ int transformPriority;
  /* 03CC */ int charmType;
  /* 03D0 */ int charmCount;
  /* 03D4 */ int firstFollowerEffect;
  /* 03D8 */ int secondFollowerEffect;
  /* 03DC */ int mountedState;
  union { /* 03E0 */ std::uint16_t actorFlags; struct { unsigned char storedActorEntry; /* 03E1 */ bool modelReady; }; };
  /* 03E2 */ unsigned char padding_03E2[0x06];
  /* 03E8 */ std::int64_t maximumValue;
  /* 03F0 */ vector3d positionDelta;
  /* 03FC */ std::set<int> exclusiveEffects;
  /* 0404 */ int shapeShift;
  /* 0408 */ int savedShapeShift;
  /* 040C */ unsigned char padding_040C[0x04];

  int Get8Dir(float angle) override;
  void SetRenderInfo(const float* bounds, float x, float y) override;
  void SetAction(unsigned animation, int motion, int repeat) override;

  std::uint32_t GetStateFlags() override;
  virtual bool ProcessState();
  virtual void SetState(int first, int second);
  virtual void RegisterPos();
  virtual void UnRegisterPos();
  virtual void SetSprAct(int first, int second, int third);
  virtual void SetSprActFull(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh);
  virtual void SetSprAct2(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh);
  virtual void SetSprJob(int value);
  virtual void SetSprHead(int value);
  virtual void SetSprWeapon(int first, int second);
  virtual void SetSprWeapon2(int first, int second);
  virtual void SetSprAccessory(int value);
  virtual void SetSprAccessory2(int value);
  virtual void SetSprAccessory3(int value);
  virtual void SetSprShield(int value);
  virtual void SetSprShoe(int value) {}
  virtual void SetImfFileName();
  virtual void SetHeadPaletteName();
  virtual void SetBodyPaletteName();
  virtual void SetBodyPalette(int value);
  virtual void SetHeadPalette(int value);
  virtual std::int64_t GetWeapon();
  virtual void ProcessMotionWithDist();
  virtual int GetAttackMotion();
  virtual void MakeCorpse();
  virtual void SetModifyFactorOfmotionSpeed(int value);
  virtual void SetHonor(int value);
  virtual void SetPropensityInfo(int first, int second);
  virtual int GetPkState();
  virtual void SetSprRobe(int first, int second);
  virtual void SetBodyCostume(int first, int second);
  virtual int GetNpcLocationRadius();
  virtual int GetBodyCostume();
  virtual void SetRiding(unsigned char value);
  virtual void SetGdid(int value);
  virtual void SetGuildInfo(int guildId, int version);
  virtual int GetGdid();
  virtual float* GetMsg2AI(int index);
  virtual void SetMsg2AI(const float* value, int index);
  virtual float* GetResMsg2AI(int index);
  virtual void SetResMsg2AI(const float* value, int index);
  virtual int GetMonsterTransformJob();
  virtual int GetMadogearType();
  virtual void SetMadogearType(int value);
  virtual bool IsArrowEffectSuppressed();
  virtual void SetArrowEffectSuppressed(bool value);
  virtual void RefreshAccessory3();
  virtual float* GetExtent(float* extent) const;
  virtual void SetSex(int value);

  void SetSpGauge(unsigned char value, int extra);
  void RaiseTransformPriority(int value);
  void SetGroupMemberVisible(int value);
  void SetHealthState(int value);
  void SetSexBit(int value);
  void ApplySavedShapeShift();
  void ApplyNatureProtection();
  void UpdateMaxLevelAura();
  void SetActorJob(int job);
  void RenderMercenary(const float* matrix, int job);
  void ProcessBodyState();
  int GetEffectiveJob(int job);
  void LaunchAttackProjectile(int target, float scale);
  void SetStatusStates(int bodyState, std::uint32_t stateFlags, int effectState, bool notify);
  void PlayGroundSkillEffect(int sender, const int* skill, int argument3, int argument4);
  void ShowPartyMemberEffect(std::uint32_t accountId);
  int GetShapeShiftJob() const;
  void SetTransformJob(int value, int slot);
  void SetTetraBall(int index, int on, int value);
  bool IsSkillWithoutAction(int skillId);
  bool HasActiveHomunculusAI();
  bool HasActiveMercenaryAI();
  void SetCharm(int type, int count);
  __declspec(noinline) int ActionBase(int state);
  void SetMonsterTransformJob(int value);
  void SetShapeShift(int value);
  void SetMotionTintState(const std::uint32_t* value);
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
  void ClearStatusEffectInstance(CGameObject* value);
  void SpawnDamageDigits(int unused, int damage, std::uint32_t color, int type);
  __declspec(noinline) int WeaponActionBase(int weaponType);
  int GetShapeShift();
  int MountHeightOffset();
  void ResetState();
  void UpdateGaugePositions();
  void UpdateHpGauge();
  void UpdateStateEffects();
  void ReleaseAttachments();
  void ClearSiegeNameWindow();
  void RefreshGuildEmblemWindow();
  void NotifyCellMove(const float* x, const float* z);
  void ProcessViewCheck();
  void ProcessActorEffects();
  void ProcessDamageMessages();
  void UpdateDamageMessages();
  void ProcessHitMessage(CGameObject* sender, const ActorHitInfo* info, int unused1, int unused2);
  void ProcessSkillMessage(CGameObject* sender, const ActorSkillInfo* info, int unused1, int unused2);
  CGameObject* GetEffectCounter(int effectId);
  float GetNameOffset();
  float GetNameHeight();
  void SetStackedEffect(int effectId, bool on);
  CSprRes* GetSprite();
  float GetElapsedFrames();
  int GetGaugeCurrent();
  int GetGaugeMaximum();
  void ResetTransformJobs();
  bool HasPendingDamage();
  bool UpdateMaximumValue(std::int64_t value);
  int IsPlayerClass(int job);
  bool UpdateFadeOut();
};

int ShapeShiftJob(int form, int job);
int __stdcall MountedJob(int job, std::uint32_t flags);
void __stdcall CreateHallucinationPc(int job, int sex, int head, const vector3d* position, int alpha, int weapon, int shield, int bodyPalette);

extern int g_maxMoveSpeed;
extern int g_multiHitInterval;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGameActor) == 0x410);
static_assert(offsetof(CGameActor, route) == 0x220);
static_assert(offsetof(CGameActor, isCounter) == 0x178);
static_assert(offsetof(CGameActor, hitTargetAid) == 0x24C);
static_assert(sizeof(ActorDamageMessage) == 0x38);
static_assert(offsetof(CGameActor, isNeverAnimation) == 0x238);
static_assert(offsetof(CGameActor, routeStartTick) == 0x254);
static_assert(offsetof(CGameActor, sex) == 0x260);
static_assert(offsetof(CGameActor, bodyState) == 0x2BC);
static_assert(offsetof(CGameActor, healthState) == 0x2C8);
static_assert(offsetof(CGameActor, isGroupMemberVisible) == 0x2E4);
static_assert(offsetof(CGameActor, hasSp) == 0x305);
static_assert(offsetof(CGameActor, spGaugeValue) == 0x308);
static_assert(offsetof(CGameActor, objectType) == 0x314);
static_assert(offsetof(CGameActor, homunMsg) == 0x318);
static_assert(offsetof(CGameActor, homunResMsg) == 0x330);
static_assert(offsetof(CGameActor, merMsg) == 0x348);
static_assert(offsetof(CGameActor, merResMsg) == 0x360);
static_assert(offsetof(CGameActor, attackMotion) == 0x378);
static_assert(offsetof(CGameActor, transformPriority) == 0x3C8);
static_assert(offsetof(CGameActor, charmType) == 0x3CC);
static_assert(offsetof(CGameActor, job) == 0x25C);
static_assert(offsetof(CGameActor, savedShapeShift) == 0x408);
static_assert(offsetof(CGameActor, movementState) == 0x158);
static_assert(offsetof(CGameActor, hitWaveName) == 0x190);
static_assert(offsetof(CGameActor, characterLevel) == 0x210);
static_assert(offsetof(CGameActor, accel) == 0x214);
static_assert(offsetof(CGameActor, pathStartCell) == 0x23C);
static_assert(offsetof(CGameActor, lastServerTime) == 0x258);
static_assert(offsetof(CGameActor, actorWindows) == 0x264);
static_assert(offsetof(CGameActor, nameWidth) == 0x290);
static_assert(offsetof(CGameActor, ySize) == 0x298);
static_assert(offsetof(CGameActor, willBeAttackList) == 0x2A0);
static_assert(offsetof(CGameActor, willBeAttackedList) == 0x2A8);
static_assert(offsetof(CGameActor, levelAuraType) == 0x2B8);
static_assert(offsetof(CGameActor, damageDestX) == 0x2D0);
static_assert(offsetof(CGameActor, damageDestZ) == 0x2D4);
static_assert(offsetof(CGameActor, effectLaunchCnt) == 0x2D8);
static_assert(offsetof(CGameActor, emblemVersion) == 0x2EC);
static_assert(offsetof(CGameActor, hpGaugeFlags) == 0x304);
static_assert(offsetof(CGameActor, homunAI) == 0x30C);
static_assert(offsetof(CGameActor, statusEffects) == 0x37C);
static_assert(offsetof(CGameActor, msgEffectList) == 0x3B0);
static_assert(offsetof(CGameActor, transformJobs) == 0x3B8);
static_assert(offsetof(CGameActor, charmCount) == 0x3D0);
static_assert(offsetof(CGameActor, actorFlags) == 0x3E0);
static_assert(offsetof(CGameActor, maximumValue) == 0x3E8);
static_assert(offsetof(CGameActor, positionDelta) == 0x3F0);
static_assert(offsetof(CGameActor, exclusiveEffects) == 0x3FC);
static_assert(offsetof(CGameActor, shapeShift) == 0x404);
#endif

#endif
