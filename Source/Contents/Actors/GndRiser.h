#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GNDRISER_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GNDRISER_H

#include "Contents/Effects/Core/EffectActor.h"

#include <cstddef>

class CGndRiser : public EffectActor {
public:
  CGndRiser();
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
  ) override;
  void Render(int argument) override;

  /* 0158 */ int lifetime;
  /* 015C */ int screenX;
  /* 0160 */ int screenY;
  /* 0164 */ int padding_0164;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGndRiser) == 0x168);
static_assert(offsetof(CGndRiser, lifetime) == 0x158);
static_assert(offsetof(CGndRiser, screenX) == 0x15C);
static_assert(offsetof(CGndRiser, screenY) == 0x160);
#endif

#endif
