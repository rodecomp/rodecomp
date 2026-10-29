#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GNDRISER_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GNDRISER_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>

class CGndRiser : public EffectActor {
public:
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  /* 0158 */ int lifetime;
  /* 015C */ int screenX;
  /* 0160 */ int screenY;
  /* 0164 */ int unknown_0164;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGndRiser) == 0x168);
static_assert(offsetof(CGndRiser, lifetime) == 0x158);
static_assert(offsetof(CGndRiser, screenX) == 0x15C);
static_assert(offsetof(CGndRiser, screenY) == 0x160);
#endif

#endif
