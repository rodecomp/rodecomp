#include "Contents/TerrainHeight.h"

#include <bit>
#include <emmintrin.h>

// OFFSET: 0x00749040
// FUNCTION: fn_07840
float TerrainHeight::PlaneHeight(float x, float z) const {
  const float size = static_cast<float>(this->cellSize);
  const int columns = this->width;
  const int rows = this->height;
  const int column = _mm_cvttss_si32(_mm_set_ss(((static_cast<float>(columns) * size) * 0.5f + x) / size));
  const int row = _mm_cvttss_si32(_mm_set_ss(((static_cast<float>(rows) * size) * 0.5f + z) / size));
  const TerrainHeightCell* cell = this->CellAt(column, row);

  if (!cell) {
    return 0.0f;
  }

  const auto dx = static_cast<std::uint32_t>(column) - static_cast<std::uint32_t>(columns / 2);
  const auto dz = static_cast<std::uint32_t>(row) - static_cast<std::uint32_t>(rows / 2);
  const float left = static_cast<float>(std::bit_cast<int>(dx)) * size;
  const float right = static_cast<float>(std::bit_cast<int>(dx + 1u)) * size;
  const float top = static_cast<float>(std::bit_cast<int>(dz)) * size;
  const float bottom = static_cast<float>(std::bit_cast<int>(dz + 1u)) * size;
  const float first = cell->heights[0];
  const float second = cell->heights[1];
  const float third = cell->heights[2];

  // Keep cancelling terms and both negative multiplications: they are
  // observable for nonfinite values and rounded scalar intermediates.
  const float xTerm = (((bottom - top) * second + (top - bottom) * first) + (top - top) * third) * x;
  const float zTerm = (((third - first) * right + (second - third) * left) + (first - second) * left) * z;
  const float constant = (((top * third - bottom * first) * right + (bottom * second - top * third) * left) +
                          (top * first - top * second) * left) *
                         -1.0f;
  const float numerator = ((xTerm + zTerm) + constant) * -1.0f;
  const float denominator = ((right - left) * top + (left - left) * top) + (left - right) * bottom;
  return numerator / denominator;
}
