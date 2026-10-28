#include "Contents/Ground/GroundGeometry.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00AC09B0
// FUNCTION: fn_17568
void GroundGeometry::WorldColor(float x, float, float z, std::uint32_t* output) const {
  int column;
  int row;
  this->Spatial().SpatialCell(x, z, column, row);

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
void GroundGeometry::Lighting(int column, int row, float& red, float& green, float& blue) const {
  if (column < 0 || (column >> 1) >= this->width || row < 0 || (row >> 1) >= this->height || this->lighting.empty()) {
    red = 1.0f;
    green = 1.0f;
    blue = 1.0f;

    return;
  }

  const unsigned index = static_cast<unsigned>(column) + 2u * static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
  red = this->lighting[index][0];
  green = this->lighting[index][1];
  blue = this->lighting[index][2];
}

// OFFSET: 0x00AC11C0
// FUNCTION: fn_17578
int GroundGeometry::Slot12(int, int, int, int, int) const {
  return 0;
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

  const unsigned sample = (static_cast<unsigned>(column) & 1u) + 2u * (static_cast<unsigned>(row) & 1u);

  const auto read = [&](unsigned shift) { return static_cast<unsigned char>(this->cells[index].top->lightmap->unknown_0028[sample] >> shift); };
  alpha = read(24);
  red = read(16);
  green = read(8);
  blue = read(0);
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

  return this->OverlayHeights(terrain, vertices, column, row);
}
