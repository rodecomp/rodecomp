#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_PC_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_PC_H

#include "Contents/Actors/GameActor.h"

#include <cstddef>
#include <cstdint>
#include <string>

class C3dGrannyModelRes;
struct granny_control;
struct granny_model_instance;

class CPc : public CGameActor {
public:
  ~CPc() override;
  void SendObjectMessage(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;

  void Slot34(int value) override;
  std::int64_t Slot35() override;
  void Slot39(int value) override;
  void Slot40(int value) override;
  void Slot41(int first, int second) override;
  int Slot42() override;
  int Slot46() override;
  void Slot47(unsigned char value) override;
  int Slot56() override;
  bool Slot58() override;
  void Slot60() override;

  float* GetExtent(float* extent) const override;

  /* 0410 */ unsigned char unknown_0410[8];
  /* 0418 */ std::string unknown_0418;
  /* 0430 */ int unknown_0430;
  /* 0434 */ int unknown_0434;
  /* 0438 */ unsigned char unknown_0438[0x0C];
  /* 0444 */ int unknown_0444;
  /* 0448 */ int weapon;
  /* 044C */ int unknown_044C;
  /* 0450 */ unsigned char unknown_0450[0x08];
  /* 0458 */ int unknown_0458;
  /* 045C */ int unknown_045C;
  /* 0460 */ unsigned char unknown_0460[0x6C];
  /* 04CC */ int unknown_04CC;
  /* 04D0 */ int unknown_04D0;
  /* 04D4 */ unsigned char unknown_04D4[0x08];
  /* 04DC */ int unknown_04DC;
  /* 04E0 */ bool unknown_04E0;
  /* 04E1 */ unsigned char unknown_04E1[0x03];
  /* 04E4 */ float unknown_04E4;
  /* 04E8 */ float unknown_04E8;
  /* 04EC */ float unknown_04EC;
  /* 04F0 */ float unknown_04F0;
  /* 04F4 */ int unknown_04F4;
  /* 04F8 */ float unknown_04F8;
  /* 04FC */ int unknown_04FC;
  /* 0500 */ unsigned char unknown_0500[0x0C];
  /* 050C */ int unknown_050C;
};

class CBlurPC : public CPc {
public:
  ~CBlurPC() override;
  bool Update() override;
  void SendObjectMessage(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Draw(int argument) override;
};

class CHalluPC : public CPc {
public:
  ~CHalluPC() override;
  bool Update() override;
  void SendObjectMessage(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Draw(int argument) override;

  /* 0510 */ int unknown_0510;
  /* 0514 */ int unknown_0514;
};

class CHalluPC2 : public CPc {
public:
  ~CHalluPC2() override;
  bool Update() override;
  void SendObjectMessage(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Draw(int argument) override;

  /* 0510 */ int unknown_0510;
  /* 0514 */ int unknown_0514;
};

class CPlayer : public CPc {
public:
  static int AngleToDirection(float angle);

  bool IsSpecialState(int state) const;
};

class CGrannyPc : public CGameActor {
public:
  void SetAnimation(unsigned animation, int motion, int repeat) override;
  void Slot20(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) override;
  void Slot21(int value) override;
  void Slot22(int value) override;
  void Slot23(int first, int second) override;
  void Slot24(int first, int second) override;
  void Slot25(int value) override;
  void Slot26(int value) override;
  void Slot27(int value) override;
  void Slot39(int value) override;
  void Slot40(int value) override;
  void Slot41(int first, int second) override;

  void PlayMotion(int repeat);

  /* 0410 */ std::string unknown_0410;
  /* 0428 */ int unknown_0428;
  /* 042C */ int unknown_042C;
  /* 0430 */ unsigned char unknown_0430[4];
  /* 0434 */ int unknown_0434;
  /* 0438 */ unsigned char unknown_0438[8];
  /* 0440 */ int unknown_0440;
  /* 0444 */ int unknown_0444;
  /* 0448 */ int unknown_0448;
  /* 044C */ int unknown_044C;
  /* 0450 */ int unknown_0450;
  /* 0454 */ int unknown_0454;
  /* 0458 */ unsigned char unknown_0458[8];
  /* 0460 */ std::string unknown_0460;
  /* 0478 */ unsigned char unknown_0478[0x0C];
  /* 0484 */ C3dGrannyModelRes* model;
  /* 0488 */ unsigned char unknown_0488[0x28];
  /* 04B0 */ granny_model_instance* instance;
  /* 04B4 */ unsigned char unknown_04B4[0xA70];
  /* 0F24 */ int unknown_0F24;
  /* 0F28 */ unsigned char unknown_0F28[0x14];
  /* 0F3C */ int motion;
  /* 0F40 */ unsigned char unknown_0F40[0x110];
  /* 1050 */ granny_control* controls[20];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CPc, unknown_0430) == 0x430);
static_assert(offsetof(CPc, unknown_0444) == 0x444);
static_assert(offsetof(CPc, unknown_0458) == 0x458);
static_assert(offsetof(CGrannyPc, unknown_0428) == 0x428);
static_assert(offsetof(CGrannyPc, unknown_0440) == 0x440);
static_assert(offsetof(CGrannyPc, unknown_0460) == 0x460);
static_assert(offsetof(CGrannyPc, model) == 0x484);
static_assert(offsetof(CGrannyPc, instance) == 0x4B0);
static_assert(offsetof(CGrannyPc, unknown_0F24) == 0xF24);
static_assert(offsetof(CGrannyPc, motion) == 0xF3C);
static_assert(offsetof(CGrannyPc, controls) == 0x1050);
static_assert(sizeof(CBlurPC) == 0x510);
static_assert(sizeof(CHalluPC) == 0x518);
static_assert(sizeof(CHalluPC2) == 0x518);
static_assert(offsetof(CPc, unknown_0418) == 0x418);
static_assert(offsetof(CPc, unknown_04E4) == 0x4E4);
static_assert(offsetof(CPc, unknown_04F0) == 0x4F0);
static_assert(offsetof(CPc, unknown_04F8) == 0x4F8);
static_assert(offsetof(CPc, unknown_04FC) == 0x4FC);
static_assert(offsetof(CPc, unknown_050C) == 0x50C);
static_assert(offsetof(CHalluPC, unknown_0510) == 0x510);
static_assert(offsetof(CHalluPC2, unknown_0510) == 0x510);
static_assert(offsetof(CPc, weapon) == 0x448);
static_assert(offsetof(CPc, unknown_044C) == 0x44C);
static_assert(offsetof(CPc, unknown_04CC) == 0x4CC);
static_assert(offsetof(CPc, unknown_04DC) == 0x4DC);
static_assert(offsetof(CPc, unknown_04E0) == 0x4E0);
#endif

#endif
