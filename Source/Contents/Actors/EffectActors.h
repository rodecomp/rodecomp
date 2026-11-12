#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_EFFECTACTORS_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_EFFECTACTORS_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>
#include <cstdint>

class CBlowEffect : public CRenderObject {
public:
  CBlowEffect();
  ~CBlowEffect() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
  ) override;
};

class CCorpse : public CRenderObject {
public:
  CCorpse();
  ~CCorpse() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
  ) override;
  void Render(int argument) override;

  /* 0138 */ float spriteOffset;
  /* 013C */ std::uint32_t padding_013C;
};

class CMasterChaser : public CRenderObject {
public:
  CMasterChaser();
  ~CMasterChaser() override;

  void ClearChaseState();

  /* 0138 */ std::uint32_t chaseState;
  /* 013C */ std::uint32_t padding_013C;
};

class CNumEffect : public CRenderObject {
public:
  CNumEffect();
  ~CNumEffect() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
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
