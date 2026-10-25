#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPARTICLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPARTICLE_H

#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObject.h"

#include <list>

struct JobNames;
class ResourceMgr;
struct RagEffectState;

struct EffectParticle : CRenderObject {
  EffectParticle(ResourceMgr& resources, JobNames& names);

  EffectPrimitiveState state;
};

EffectParticle* CreateEffectParticle(
  RagEffectState& parent, const RenderObjectState& parentRender, int type, float x, float y, float z, ResourceMgr& resources, JobNames& names,
  std::list<CRenderObject*>& worldObjects
);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectParticle) == 0xFA18);
#endif

#endif
