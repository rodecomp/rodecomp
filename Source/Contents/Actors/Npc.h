#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_NPC_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_NPC_H

#include "Contents/Actors/GameActor.h"

#include <cstddef>
#include <cstdint>

class UIWindow;

class CNpc : public CGameActor {
public:
  CNpc();
  ~CNpc() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;
  void SetState(int state, int unused) override;
  void RegisterPos() override;
  void UnRegisterPos() override;
  void SetSprAct(int job, int value, int unused) override;
  void SetSprJob(int value) override;
  void SetSprHead(int value) override;
  void SetSprWeapon(int first, int second) override;
  void SetSprAccessory(int value) override;
  void SetImfFileName() override;

  void UpdateBubblePositions();
  void UpdateMinimapMarker();

  /* 0410 */ float boundsLeft;
  /* 0414 */ float boundsTop;
  /* 0418 */ float boundsRight;
  /* 041C */ float boundsBottom;
  /* 0420 */ int npcState;
  /* 0424 */ std::uint32_t effectTick;
  /* 0428 */ int appearanceInitialized;
  /* 042C */ UIWindow* playerGaugeWindow;
  /* 0430 */ UIWindow* pcGaugeWindow;
  /* 0434 */ CGameObject* attachedEffect;
  union {
    /* 0438 */ int attachedEffectId;
    /* 0438 */ std::int16_t minimapMarkerCell[2];
  };
  /* 043C */ int unknown_043C;
};

class CCharacterNpc : public CNpc {
public:
  CCharacterNpc();
  ~CCharacterNpc() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;
  void SetSprJob(int value) override;

  /* 0440 */ CGameObject* characterRender;
};

class CHalluNPC : public CNpc {
public:
  explicit CHalluNPC(CGameActor* source);
  ~CHalluNPC() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  /* 0440 */ float hallucinationOffset;
  /* 0444 */ int fadeStep;
  /* 0448 */ int unknown_0448;
  /* 044C */ int unknown_044C;
  /* 0450 */ int unknown_0450;
  /* 0454 */ int processCount;
};

class CHalluPlant : public CNpc {
public:
  CHalluPlant();
  ~CHalluPlant() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  /* 0440 */ std::uint32_t processCount;
  /* 0444 */ int unknown_0444;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CNpc) == 0x440);
static_assert(sizeof(CCharacterNpc) == 0x448);
static_assert(sizeof(CHalluNPC) == 0x458);
static_assert(sizeof(CHalluPlant) == 0x448);
static_assert(offsetof(CNpc, playerGaugeWindow) == 0x42C);
static_assert(offsetof(CNpc, effectTick) == 0x424);
static_assert(offsetof(CNpc, attachedEffect) == 0x434);
static_assert(offsetof(CNpc, attachedEffectId) == 0x438);
static_assert(offsetof(CCharacterNpc, characterRender) == 0x440);
static_assert(offsetof(CHalluNPC, fadeStep) == 0x444);
static_assert(offsetof(CHalluNPC, processCount) == 0x454);
static_assert(offsetof(CHalluPlant, processCount) == 0x440);
#endif

#endif
