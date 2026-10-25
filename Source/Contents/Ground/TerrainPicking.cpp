#include "Contents/Ground/TerrainHeight.h"
#include "Utility/Math/RayTriangle.h"

// OFFSET: 0x00749300
// FUNCTION: fn_07842
float TerrainHeight::RayDistance(const float* ray, int column, int row) const {
  const TerrainHeightCell* cell = this->CellAt(column, row);

  if (!cell) {
    return 0.0f;
  }

  const float size = static_cast<float>(this->cellSize);
  const int x = column - this->width / 2;
  const int z = row - this->height / 2;
  const float left = static_cast<float>(x) * size;
  const float right = static_cast<float>(x + 1) * size;
  const float top = static_cast<float>(z) * size;
  const float bottom = static_cast<float>(z + 1) * size;
  const float first[3] = {left, cell->heights[0], top};
  const float second[3] = {right, cell->heights[1], top};
  const float third[3] = {left, cell->heights[2], bottom};
  const float fourth[3] = {right, cell->heights[3], bottom};
  float nearest = 999999.0f;
  float u;
  float v;
  float distance;

  if (IntersectRayTriangle(ray, first, second, third, u, v, distance)) {
    nearest = distance < nearest ? distance : nearest;
  }

  if (IntersectRayTriangle(ray, second, fourth, third, u, v, distance)) {
    nearest = distance < nearest ? distance : nearest;
  }

  return nearest;
}
