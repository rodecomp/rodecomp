#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SOULUNITYEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_SOULUNITYEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <list>

class CParticle;

class CSoulUnityEffect : public CEffect {
public:
  CSoulUnityEffect();
  ~CSoulUnityEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::list<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSoulUnityEffect) == 0xD4);
static_assert(offsetof(CSoulUnityEffect, particles) == 0xCC);
#endif

#endif
