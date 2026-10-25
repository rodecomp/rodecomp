#include "Contents/Ground/GroundSpatial.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/World/WorldSpatialInitialization.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Resource/World/TerrainResource.h"

#include <atomic>

// OFFSET: 0x00AC79C0
// FUNCTION: fn_17660
void InitializeWorldSpatial(
  WorldSpatialTree& root, GroundSpatial& ground, C3dAttr& terrain, std::span<C3dActor* const> models, const WorldSpatialTree* serialized
) {
  root.ClearChildren();
  const int halfWidth = ground.width / 2;
  const int halfHeight = ground.height / 2;
  root.minimum[0] = static_cast<float>(ground.width / -2) * ground.cellSize;
  root.maximum[0] = static_cast<float>(ground.width - halfWidth) * ground.cellSize - 1.0f;
  root.minimum[2] = static_cast<float>(ground.height / -2) * ground.cellSize;
  root.maximum[2] = static_cast<float>(ground.height - halfHeight) * ground.cellSize - 1.0f;
  root.halfSize[0] = 0;
  root.halfSize[2] = 0;
  root.Subdivide(0);

  for (C3dActor* model : models) {
    for (WorldSpatialTree* child : root.children) {
      if (model->oriented.Intersects(child->minimum)) {
        child->InsertModel(*model, 1);
      }
    }
  }

  root.BindGround(ground, 0);
  root.BindTerrain(terrain, 0);

  if (serialized) {
    root.CopyBounds(0, *serialized);
  } else {
    root.dirty = 0;

    for (WorldSpatialTree* child : root.children) {
      if (child->dirty) {
        child->RefreshBounds(1);
      }
    }
  }
}

// INLINED
void PublishWorldModels(WorldSpatialTree& root, C3dActor* const* models, int& published, int& pending) {
  std::atomic_ref<int> count(pending);
  const int ready = count.load(std::memory_order_acquire);

  if (ready == 0) {
    return;
  }

  const int end = published + ready;

  while (published < end) {
    C3dActor& model = *models[published];

    for (WorldSpatialTree* child : root.children) {
      if (model.oriented.Intersects(child->minimum)) {
        child->InsertModel(model, 1);
      }
    }

    count.fetch_sub(1, std::memory_order_seq_cst);
    ++published;
  }
}
