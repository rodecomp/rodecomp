#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/PartyEffect.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Render/Textures/TextureCache.h"

#include <limits>

// OFFSET: 0x00C2CF20
// FUNCTION: fn_20088
bool UpdatePartyEffect(
  CRagEffect& self, ResourceMgr& resources, std::list<CRenderObject*>& worldObjects, TextureCache& textures, const TerrainHeight& ground,
  int updateTick
) {
  RagEffectState& effect = self.state;
  RenderObjectState& render = self.render;

  if (!effect.parent) {
    return false;
  }

  if (updateTick == 0) {
    return true;
  }

  if (updateTick > 0 && (effect.parent->render.unknown_00EC & 0xFFu) == 1) {
    effect.parent->render.flags = 0;
    effect.parent->render.unknown_00EC &= 0xFFFFFF00u;
  }

  const RenderObjectState& actor = effect.parent->render;
  render.position.x = actor.position.x;
  render.position.y = actor.position.y;
  render.position.z = actor.position.z;

  if (effect.unknown_0178 == 9998) {
    effect.unknown_0178 = 0;
  }

  if (effect.unknown_0178 == 0) {
    CEffectPrim* particle = CreateEffectParticle(self, render, 15, 0, 0, 0, worldObjects);
    effect.unknown_11C84 = &particle->state;
    EffectPrimitiveState& state = *effect.unknown_11C84;
    state.duration = 9999;
    state.longitude = 0;
    state.roll = 90.0f;
    state.widthSize = 4.0f;
    state.heightSize = 4.0f;
    state.red = 50;
    state.blue = 253;
    state.green = 50;
    const auto bytes = state.totalTexture >> 30 != 0 ? std::numeric_limits<std::size_t>::max() : static_cast<std::size_t>(4u * state.totalTexture);

    state.textures = static_cast<CTexture**>(::operator new(bytes));
    state.textures[0] = textures.Load("effect\\party.tga", 0, false, true, false);
  }

  render.position.y = ground.Height(render.position.x, render.position.z) - 2.0f;
  ++effect.unknown_0178;
  return effect.unknown_0178 < effect.lifetime;
}
