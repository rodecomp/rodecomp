#include "Contents/World/WorldRayQuery.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Contents/World/WorldTerrainPicking.h"
#include "Render/Projection/RenderProjection.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00CD4060
// FUNCTION: fn_21274
bool PickTerrainRay(const float* ray, WorldSpatialTree& root, int& column, int& row) {
  WorldRayQuery query;
  query.Collect(ray, &root);
  float nearest = 999999.0f;

  for (WorldSpatialTree* node : query.nodes) {
    C3dAttr* terrain = node->terrain;
    int candidateRow = node->terrainCells[1];

    while (candidateRow <= node->terrainCells[3]) {
      int candidateColumn = node->terrainCells[0];

      while (candidateColumn <= node->terrainCells[2]) {
        const float distance = terrain->Grid().RayDistance(ray, candidateColumn, candidateRow);

        if (nearest > distance) {
          nearest = distance;
          column = candidateColumn;
          row = candidateRow;
        }

        candidateColumn = candidateColumn + 1;
      }

      candidateRow = candidateRow + 1;
    }
  }

  return 999999.0f > nearest;
}

// OFFSET: 0x00CD4060
// FUNCTION: fn_21274
bool PickScreenTerrain(
  const RenderProjection& projection, int x, int y, const float* origin, const float* cameraMatrix, WorldSpatialTree& root, int& column, int& row
) {
  float ray[6]{};
  projection.Ray(x, y, origin, cameraMatrix, ray);
  return PickTerrainRay(ray, root, column, row);
}
