#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_DUMMYEFFECTLAUNCHER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_DUMMYEFFECTLAUNCHER_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>

class CDummyEffectLauncher : public EffectActor {
public:
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;

  /* 0158 */ int lifeFrames;
  /* 015C */ int unknown_015C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CDummyEffectLauncher) == 0x160);
static_assert(offsetof(CDummyEffectLauncher, lifeFrames) == 0x158);
#endif

#endif
