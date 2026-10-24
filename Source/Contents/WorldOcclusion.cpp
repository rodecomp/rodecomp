#include "Contents/GroundGeometry.h"
#include "Contents/ModelActor.h"
#include "Contents/WorldOcclusion.h"
#include "Contents/WorldRayQuery.h"
#include "Contents/WorldSpatialTree.h"
#include "Render/RenderProjection.h"

#include <bit>
#include <cstring>
#include <emmintrin.h>

// OFFSET: 0x00CD5AD0
// FUNCTION: fn_21286
void MarkWorldOcclusionRay(
  const float* ray,
  float actorLimit,
  float groundLimit,
  WorldSpatialTree& root,
  GroundGeometry& ground,
  bool markGround
) {
  float actorRay[7];
  std::memcpy(actorRay, ray, 6 * sizeof(float));
  actorRay[6] = actorLimit;
  WorldRayQuery query;
  query.Collect(ray, &root);

  for (WorldSpatialTree* node : query.nodes) {
    for (C3dActor* model : node->objects) {
      model->MarkOccluded(actorRay);
    }
  }

  if (markGround) {
    for (WorldSpatialTree* node : query.nodes) {
      int row = node->groundCells[1];

      while (row <= node->groundCells[3]) {
        int column = node->groundCells[0];

        while (column <= node->groundCells[2]) {
          ground.MarkRay(ray, column, row, groundLimit);
          column = std::bit_cast<int>(static_cast<unsigned>(column) + 1u);
        }

        row = std::bit_cast<int>(static_cast<unsigned>(row) + 1u);
      }
    }
  }
}

void MarkScreenWorldOcclusion(
  const RenderProjection& projection,
  int screenX,
  int screenY,
  float rendererScale,
  const float* origin,
  const float* cameraMatrix,
  float playerHeight,
  float cameraHeight,
  WorldSpatialTree& root,
  GroundGeometry& ground,
  bool markGround
) {
  const float shifted = static_cast<float>(screenY) - rendererScale * 23.0f;
  const int y = _mm_cvttss_si32(_mm_set_ss(shifted));
  float ray[6]{};
  projection.Ray(screenX, y, origin, cameraMatrix, ray);
  const float limit = (playerHeight + cameraHeight) - 16.0f;
  MarkWorldOcclusionRay(ray, limit, limit, root, ground, markGround);
}
