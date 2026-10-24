#include "Contents/GroundSpatial.h"

#include <bit>
#include <cmath>
#include <emmintrin.h>

// OFFSET: 0x00ABD8F0
// FUNCTION: fn_17548
GroundQuad::GroundQuad() : texture(nullptr), selected(0) {
  for (auto& vertex : this->vertices) {
    vertex[0] = 0.0f;
    vertex[1] = 0.0f;
    vertex[2] = 0.0f;
  }
}

// OFFSET: 0x00ABD8A0
// FUNCTION: fn_17547
GroundCell::GroundCell() : corners{}, top(nullptr), side1(nullptr), side2(nullptr), color{255, 255, 255} {
}

// OFFSET: 0x00AC0180
// FUNCTION: fn_17562
void GroundSpatial::SpatialCell(float x, float z, int& column, int& row) const {
  const float first = static_cast<float>(std::floor(static_cast<double>(x / this->cellSize)));
  column = std::bit_cast<int>(
    static_cast<unsigned>(_mm_cvttss_si32(_mm_set_ss(first))) + static_cast<unsigned>(this->width / 2)
  );
  const float second = static_cast<float>(std::floor(static_cast<double>(z / this->cellSize)));
  row = std::bit_cast<int>(
    static_cast<unsigned>(_mm_cvttss_si32(_mm_set_ss(second))) + static_cast<unsigned>(this->height / 2)
  );
}

// OFFSET: 0x00AC0EA0
// FUNCTION: fn_17573
void GroundSpatial::HeightBounds(const int* rectangle, float& minimum, float& maximum) const {
  if (
    rectangle[0] < 0 || rectangle[0] >= this->width || rectangle[2] < 0 || rectangle[2] >= this->width ||
    rectangle[1] < 0 || rectangle[1] >= this->height || rectangle[3] < 0 || rectangle[3] >= this->height ||
    rectangle[2] <= rectangle[0] || rectangle[3] <= rectangle[1]
  ) {
    minimum = 0.0f;
    maximum = 0.0f;
    return;
  }

  minimum = 999999.0f;
  maximum = -999999.0f;
  int column = rectangle[0];

  while (column <= rectangle[2]) {
    int row = rectangle[1];

    while (row <= rectangle[3]) {
      const auto index =
        static_cast<unsigned>(column) + static_cast<unsigned>(row) * static_cast<unsigned>(this->width);
      const GroundQuad* top = this->cells[index].top;

      if (top) {
        const float heights[4] = {top->vertices[0][1], top->vertices[1][1], top->vertices[2][1], top->vertices[3][1]};

        for (float value : heights) {
          if (value > maximum) {
            maximum = value;
          }
        }

        for (float value : heights) {
          if (minimum > value) {
            minimum = value;
          }
        }
      }

      row = std::bit_cast<int>(static_cast<unsigned>(row) + 1u);
    }

    column = std::bit_cast<int>(static_cast<unsigned>(column) + 1u);
  }
}
