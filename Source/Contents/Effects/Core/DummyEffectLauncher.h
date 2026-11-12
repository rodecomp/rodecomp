#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_DUMMYEFFECTLAUNCHER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_DUMMYEFFECTLAUNCHER_H

#include "Contents/Effects/Core/EffectActor.h"

#include <cstddef>

class CDummyEffectLauncher : public EffectActor {
public:
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;

  /* 0158 */ int lifeFrames;
  /* 015C */ int padding_015C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CDummyEffectLauncher) == 0x160);
static_assert(offsetof(CDummyEffectLauncher, lifeFrames) == 0x158);
#endif

#endif
