#include "Render/Projection/RenderProjection.h"

#include <bit>
#include <cmath>
#include <cstring>

namespace {
  // INLINED
  float InverseLength(float squared) {
    const float length = std::sqrt(squared);
    return 1.0f / length;
  }
}

// OFFSET: 0x0055A090
// FUNCTION: fn_02076
void RenderProjection::Ray(int x, int y, const float* origin, const float* matrix, float* output) const {
  float transform[12];
  std::memcpy(transform, matrix, sizeof(transform));
  const float horizontal = static_cast<float>(x) / static_cast<float>(this->centerX);
  output[0] = origin[0];
  output[1] = origin[1];
  output[2] = origin[2];

  const float vertical = 1.0f - static_cast<float>(y) / static_cast<float>(this->centerY);
  const float negative = -vertical;
  const float dy = (static_cast<float>(this->centerY) * negative) / this->scaleY;
  const float dx = (static_cast<float>(this->centerX) * (horizontal - 1.0f)) / this->scaleX;
  const float first = ((dy * transform[3] + dx * transform[0]) + transform[6]) + 0.0f;
  output[3] = first;
  const float second = ((dx * transform[1] + dy * transform[4]) + transform[7]) + 0.0f;
  output[4] = second;
  const float third = ((transform[5] * dy + dx * transform[2]) + transform[8]) + 0.0f;
  output[5] = third;

  const float inverse = InverseLength((second * second + first * first) + third * third);
  output[3] = inverse * output[3];
  const float last = inverse * output[5];
  output[4] = inverse * output[4];
  output[5] = last;
}
