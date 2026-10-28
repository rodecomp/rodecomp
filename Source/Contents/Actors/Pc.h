#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_PC_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_PC_H

#include "Contents/Actors/GameActor.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CPc : public CGameActor {
public:
  ~CPc() override;
  void SendObjectMessage(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;

  float* GetExtent(float* extent) const;

  /* 0410 */ unsigned char unknown_0410[8];
  /* 0418 */ std::string unknown_0418;
  /* 0430 */ unsigned char unknown_0430[0xB4];
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

#if defined(_WIN32) && !defined(_WIN64)
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
#endif

#endif
