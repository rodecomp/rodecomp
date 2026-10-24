#include "Contents/ModelActor.h"
#include "Contents/WorldModelPlacement.h"
#include "Resource/WorldResource.h"

#include <cstring>

// OFFSET: 0x00AC94C0
// FUNCTION: fn_17684
// OFFSET: 0x00AC8BF0
// FUNCTION: fn_17680
// Movie registration precedes node suitability.
C3dActor* CreatePlacedWorldModel(
  const WorldModelRecord& placement,
  C3dModelRes& resource,
  BinkMovies& movies,
  ModelLightingContext& lighting
) {
  resource.RegisterMovies(movies, placement.position);

  if (!resource.FindNode(placement.node) && resource.nodes.empty()) {
    return nullptr;
  }

  C3dActor* actor = nullptr;

  if (resource.major == 1) {
    actor = new C3dActorV1;
  } else if (resource.major == 2) {
    actor = new C3dActorV2;
  }

  actor->CopyVolumes(resource.volumes);
  actor->SetSpeed(placement.animationSpeed);
  actor->playback = placement.animation;
  std::memcpy(actor->position, placement.position, sizeof(actor->position));
  actor->dirty = 1;
  std::memcpy(actor->rotation, placement.rotation, sizeof(actor->rotation));
  actor->dirty = 1;
  std::memcpy(actor->scale, placement.scale, sizeof(actor->scale));
  actor->dirty = 1;
  actor->commandMode = placement.flag;
  actor->color = static_cast<std::uint32_t>(placement.index);
  actor->Assign(resource);
  actor->occlusionEnabled = 0;
  actor->Prepare(lighting);
  return actor;
}
