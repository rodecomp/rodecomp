#include "Render/Projection/RenderProjection.h"
#include "Render/Renderer.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x0055A090
// FUNCTION: fn_02076
void CRenderer::Ray(int x, int y, const float* origin, const float* matrix, float* output) const {
  float transform[12];
  std::memcpy(transform, matrix, sizeof(transform));
  const float horizontal = static_cast<float>(x) / static_cast<float>(this->viewport.centerX);
  const float vertical = static_cast<float>(y) / static_cast<float>(this->viewport.centerY);
  output[0] = origin[0];
  output[1] = origin[1];
  output[2] = origin[2];

  const float depth = g_projectionDepthNumerator;
  const float dy = (static_cast<float>(this->viewport.centerY) * -(depth - vertical)) / this->viewport.scaleY;
  const float dx = (static_cast<float>(this->viewport.centerX) * (horizontal - depth)) / this->viewport.scaleX;
  const float directionX = ((dy * transform[3] + dx * transform[0]) + transform[6]) + 0.0f;
  output[3] = directionX;
  const float directionY = ((dx * transform[1] + dy * transform[4]) + transform[7]) + 0.0f;
  output[4] = directionY;
  const float directionZ = ((transform[5] * dy + dx * transform[2]) + transform[8]) + 0.0f;
  output[5] = directionZ;

  const float inverseLength = 1.0f / SquareRoot((directionY * directionY + directionX * directionX) + directionZ * directionZ);
  output[3] = inverseLength * output[3];
  output[4] = inverseLength * output[4];
  output[5] = inverseLength * output[5];
}
