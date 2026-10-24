#include "Contents/EffectActor.h"
#include "Contents/EffectParticle.h"
#include "Contents/PartyEffect.h"
#include "Contents/RagEffectState.h"
#include "Contents/TerrainHeight.h"
#include "Render/TextureCache.h"

#include <bit>
#include <limits>
#include <new>

// OFFSET: 0x00C2CF20
// FUNCTION: fn_20088
bool UpdatePartyEffect(
  RagEffectState& effect,
  RenderObjectState& render,
  ResourceMgr& resources,
  JobNames& names,
  std::list<CRenderObject*>& worldObjects,
  TextureCache& textures,
  const TerrainHeight& ground,
  int updateTick
) {
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
  render.position[0] = actor.position[0];
  render.position[1] = actor.position[1];
  render.position[2] = actor.position[2];

  if (effect.unknown_0178 == 9998) {
    effect.unknown_0178 = 0;
  }

  if (effect.unknown_0178 == 0) {
    EffectParticle* particle = CreateEffectParticle(effect, render, 15, 0, 0, 0, resources, names, worldObjects);
    effect.unknown_11C84 = &particle->state;
    EffectPrimitiveState& state = *effect.unknown_11C84;
    state.lifetime = 9999;
    state.unknown_030C = 0;
    state.unknown_0314 = 90.0f;
    state.unknown_0354 = 4.0f;
    state.unknown_0360 = 4.0f;
    state.red = 50;
    state.blue = 253;
    state.green = 50;
    const auto bytes = state.textureCount >> 30 != 0 ? std::numeric_limits<std::size_t>::max()
                                                     : static_cast<std::size_t>(4u * state.textureCount);
    // Original array new delegates to scalar new, whose storage is CRT malloc.
    // Keep new-handler/exception behavior while retaining its free-domain teardown.
    state.textures = static_cast<CTexture**>(::operator new(bytes));
    state.textures[0] = textures.Load("effect\\party.tga", 0, false, true, false);
  }

  render.position[1] = ground.Height(render.position[0], render.position[2]) - 2.0f;
  ++effect.unknown_0178;
  return std::bit_cast<int>(effect.unknown_0178) < effect.lifetime;
}
