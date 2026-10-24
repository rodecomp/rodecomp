#include "Contents/TerrainHeight.h"

#include <emmintrin.h>

// OFFSET: 0x00748D80
// FUNCTION: fn_07838
float TerrainHeight::Height(float x, float z) const {
  const float size = static_cast<float>(this->cellSize);
  const float gridX = (static_cast<float>(this->width) * size) * 0.5f + x;
  const float gridZ = (static_cast<float>(this->height) * size) * 0.5f + z;
  const int column = _mm_cvttss_si32(_mm_set_ss(gridX / size));
  const int row = _mm_cvttss_si32(_mm_set_ss(gridZ / size));

  if (column < 0 || column >= this->width || row < 0 || row >= this->height) {
    return 0.0f;
  }

  // The original checks the computed address, not the array base. Preserve
  // that distinction; a null base with a nonzero index is not a valid map.
  const auto index =
    static_cast<std::uint32_t>(column) + static_cast<std::uint32_t>(row) * static_cast<std::uint32_t>(this->width);
  const auto address =
    reinterpret_cast<std::uintptr_t>(this->cells) + static_cast<std::uintptr_t>(index) * sizeof(TerrainHeightCell);
  const auto* cell = reinterpret_cast<const TerrainHeightCell*>(address);

  if (!cell) {
    return 0.0f;
  }

  const float localZ = gridZ - static_cast<float>(row) * size;
  const float first = ((cell->heights[2] - cell->heights[0]) * localZ) / size + cell->heights[0];
  const float second = ((cell->heights[3] - cell->heights[1]) * localZ) / size + cell->heights[1];
  const float localX = gridX - static_cast<float>(column) * size;
  return (localX * (second - first)) / size + first;
}

// OFFSET: 0x00748D30
// FUNCTION: fn_07837
const TerrainHeightCell* TerrainHeight::CellAt(int column, int row) const {
  if (column < 0 || column >= this->width || row < 0 || row >= this->height) {
    return nullptr;
  }

  const auto index =
    static_cast<std::uint32_t>(column) + static_cast<std::uint32_t>(row) * static_cast<std::uint32_t>(this->width);
  return reinterpret_cast<const TerrainHeightCell*>(
    reinterpret_cast<std::uintptr_t>(this->cells) + static_cast<std::uintptr_t>(index) * sizeof(TerrainHeightCell)
  );
}

// OFFSET: 0x007492A0
// FUNCTION: fn_07841
bool TerrainHeight::Walkable(int column, int row) const {
  const TerrainHeightCell* cell = this->CellAt(column, row);
  return cell && cell->type != 1 && cell->type != 5;
}
