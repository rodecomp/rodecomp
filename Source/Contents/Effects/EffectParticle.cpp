#include "Contents/Effects/EffectParticle.h"
#include "Contents/Effects/AttachedEffect.h"

// OFFSET: 0x00BAD940
// FUNCTION: fn_19761
EffectParticle::EffectParticle() : state(this->render) {
}

// OFFSET: 0x00C1DE70
// FUNCTION: fn_20076
EffectParticle* CreateEffectParticle(
  AttachedEffect& master, const RenderObjectState& masterRender, int type, float x, float y, float z,
  std::list<CRenderObject*>& worldObjects
) {
  auto* particle = new EffectParticle();

  particle->state.Initialize(particle->render, master, masterRender, type, x, y, z);
  master.state.related.push_back(&particle->state);
  worldObjects.push_back(particle);

  return particle;
}
