#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECT_H

#include "Contents/Effects/RagEffectState.h"
#include "Contents/Actors/RenderObject.h"

#include <cstddef>

struct CRagEffect : CRenderObject {
  CRagEffect();
  ~CRagEffect() override;

  /* 0138 */ RagEffectState state;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRagEffect) == 0x11CA8);
static_assert(offsetof(CRagEffect, state) == 0x138);
#endif

#endif
