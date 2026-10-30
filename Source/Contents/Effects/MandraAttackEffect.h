#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MANDRAATTACKEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MANDRAATTACKEFFECT_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>

class CMandraAttackEffect : public CRenderObject {
public:
  bool OnProcess() override;

  /* 0138 */ float startFrame;
  /* 013C */ int unknown_013C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMandraAttackEffect) == 0x140);
static_assert(offsetof(CMandraAttackEffect, startFrame) == 0x138);
#endif

#endif
