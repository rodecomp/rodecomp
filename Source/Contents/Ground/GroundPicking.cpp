#include "Contents/Ground/GroundGeometry.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/RayTriangle.h"

#include <cstring>

// OFFSET: 0x00AC1010
// FUNCTION: fn_17574
void GroundGeometry::CellBounds(int column, int row, float& minimum, float& maximum) const {
  if (column < 0 || column >= this->width || row < 0 || row >= this->height) {
    minimum = 0.0f;
    maximum = 0.0f;
    return;
  }

  minimum = 999999.0f;
  maximum = -999999.0f;
  const GroundQuad* quad = this->cells[static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width)].top;

  if (quad) {
    const float heights[4] = {quad->vertices[0][1], quad->vertices[1][1], quad->vertices[2][1], quad->vertices[3][1]};
    float high = -999999.0f;

    for (float value : heights) {
      if (value > high) {
        maximum = value;
        high = value;
      }
    }

    float low = minimum;

    for (float value : heights) {
      if (low > value) {
        minimum = value;
        low = value;
      }
    }
  }
}

// OFFSET: 0x00AC1110
// FUNCTION: fn_17575
void GroundGeometry::Slot08(int, int, int) const {
}

// OFFSET: 0x00AC1120
// FUNCTION: fn_17576
void GroundGeometry::Slot07(int, int, int, int) const {
}

// OFFSET: 0x00AC0E30
// FUNCTION: fn_17572
bool GroundGeometry::OverlayHeights(const C3dAttr& terrain, float* vertices, int column, int row) const {
  const TerrainHeightCell* cell = terrain.Grid().CellAt(column, row);

  if (!cell) {
    return false;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    vertices[corner * 3 + 1] = cell->heights[corner];
  }

  return true;
}

// OFFSET: 0x00AC0D40
// FUNCTION: fn_17571
bool GroundGeometry::CellVertices(const C3dAttr& terrain, float* vertices, int column, int row) const {
  if (column < 0 || (column >> 1) >= this->width || row < 0 || (row >> 1) >= this->height) {
    return false;
  }

  const GroundQuad* quad = this->cells[static_cast<unsigned>(column >> 1) + static_cast<unsigned>(row >> 1) * static_cast<unsigned>(this->width)].top;

  if (!quad) {
    return false;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    for (unsigned axis = 0; axis < 3; ++axis) {
      vertices[corner * 3 + axis] = quad->vertices[corner][axis];
    }
  }

  this->OverlayHeights(terrain, vertices, column, row);
  return true;
}

// OFFSET: 0x00ABFD60
// FUNCTION: fn_17560
void GroundGeometry::MarkRay(const float* ray, int column, int row, float limit) {
  if (column < 0 || column >= this->width || row < 0 || row >= this->height) {
    return;
  }

  GroundCell& cell = this->cells[static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width)];
  GroundQuad* quads[3] = {cell.top, cell.side2, cell.side1};

  for (GroundQuad* quad : quads) {
    if (!quad) {
      continue;
    }

    float vertices[4][3];

    for (unsigned corner = 0; corner < 4; ++corner) {
      std::memcpy(vertices[corner], quad->vertices[corner], sizeof(vertices[corner]));
    }

    float u;
    float v;
    float distance;

    if (IntersectRayTriangle(ray, vertices[0], vertices[1], vertices[2], u, v, distance) && limit > distance) {
      quad->selected = 1;
    }

    if (quad->selected == 0 && IntersectRayTriangle(ray, vertices[1], vertices[3], vertices[2], u, v, distance) && limit > distance) {
      quad->selected = 1;
    }
  }
}

// OFFSET: 0x00AC0170
// FUNCTION: fn_17561
void GroundGeometry::Slot09(int, int, int, int) {
}
