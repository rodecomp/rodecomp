#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/RagEffectInitialization.h"
#include "Contents/Effects/RagEffectState.h"
#include "Resource/EZeffectResource.h"
#include "Resource/ResourceMgr.h"

#include <bit>
#include <cstring>

// OFFSET: 0x00C1C2D0
// FUNCTION: fn_20074
void InitializeRagEffect(
  RagEffectState& effect, RenderObjectState& render, EffectActor* parent, const RenderObjectState& parentState, int id, float x, float y, float z,
  ServiceOptions& options, ResourceMgr& resources
) {
  effect.id = id;
  effect.parent = parent;
  render.position[0] = parentState.position[0];
  render.position[1] = parentState.position[1];
  render.position[2] = parentState.position[2];
  effect.unknown_013C[4] = std::bit_cast<std::uint32_t>(parentState.angle);
  effect.lifetime = EffectLifetime(effect.id);
  effect.offset[0] = x;
  effect.offset[1] = y;
  effect.offset[2] = z;
  char name[64];
  SelectRagEffectResource(effect, options, name);

  if (std::strlen(name) != 0) {
    auto* resource = static_cast<CEZeffectRes*>(resources.Load(name));
    effect.resource = resource;

    if (resource) {
      ++resource->references;

      effect.resourceData = &resource->maximumFrame;
      int count = resource->clipCount;

      if (resource->clips[0].frameCount == 0) {
        --count;
      }

      const auto bits = static_cast<std::uint32_t>(count);
      std::memcpy(effect.unknown_119F8, &bits, sizeof(bits));
    }
  }
}
