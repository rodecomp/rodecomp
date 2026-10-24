#include "Contents/TerrainHeight.h"

#include <bit>
#include <cmath>
#include <emmintrin.h>

// OFFSET: 0x00AC8150
// FUNCTION: fn_17666
void TerrainHeight::CellOrigin(int column, int row, float& x, float& z) const {
  const auto dx = static_cast<std::uint32_t>(column) - static_cast<std::uint32_t>(this->width / 2);
  const auto first = dx * static_cast<std::uint32_t>(this->cellSize);
  x = static_cast<float>(std::bit_cast<int>(first));
  const auto dz = static_cast<std::uint32_t>(row) - static_cast<std::uint32_t>(this->height / 2);
  const auto second = dz * static_cast<std::uint32_t>(this->cellSize);
  z = static_cast<float>(std::bit_cast<int>(second));
}

// OFFSET: 0x00CD36E0
// FUNCTION: fn_21270
void TerrainHeight::CellCenter(int column, int row, float& x, float& z) const {
  const float size = static_cast<float>(this->cellSize);
  const float offset = (size * 8.0f) * 0.0625f;
  x = ((static_cast<float>(column) - static_cast<float>(this->width) * 0.5f) * size) + offset;
  z =
    ((static_cast<float>(row) - static_cast<float>(this->height) * 0.5f) * static_cast<float>(this->cellSize)) + offset;
}

// OFFSET: 0x00CD5230
// FUNCTION: fn_21278
void TerrainHeight::WorldCell(float x, float z, int& column, int& row) const {
  const float size = static_cast<float>(this->cellSize);
  const float first = static_cast<float>(this->width / 2) + x / size;
  const float flooredX = static_cast<float>(std::floor(static_cast<double>(first)));
  column = _mm_cvttss_si32(_mm_set_ss(flooredX));
  const float second = static_cast<float>(this->height / 2) + z / size;
  const float flooredZ = static_cast<float>(std::floor(static_cast<double>(second)));
  row = _mm_cvttss_si32(_mm_set_ss(flooredZ));
}

// OFFSET: 0x00CD50A0
// FUNCTION: fn_21277
void TerrainHeight::WorldSubcell(float x, float z, int& column, int& row, int& subColumn, int& subRow) const {
  const float size = static_cast<float>(this->cellSize);
  const float localX = x / size;
  const float gridX = static_cast<float>(this->width / 2) + localX;
  const float flooredX = static_cast<float>(std::floor(static_cast<double>(gridX)));
  const float localZ = z / size;
  column = _mm_cvttss_si32(_mm_set_ss(flooredX));
  const float gridZ = static_cast<float>(this->height / 2) + localZ;
  const float flooredZ = static_cast<float>(std::floor(static_cast<double>(gridZ)));
  row = _mm_cvttss_si32(_mm_set_ss(flooredZ));

  const float fractionX = (static_cast<float>(this->width / 2) + localX) - static_cast<float>(column);
  const float sixteenthsX = static_cast<float>(std::floor(static_cast<double>(fractionX * 16.0f)));
  subColumn = _mm_cvttss_si32(_mm_set_ss(sixteenthsX)) % 16;
  const float fractionZ = (static_cast<float>(this->height / 2) + localZ) - static_cast<float>(row);
  const float sixteenthsZ = static_cast<float>(std::floor(static_cast<double>(fractionZ * 16.0f)));
  subRow = _mm_cvttss_si32(_mm_set_ss(sixteenthsZ)) % 16;
}

// OFFSET: 0x00A8C790
// FUNCTION: fn_17135
void TerrainHeight::SpatialCell(float x, float z, int& column, int& row) const {
  const int halfRows = 2 * (this->height / 4);
  const float first = static_cast<float>(std::floor(static_cast<double>(x / static_cast<float>(this->cellSize))));
  const int firstCell = _mm_cvttss_si32(_mm_set_ss(first));
  column = std::bit_cast<int>(static_cast<unsigned>(firstCell) + static_cast<unsigned>(2 * (this->width / 4)));
  const float second = static_cast<float>(std::floor(static_cast<double>(z / static_cast<float>(this->cellSize))));
  row =
    std::bit_cast<int>(static_cast<unsigned>(_mm_cvttss_si32(_mm_set_ss(second))) + static_cast<unsigned>(halfRows));
}

// OFFSET: 0x00748E90
// FUNCTION: fn_07839
void TerrainHeight::HeightBounds(const int* rectangle, float& minimum, float& maximum) const {
  minimum = 999999.0f;
  maximum = -999999.0f;
  int column = rectangle[0];

  while (column <= rectangle[2]) {
    int row = rectangle[1];

    while (row <= rectangle[3]) {
      const TerrainHeightCell* cell = this->CellAt(column, row);

      if (cell) {
        // Maximum writes precede minimum reads, including aliased outputs.
        for (float value : cell->heights) {
          if (value > maximum) {
            maximum = value;
          }
        }

        for (float value : cell->heights) {
          if (minimum > value) {
            minimum = value;
          }
        }
      }

      row = std::bit_cast<int>(static_cast<std::uint32_t>(row) + 1u);
    }

    column = std::bit_cast<int>(static_cast<std::uint32_t>(column) + 1u);
  }
}
