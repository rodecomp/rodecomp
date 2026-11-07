#include "Contents/Ground/GroundGeometry.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00AC09B0
// FUNCTION: fn_17568
void GroundGeometry::WorldColor(float x, float, float z, std::uint32_t* output) const {
  int column;
  int row;
  this->SpatialCell(x, z, column, row);

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

// OFFSET: 0x00AC11C0
// FUNCTION: fn_17578
int GroundBase::Lighting(int, int, float&, float&, float&) const {
  return 0;
}

// INLINED
static __forceinline void ReadLightmapSample(const GroundCell& cell, unsigned sample, unsigned char& alpha, unsigned char& red, unsigned char& green, unsigned char& blue) {
  alpha = static_cast<unsigned char>(cell.top->lightmap->unknown_0028[sample] >> 24);
  red = static_cast<unsigned char>(cell.top->lightmap->unknown_0028[sample] >> 16);
  green = static_cast<unsigned char>(cell.top->lightmap->unknown_0028[sample] >> 8);
  blue = static_cast<unsigned char>(cell.top->lightmap->unknown_0028[sample]);
}

// OFFSET: 0x00AC11D0
// FUNCTION: fn_17579
void GroundGeometry::LightmapSample(int column, int row, unsigned char& alpha, unsigned char& red, unsigned char& green, unsigned char& blue) const {
  if (column < 0 || (column >> 1) >= this->width || row < 0 || (row >> 1) >= this->height || this->cells.empty()) {
    alpha = red = green = blue = 255;
    return;
  }

  const unsigned index = static_cast<unsigned>(column >> 1) + static_cast<unsigned>(row >> 1) * static_cast<unsigned>(this->width);

  if (!this->cells[index].top || !this->cells[index].top->lightmap) {
    alpha = red = green = blue = 255;
    return;
  }

  const GroundCell& cell = this->cells[index];

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
}

// OFFSET: 0x00AC0C40
// FUNCTION: fn_17570
bool GroundGeometry::TerrainCellCorners(const C3dAttr& terrain, int column, int row, float* vertices) const {
  const int columns = 2 * this->width;
  const int rows = 2 * this->height;

  if (column < 0 || columns < column || row < 0 || rows < row) {
    return false;
  }

  const float size = this->cellSize * 0.5f;
  const int dx = column - this->width;
  const int dz = row - this->height;
  const float left = static_cast<float>(dx) * size;
  const float right = static_cast<float>(dx + 1) * size;
  const float top = static_cast<float>(dz) * size;
  const float bottom = static_cast<float>(dz + 1) * size;

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

  const TerrainHeightCell* cell = terrain.CellAt(column, row);

  if (!cell) {
    return false;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner * 3 + 1] = cell->heights[corner];
  }

  return true;
}
