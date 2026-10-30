#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MSGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MSGEFFECT_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>

class CMsgEffect : public EffectActor {
public:
  static bool UsesMsgEffect(int type);

  ~CMsgEffect() override;

  /* 0158 */ int unknown_0158;
  /* 015C */ CGameObject* m_owner;
  /* 0160 */ unsigned char unknown_0160[0x58];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMsgEffect) == 0x1B8);
static_assert(offsetof(CMsgEffect, m_owner) == 0x15C);
#endif

#endif
