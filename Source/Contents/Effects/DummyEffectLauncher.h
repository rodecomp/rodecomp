#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_DUMMYEFFECTLAUNCHER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_DUMMYEFFECTLAUNCHER_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>

class CDummyEffectLauncher : public EffectActor {
public:
  bool OnProcess() override;

  /* 0158 */ int lifeFrames;
  /* 015C */ int unknown_015C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CDummyEffectLauncher) == 0x160);
static_assert(offsetof(CDummyEffectLauncher, lifeFrames) == 0x158);
#endif

#endif
