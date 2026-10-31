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
  void Slot16() override;
  void Slot17() override;
  void Slot18(int job, int value, int unused) override;
  void Slot21(int value) override;
  void Slot22(int value) override;
  void Slot23(int first, int second) override;
  void Slot25(int value) override;
  void Slot30() override;

  void UpdateBubblePositions();
  void UpdateMinimapMarker();

  /* 0410 */ unsigned char unknown_0410[0x10];
  /* 0420 */ int unknown_0420;
  /* 0424 */ std::uint32_t unknown_0424;
  /* 0428 */ int unknown_0428;
  /* 042C */ UIWindow* unknown_042C;
  /* 0430 */ UIWindow* unknown_0430;
  /* 0434 */ CGameObject* unknown_0434;
  /* 0438 */ int unknown_0438;
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
  void Slot21(int value) override;

  /* 0440 */ CGameObject* unknown_0440;
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

  /* 0440 */ float unknown_0440;
  /* 0444 */ int unknown_0444;
  /* 0448 */ int unknown_0448;
  /* 044C */ int unknown_044C;
  /* 0450 */ int unknown_0450;
  /* 0454 */ int unknown_0454;
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

  /* 0440 */ std::uint32_t unknown_0440;
  /* 0444 */ int unknown_0444;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CNpc) == 0x440);
static_assert(sizeof(CCharacterNpc) == 0x448);
static_assert(sizeof(CHalluNPC) == 0x458);
static_assert(sizeof(CHalluPlant) == 0x448);
static_assert(offsetof(CNpc, unknown_042C) == 0x42C);
static_assert(offsetof(CNpc, unknown_0424) == 0x424);
static_assert(offsetof(CNpc, unknown_0434) == 0x434);
static_assert(offsetof(CNpc, unknown_0438) == 0x438);
static_assert(offsetof(CCharacterNpc, unknown_0440) == 0x440);
static_assert(offsetof(CHalluNPC, unknown_0444) == 0x444);
static_assert(offsetof(CHalluNPC, unknown_0454) == 0x454);
static_assert(offsetof(CHalluPlant, unknown_0440) == 0x440);
#endif

#endif
