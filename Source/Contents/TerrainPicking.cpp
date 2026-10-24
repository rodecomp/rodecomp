#include "Contents/TerrainHeight.h"
#include "Utility/RayTriangle.h"

#include <bit>
#include <emmintrin.h>

// OFFSET: 0x00749300
// FUNCTION: fn_07842
float TerrainHeight::RayDistance(const float* ray, int column, int row) const {
  const TerrainHeightCell* cell = this->CellAt(column, row);

  if (!cell) {
    return 0.0f;
  }

  const float size = static_cast<float>(this->cellSize);
  const auto x = static_cast<std::uint32_t>(column) - static_cast<std::uint32_t>(this->width / 2);
  const auto z = static_cast<std::uint32_t>(row) - static_cast<std::uint32_t>(this->height / 2);
  const float left = static_cast<float>(std::bit_cast<int>(x)) * size;
  const float right = static_cast<float>(std::bit_cast<int>(x + 1u)) * size;
  const float top = static_cast<float>(std::bit_cast<int>(z)) * size;
  const float bottom = static_cast<float>(std::bit_cast<int>(z + 1u)) * size;
  const float first[3] = {left, cell->heights[0], top};
  const float second[3] = {right, cell->heights[1], top};
  const float third[3] = {left, cell->heights[2], bottom};
  const float fourth[3] = {right, cell->heights[3], bottom};
  float nearest = 999999.0f;
  float u;
  float v;
  float distance;

  if (IntersectRayTriangle(ray, first, second, third, u, v, distance)) {
    nearest = _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(distance), _mm_set_ss(nearest)));
  }

  if (IntersectRayTriangle(ray, second, fourth, third, u, v, distance)) {
    nearest = _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(distance), _mm_set_ss(nearest)));
  }

  return nearest;
}
