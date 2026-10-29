#include "Client/Mode/CGameMode.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/ScalarConversion.h"

#include <cmath>

// OFFSET: 0x00AC8150
// FUNCTION: fn_17666
void TerrainHeight::CellOrigin(int column, int row, float& x, float& z) const {
  x = static_cast<float>((column - this->width / 2) * this->cellSize);
  z = static_cast<float>((row - this->height / 2) * this->cellSize);
}

// OFFSET: 0x00CD36E0
// FUNCTION: fn_21270
void TerrainHeight::CellCenter(int column, int row, float& x, float& z) const {
  const float size = static_cast<float>(this->cellSize);
  const float offset = (size * 8.0f) * 0.0625f;
  x = ((static_cast<float>(column) - static_cast<float>(this->width) * 0.5f) * size) + offset;
  z = ((static_cast<float>(row) - static_cast<float>(this->height) * 0.5f) * static_cast<float>(this->cellSize)) + offset;
}

// OFFSET: 0x00CD3620
// FUNCTION: fn_21269
void CGameMode::SubcellPosition(int column, int row, int subColumn, int subRow, float& x, float& z) const {
  x = (static_cast<float>(column) - static_cast<float>(this->scene->terrain->width) * 0.5f) * static_cast<float>(this->scene->terrain->cellSize) +
    static_cast<float>(subColumn) * static_cast<float>(this->scene->terrain->cellSize) * 0.0625f;
  z = (static_cast<float>(row) - static_cast<float>(this->scene->terrain->height) * 0.5f) * static_cast<float>(this->scene->terrain->cellSize) +
    static_cast<float>(subRow) * static_cast<float>(this->scene->terrain->cellSize) * 0.0625f;
}

// OFFSET: 0x00CD5230
// FUNCTION: fn_21278
void CGameMode::WorldCell(float x, float z, int& column, int& row) const {
  const float size = static_cast<float>(this->scene->terrain->cellSize);
  const float first = static_cast<float>(this->scene->terrain->width / 2) + x / size;
  const float flooredX = static_cast<float>(std::floor(static_cast<double>(first)));
  column = TruncateToInt(flooredX);
  const float second = static_cast<float>(this->scene->terrain->height / 2) + z / size;
  const float flooredZ = static_cast<float>(std::floor(static_cast<double>(second)));
  row = TruncateToInt(flooredZ);
}

// OFFSET: 0x00CD50A0
// FUNCTION: fn_21277
void TerrainHeight::WorldSubcell(float x, float z, int& column, int& row, int& subColumn, int& subRow) const {
  const float size = static_cast<float>(this->cellSize);
  const float localX = x / size;
  const float gridX = static_cast<float>(this->width / 2) + localX;
  const float flooredX = static_cast<float>(std::floor(static_cast<double>(gridX)));
  const float localZ = z / size;
  column = TruncateToInt(flooredX);
  const float gridZ = static_cast<float>(this->height / 2) + localZ;
  const float flooredZ = static_cast<float>(std::floor(static_cast<double>(gridZ)));
  row = TruncateToInt(flooredZ);

  const float fractionX = (static_cast<float>(this->width / 2) + localX) - static_cast<float>(column);
  const float sixteenthsX = static_cast<float>(std::floor(static_cast<double>(fractionX * 16.0f)));
  subColumn = TruncateToInt(sixteenthsX) % 16;
  const float fractionZ = (static_cast<float>(this->height / 2) + localZ) - static_cast<float>(row);
  const float sixteenthsZ = static_cast<float>(std::floor(static_cast<double>(fractionZ * 16.0f)));
  subRow = TruncateToInt(sixteenthsZ) % 16;
}

// OFFSET: 0x00A8C790
// FUNCTION: fn_17135
void TerrainHeight::SpatialCell(float x, float z, int& column, int& row) const {
  const int halfRows = 2 * (this->height / 4);
  const float first = static_cast<float>(std::floor(static_cast<double>(x / static_cast<float>(this->cellSize))));
  const int firstCell = TruncateToInt(first);
  column = firstCell + 2 * (this->width / 4);
  const float second = static_cast<float>(std::floor(static_cast<double>(z / static_cast<float>(this->cellSize))));
  row = TruncateToInt(second) + halfRows;
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

      ++row;
    }

    ++column;
  }
}
