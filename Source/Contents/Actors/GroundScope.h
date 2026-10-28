#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GROUNDSCOPE_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GROUNDSCOPE_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>

class CGroundScope : public EffectActor {
public:
  bool Update() override;

  /* 0158 */ unsigned char unknown_0158[0xC];
  /* 0164 */ unsigned long m_lastingTime;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CGroundScope, m_lastingTime) == 0x164);
#endif

#endif
