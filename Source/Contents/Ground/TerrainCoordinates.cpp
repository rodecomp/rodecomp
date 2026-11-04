#include "Client/Mode/CGameMode.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/ScalarConversion.h"

#include <cmath>

// OFFSET: 0x00AC8150
// FUNCTION: fn_17666
void CWorld::CellOrigin(int column, int row, float& x, float& z) const {
  x = static_cast<float>((column - this->terrain->width / 2) * this->terrain->cellSize);
  z = static_cast<float>((row - this->terrain->height / 2) * this->terrain->cellSize);
}

// OFFSET: 0x00CD36E0
// FUNCTION: fn_21270
void CGameMode::CellCenter(int column, int row, float& x, float& z) const {
  x = (static_cast<float>(column) - static_cast<float>(this->scene->terrain->width) * 0.5f) * static_cast<float>(this->scene->terrain->cellSize) +
    static_cast<float>(this->scene->terrain->cellSize) * 8.0f * 0.0625f;
  z = (static_cast<float>(row) - static_cast<float>(this->scene->terrain->height) * 0.5f) * static_cast<float>(this->scene->terrain->cellSize) +
    static_cast<float>(this->scene->terrain->cellSize) * 8.0f * 0.0625f;
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
  const float flooredX = std::floor(first);
  column = static_cast<int>(flooredX);
  const float second = static_cast<float>(this->scene->terrain->height / 2) + z / size;
  const float flooredZ = std::floor(second);
  row = static_cast<int>(flooredZ);
}

// OFFSET: 0x00CD50A0
// FUNCTION: fn_21277
void CGameMode::WorldSubcell(float x, float z, int& column, int& row, int& subColumn, int& subRow) const {
  const float size = static_cast<float>(this->scene->terrain->cellSize);

  const float localX = x / size;
  const float gridX = static_cast<float>(this->scene->terrain->width / 2) + localX;
  const float flooredX = std::floor(gridX);
  column = static_cast<int>(flooredX);
  const float localZ = z / size;
  const float gridZ = static_cast<float>(this->scene->terrain->height / 2) + localZ;
  const float flooredZ = std::floor(gridZ);
  row = static_cast<int>(flooredZ);

  const float fractionX = (static_cast<float>(this->scene->terrain->width / 2) + localX) - static_cast<float>(column);
  const float sixteenthsX = std::floor(fractionX * 16.0f);
  subColumn = static_cast<int>(sixteenthsX) % 16;
  const float fractionZ = (static_cast<float>(this->scene->terrain->height / 2) + localZ) - static_cast<float>(row);
  const float sixteenthsZ = std::floor(fractionZ * 16.0f);
  subRow = static_cast<int>(sixteenthsZ) % 16;
}

// OFFSET: 0x00A8C790
// FUNCTION: fn_17135
void C3dAttr::SpatialCell(float x, float z, int& column, int& row) const {
  const int halfRows = 2 * (this->height / 4);
  const float first = std::floor(x / static_cast<float>(this->cellSize));
  const int firstCell = static_cast<int>(first);
  column = firstCell + 2 * (this->width / 4);
  const float second = std::floor(z / static_cast<float>(this->cellSize));
  row = static_cast<int>(second) + halfRows;
}

// OFFSET: 0x00748E90
// FUNCTION: fn_07839
void C3dAttr::HeightBounds(const int* rectangle, float& minimum, float& maximum) const {
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
