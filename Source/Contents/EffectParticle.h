#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTPARTICLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTPARTICLE_H

#include "Contents/EffectPrimitiveState.h"
#include "Contents/RenderObject.h"

#include <list>

struct JobNames;
class ResourceMgr;
struct RagEffectState;

// Recovered CEffectPrim lifetime and state over the common render base.
struct EffectParticle : CRenderObject {
  EffectParticle(ResourceMgr& resources, JobNames& names);

  EffectPrimitiveState state;
};

// The active mode owns the returned object; the parent list borrows its state.
EffectParticle* CreateEffectParticle(
  RagEffectState& parent,
  const RenderObjectState& parentRender,
  int type,
  float x,
  float y,
  float z,
  ResourceMgr& resources,
  JobNames& names,
  std::list<CRenderObject*>& worldObjects
);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectParticle) == 0xFA18);
#endif

#endif
