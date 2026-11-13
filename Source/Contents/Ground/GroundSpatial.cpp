#include "Contents/Ground/GroundGeometry.h"

#include <cmath>

// OFFSET: 0x00ABD8F0
// FUNCTION: fn_17548
GroundQuad::GroundQuad() {
  int remaining = 4;
  float (*vertex)[8] = this->vertices;
  do {
    (*vertex)[0] = 0.0f;
    (*vertex)[1] = 0.0f;
    (*vertex)[2] = 0.0f;
    ++vertex;
  } while (--remaining);
  this->texture = nullptr;
  this->selected = 0;
}

// OFFSET: 0x00ABD8A0
// FUNCTION: fn_17547
GroundCell::GroundCell() {
  float (*corner)[3] = this->corners;
  int remaining = 4;
  do {
    (*corner)[0] = 0.0f;
    (*corner)[1] = 0.0f;
    (*corner)[2] = 0.0f;
    ++corner;
  } while (--remaining);
  this->top = nullptr;
  this->side1 = nullptr;
  this->side2 = nullptr;
  this->color[0] = this->color[1] = this->color[2] = 255;
}

// OFFSET: 0x00AC0EA0
// FUNCTION: fn_17573
void GroundGeometry::HeightBounds(const int* rectangle, float& minimum, float& maximum) const {
  bool outside = false;

  if (rectangle[0] < 0 || rectangle[0] >= this->width) {
    outside = true;
  }

  if (rectangle[2] < 0 || rectangle[2] >= this->width) {
    outside = true;
  }

  if (rectangle[1] < 0 || rectangle[1] >= this->height) {
    outside = true;
  }

  if (rectangle[3] < 0 || rectangle[3] >= this->height) {
    outside = true;
  }

  if (rectangle[2] <= rectangle[0] || rectangle[3] <= rectangle[1] || outside) {
    minimum = 0.0f;
    maximum = 0.0f;
    return;
  }

  minimum = 999999.0f;
  maximum = -999999.0f;

  for (int column = rectangle[0]; column <= rectangle[2]; ++column) {
    for (int row = rectangle[1]; row <= rectangle[3]; ++row) {
      const GroundQuad* top = this->cells[column + row * this->width].top;

      if (top) {
        const float height0 = top->vertices[0][1];
        const float height1 = top->vertices[1][1];
        const float height2 = top->vertices[2][1];
        const float height3 = top->vertices[3][1];

        if (height0 > maximum) {
          maximum = height0;
        }

        if (height1 > maximum) {
          maximum = height1;
        }

        if (height2 > maximum) {
          maximum = height2;
        }

        if (height3 > maximum) {
          maximum = height3;
        }

        if (minimum > height0) {
          minimum = height0;
        }

        if (minimum > height1) {
          minimum = height1;
        }

        if (minimum > height2) {
          minimum = height2;
        }

        if (minimum > height3) {
          minimum = height3;
        }
      }
    }
  }
}
