#include <algorithm>
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/RayTriangle.h"

// OFFSET: 0x00749300
// FUNCTION: fn_07842
float C3dAttr::RayDistance(const float* ray, int column, int row) const {
  float nearest = 999999.0f;
  const TerrainHeightCell* cell = this->CellAt(column, row);
  if (!cell) {
    return 0.0f;
  }

  struct Point {
    // INLINED
    Point() : x(0), y(0), z(0) {
    }

    float x;
    float y;
    float z;
  };
  Point corners[4];
  corners[0].y = cell->heights[0];
  const float size = static_cast<float>(this->cellSize);
  const int dx = column - this->width / 2;
  const int dz = row - this->height / 2;
  corners[0].x = static_cast<float>(dx) * size;
  corners[0].z = static_cast<float>(dz) * size;
  corners[1].y = cell->heights[1];
  corners[1].x = static_cast<float>(dx + 1) * size;
  corners[1].z = static_cast<float>(dz) * size;
  const float bottomLeft = cell->heights[2];
  corners[2].x = static_cast<float>(dx) * size;
  corners[2].y = bottomLeft;
  corners[2].z = static_cast<float>(dz + 1) * size;
  const float bottomRight = cell->heights[3];
  corners[3].x = static_cast<float>(dx + 1) * size;
  corners[3].y = bottomRight;
  corners[3].z = static_cast<float>(dz + 1) * size;
  float u;
  float v;
  float distance;

  if (IntersectRayTriangle(ray, &corners[0].x, &corners[1].x, &corners[2].x, u, v, distance)) {
    nearest = std::min(nearest, distance);
  }

  if (IntersectRayTriangle(ray, &corners[1].x, &corners[3].x, &corners[2].x, u, v, distance)) {
    nearest = std::min(nearest, distance);
  }

  return nearest;
}
