#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SOULUNITYEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SOULUNITYEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <list>

class CParticle;

class CSoulUnityEffect : public CEffect {
public:
  CSoulUnityEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ std::list<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSoulUnityEffect) == 0xD4);
static_assert(offsetof(CSoulUnityEffect, particles) == 0xCC);
#endif

#endif
