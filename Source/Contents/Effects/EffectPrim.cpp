#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/RagEffect.h"

// OFFSET: 0x00BAD940
// FUNCTION: fn_19761
CEffectPrim::CEffectPrim() : state(this->render) {
}

// OFFSET: 0x00C1DE70
// FUNCTION: fn_20076
CEffectPrim* CreateEffectParticle(
  CRagEffect& master, const RenderObjectState& masterRender, int type, float x, float y, float z,
  std::list<CRenderObject*>& worldObjects
) {
  auto* particle = new CEffectPrim();

  particle->state.Initialize(particle->render, master, masterRender, type, x, y, z);
  master.state.primList.push_back(&particle->state);
  worldObjects.push_back(particle);

  return particle;
}
