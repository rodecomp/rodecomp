#include "Contents/Effects/EffectParticle.h"
#include "Contents/Effects/AttachedEffect.h"

// OFFSET: 0x00BAD940
// FUNCTION: fn_19761
EffectParticle::EffectParticle(ResourceMgr& resources, JobNames& names) : CRenderObject(resources, names), state(this->render) {
}

// OFFSET: 0x00C1DE70
// FUNCTION: fn_20076
EffectParticle* CreateEffectParticle(
  AttachedEffect& master, const RenderObjectState& masterRender, int type, float x, float y, float z, ResourceMgr& resources, JobNames& names,
  std::list<CRenderObject*>& worldObjects
) {
  auto* particle = new EffectParticle(resources, names);

  particle->state.Initialize(particle->render, master, masterRender, type, x, y, z);
  master.state.related.push_back(&particle->state);
  worldObjects.push_back(particle);

  return particle;
}
