#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/World/World.h"
#include "Contents/World/WorldSpatialInitialization.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Resource/World/TerrainResource.h"

#include <atomic>

// OFFSET: 0x00AC79C0
// FUNCTION: fn_17660
void CWorld::InitializeSpatial() {
  this->spatial.ClearChildren();
  const int halfWidth = this->ground->width / 2;
  const int halfHeight = this->ground->height / 2;
  this->spatial.minimum[0] = static_cast<float>(-halfWidth) * this->ground->cellSize;
  this->spatial.maximum[0] = static_cast<float>(this->ground->width - halfWidth) * this->ground->cellSize - 1.0f;
  this->spatial.minimum[2] = static_cast<float>(-halfHeight) * this->ground->cellSize;
  this->spatial.maximum[2] = static_cast<float>(this->ground->height - halfHeight) * this->ground->cellSize - 1.0f;
  this->spatial.center[0] = 0;
  this->spatial.center[2] = 0;
  this->spatial.Subdivide(0);

  for (auto it = this->models.begin(); it != this->models.end(); ++it) {
    C3dActor* model = *it;

    for (int i = 0; i < 4; ++i) {
      WorldSpatialTree* child = this->spatial.children[i];
      if (model->oriented.Intersects(child->minimum)) {
        child->InsertModel(*model, 1);
      }
    }
  }

  this->spatial.BindGround(*this->ground, 0);
  this->spatial.BindTerrain(*this->terrain, 0);

  if (this->serializedSpatial) {
    this->spatial.CopyBounds(0, *this->serializedSpatial);
  } else {
    this->spatial.dirty = 0;

    for (int i = 0; i < 4; ++i) {
      WorldSpatialTree* child = this->spatial.children[i];
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
