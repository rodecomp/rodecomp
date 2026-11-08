#include "Resource/World/TerrainResource.h"

#include "Client/Application/ErrorDialog.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00748C80
// FUNCTION: fn_07836
std::uint32_t C3dAttr::CellType(float x, float z) const {
  const float size = static_cast<float>(this->cellSize);
  const int column = static_cast<int>(((static_cast<float>(this->width) * size) * 0.5f + x) / size);
  if (column >= 0) {
    const int row = static_cast<int>(((static_cast<float>(this->height) * size) * 0.5f + z) / size);
    if (row >= 0 && column < this->width && row < this->height) {
      return this->CellAt(column, row)->type;
    }
  }

  ShowErrorDialog(static_cast<int>(x));
  ShowErrorDialog(static_cast<int>(z));
  return 1;
}

// OFFSET: 0x00748D80
// FUNCTION: fn_07838
float C3dAttr::Height(float x, float z) const {
  const float size = static_cast<float>(this->cellSize);
  const float gridX = (static_cast<float>(this->width) * size) * 0.5f + x;
  const int column = static_cast<int>(gridX / size);
  const float gridZ = (static_cast<float>(this->height) * size) * 0.5f + z;
  const int row = static_cast<int>(gridZ / size);

  const TerrainHeightCell* cell = this->CellAt(column, row);

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
const TerrainHeightCell* C3dAttr::CellAt(int column, int row) const {
  if (column < 0 || column >= this->width || row < 0 || row >= this->height) {
    return nullptr;
  }

  return this->m_cells.data() + column + row * this->width;
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
        for (int index = 0; index < 4; ++index) {
          if (maximum < cell->heights[index]) {
            maximum = cell->heights[index];
          }
        }

        for (int index = 0; index < 4; ++index) {
          if (minimum > cell->heights[index]) {
            minimum = cell->heights[index];
          }
        }
      }

      ++row;
    }

    ++column;
  }
}

// OFFSET: 0x007492A0
// FUNCTION: fn_07841
bool C3dAttr::Walkable(int column, int row) const {
  const TerrainHeightCell* cell = this->CellAt(column, row);
  return cell && cell->type != 1 && cell->type != 5;
}
