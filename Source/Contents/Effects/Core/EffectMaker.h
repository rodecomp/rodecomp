#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_EFFECTMAKER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_EFFECTMAKER_H

#include "Contents/Effects/Core/EffectActor.h"

#include <cstddef>

struct CEffectMaker : EffectActor {
  CEffectMaker();
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;
  void Render(int argument) override;

  void Reset();

  /* 0158 */ unsigned elapsedFrames;
  /* 015C */ unsigned spawnInterval;
  /* 0160 */ int effectId;
  /* 0164 */ float parameter1;
  /* 0168 */ float parameter2;
  /* 016C */ float parameter3;
  /* 0170 */ float parameter4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEffectMaker, elapsedFrames) == 0x158);
static_assert(offsetof(CEffectMaker, parameter4) == 0x170);
#endif

#endif
