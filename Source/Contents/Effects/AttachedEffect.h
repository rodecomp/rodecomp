#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ATTACHEDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ATTACHEDEFFECT_H

#include "Contents/Effects/RagEffectState.h"
#include "Contents/Actors/RenderObject.h"

#include <cstddef>

struct AttachedEffect : CRenderObject {
  AttachedEffect(ResourceMgr& resources, JobNames& names);
  ~AttachedEffect() override;

  /* 0138 */ RagEffectState state;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AttachedEffect) == 0x11CA8);
static_assert(offsetof(AttachedEffect, state) == 0x138);
#endif

#endif
