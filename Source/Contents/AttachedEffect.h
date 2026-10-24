#ifndef RAGNAROK_SOURCE_CONTENTS_ATTACHEDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_ATTACHEDEFFECT_H

#include "Contents/RagEffectState.h"
#include "Contents/RenderObject.h"

struct AttachedEffect : CRenderObject {
  AttachedEffect(ResourceMgr& resources, JobNames& names);

  RagEffectState state;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AttachedEffect) == 0x11CA8);
#endif

#endif
