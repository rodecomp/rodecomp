#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTMAKER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTMAKER_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>

struct CEffectMaker : EffectActor {
  CEffectMaker();
  bool OnProcess() override;

  void Reset();

  /* 0158 */ unsigned unknown_0158;
  /* 015C */ unsigned unknown_015C;
  /* 0160 */ int unknown_0160;
  /* 0164 */ int unknown_0164;
  /* 0168 */ int unknown_0168;
  /* 016C */ int unknown_016C;
  /* 0170 */ int unknown_0170;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEffectMaker, unknown_0158) == 0x158);
static_assert(offsetof(CEffectMaker, unknown_0170) == 0x170);
#endif

#endif
