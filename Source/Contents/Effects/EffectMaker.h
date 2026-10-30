#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTMAKER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTMAKER_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>

struct CEffectMaker : EffectActor {
  CEffectMaker();
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  void Render(int argument) override;

  void Reset();

  /* 0158 */ unsigned unknown_0158;
  /* 015C */ unsigned unknown_015C;
  /* 0160 */ int unknown_0160;
  /* 0164 */ float unknown_0164;
  /* 0168 */ float unknown_0168;
  /* 016C */ float unknown_016C;
  /* 0170 */ float unknown_0170;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEffectMaker, unknown_0158) == 0x158);
static_assert(offsetof(CEffectMaker, unknown_0170) == 0x170);
#endif

#endif
