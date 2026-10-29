#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_EFFECTACTORS_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_EFFECTACTORS_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>
#include <cstdint>

class CBlowEffect : public CRenderObject {
public:
  ~CBlowEffect() override;
  void SendObjectMessage(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
};

class CCorpse : public CRenderObject {
public:
  CCorpse();
  ~CCorpse() override;
  void SendObjectMessage(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Draw(int argument) override;

  /* 0138 */ float spriteOffset;
  /* 013C */ std::uint32_t unknown_013C;
};

class CMasterChaser : public CRenderObject {
public:
  ~CMasterChaser() override;

  /* 0138 */ std::uint32_t unknown_0138;
  /* 013C */ std::uint32_t unknown_013C;
};

class CNumEffect : public CRenderObject {
public:
  ~CNumEffect() override;
  bool Update() override;
  void SendObjectMessage(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;

  /* 0138 */ float baseHeight;
  /* 013C */ float baseScale;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CBlowEffect) == 0x138);
static_assert(sizeof(CCorpse) == 0x140);
static_assert(sizeof(CMasterChaser) == 0x140);
static_assert(sizeof(CNumEffect) == 0x140);
static_assert(offsetof(CNumEffect, baseHeight) == 0x138);
static_assert(offsetof(CNumEffect, baseScale) == 0x13C);
#endif

#endif
