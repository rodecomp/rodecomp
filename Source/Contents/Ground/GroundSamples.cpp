#include "Contents/Ground/GroundGeometry.h"
#include "Resource/World/TerrainResource.h"

#include <cmath>

// OFFSET: 0x00AC0180
// FUNCTION: fn_17562
void GroundBase::SpatialCell(float x, float z, int& column, int& row) const {
  column = static_cast<int>(std::floor(x / this->cellSize)) + this->width / 2;
  row = static_cast<int>(std::floor(z / this->cellSize)) + this->height / 2;
}

// OFFSET: 0x00AC09B0
// FUNCTION: fn_17568
void GroundGeometry::WorldColor(vector3d position, std::uint32_t* output) const {
  int column;
  int row;
  this->SpatialCell(position.x, position.z, column, row);

  if (column < 0 || row < 0 || column >= this->width || row >= this->height) {
    output[1] = 255;
    output[0] = 255;
    output[2] = 255;

    return;
  }

  const GroundCell& cell = this->cells[static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width)];
  output[0] = cell.color[0];
  output[1] = cell.color[1];
  output[2] = cell.color[2];
}

// OFFSET: 0x00AC1130
// FUNCTION: fn_17577
int GroundGeometry::Lighting(int column, int row, float& red, float& green, float& blue) const {
  if (column < 0 || (column >> 1) >= this->width || row < 0 || (row >> 1) >= this->height || this->lighting.empty()) {
    red = 1.0f;
    green = 1.0f;
    blue = 1.0f;

    return 1;
  }

  const unsigned index = static_cast<unsigned>(column) + 2u * static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
  red = this->lighting[index][0];
  green = this->lighting[index][1];
  blue = this->lighting[index][2];
  return 1;
}

// OFFSET: 0x00AC3090
// FUNCTION: fn_45872
void GroundGeometry::SetLighting(int column, int row, float& red, float& green, float& blue) {
  if (!this->lighting.empty()) {
    const unsigned index = static_cast<unsigned>(column) + 2u * static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
    this->lighting[index][0] = red;
    this->lighting[index][1] = green;
    this->lighting[index][2] = blue;
  }
}

// OFFSET: 0x00AC11C0
// FUNCTION: fn_17578
int GroundBase::Lighting(int, int, float&, float&, float&) const {
  return 0;
}

// INLINED
static __forceinline void ReadLightmapSample(const GroundCell& cell, unsigned sample, unsigned char& alpha, unsigned char& red, unsigned char& green, unsigned char& blue) {
  alpha = cell.top->lightmap->cornerColors[sample].a;
  red = cell.top->lightmap->cornerColors[sample].r;
  green = cell.top->lightmap->cornerColors[sample].g;
  blue = cell.top->lightmap->cornerColors[sample].b;
}

// OFFSET: 0x00AC11D0
// FUNCTION: fn_17579
void GroundGeometry::LightmapSample(int column, int row, unsigned char& alpha, unsigned char& red, unsigned char& green, unsigned char& blue) const {
  if (column >= 0 && (column >> 1) < this->width && row >= 0 && (row >> 1) < this->height && !this->cells.empty()) {
    const unsigned index = static_cast<unsigned>(column >> 1) + static_cast<unsigned>(row >> 1) * static_cast<unsigned>(this->width);
    const GroundCell& cell = this->cells[index];

    if (cell.top && cell.top->lightmap) {
      if ((column & 1) == 0) {
        if ((row & 1) == 0) {
          ReadLightmapSample(cell, 0, alpha, red, green, blue);
        } else {
          ReadLightmapSample(cell, 2, alpha, red, green, blue);
        }
      } else {
        if ((row & 1) == 0) {
          ReadLightmapSample(cell, 1, alpha, red, green, blue);
        } else {
          ReadLightmapSample(cell, 3, alpha, red, green, blue);
        }
      }
      return;
    }
  }

  alpha = 255;
  red = 255;
  green = 255;
  blue = 255;
}

#pragma inline_depth(0)
// OFFSET: 0x00AC0C40
// FUNCTION: fn_17570
bool GroundGeometry::TerrainCellCorners(int column, int row, float* vertices) const {
  if (column < 0 || 2 * this->width < column || row < 0 || 2 * this->height < row) {
    return false;
  }

  const float size = this->cellSize * 0.5f;
  int coordinate = column - this->width;
  const float left = static_cast<float>(coordinate++) * size;
  const float right = static_cast<float>(coordinate) * size;
  coordinate = row - this->height;
  const float top = static_cast<float>(coordinate++) * size;
  const float bottom = static_cast<float>(coordinate) * size;
  vertices[1] = 0.0f;
  vertices[4] = 0.0f;
  vertices[7] = 0.0f;
  vertices[10] = 0.0f;
  vertices[0] = left;
  vertices[2] = top;
  vertices[3] = right;
  vertices[5] = top;
  vertices[6] = left;
  vertices[8] = bottom;
  vertices[9] = right;
  vertices[11] = bottom;

  const TerrainHeightCell* cell = this->terrain->CellAt(column, row);

  if (!cell) {
    return false;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner * 3 + 1] = cell->heights[corner];
  }

  return true;
}
#pragma inline_depth()
