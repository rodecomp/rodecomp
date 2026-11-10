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
  const float size = static_cast<float>(this->cellSize);
  const int dx = column - this->width / 2;
  const int dz = row - this->height / 2;
  const float left = static_cast<float>(dx) * size;
  const float right = static_cast<float>(dx + 1) * size;
  const float top = static_cast<float>(dz) * size;
  const float bottom = static_cast<float>(dz + 1) * size;
  corners[0].x = left;
  corners[0].y = cell->heights[0];
  corners[0].z = top;
  corners[1].x = right;
  corners[1].y = cell->heights[1];
  corners[1].z = top;
  corners[2].x = left;
  corners[2].y = cell->heights[2];
  corners[2].z = bottom;
  corners[3].x = right;
  corners[3].y = cell->heights[3];
  corners[3].z = bottom;
  float u;
  float v;
  float distance;

  if (IntersectRayTriangle(ray, &corners[0].x, &corners[1].x, &corners[2].x, u, v, distance)) {
    nearest = distance < nearest ? distance : nearest;
  }

  if (IntersectRayTriangle(ray, &corners[1].x, &corners[3].x, &corners[2].x, u, v, distance)) {
    nearest = distance < nearest ? distance : nearest;
  }

  return nearest;
}
