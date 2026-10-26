#include "Render/Renderer.h"

// FUNCTION: fn_02104
float* CRenderer::ProjectViewport(const float* position, const float* matrix, float* x, float* y, float* reciprocalDepth) {
  const float inverse = 1.0f / (((matrix[2] * position[0] + matrix[5] * position[1]) + matrix[8] * position[2]) + matrix[11]);
  *reciprocalDepth = inverse;
  *x = (((((matrix[3] * position[1] + position[0] * matrix[0]) + matrix[6] * position[2]) + matrix[9]) * inverse) * this->viewport.scaleX) +
       static_cast<float>(this->viewport.clipCenterX);
  *y = (((((matrix[1] * position[0] + matrix[4] * position[1]) + matrix[7] * position[2]) + matrix[10]) * *reciprocalDepth) * this->viewport.scaleY) +
       static_cast<float>(this->viewport.clipCenterY);
  return y;
}

// FUNCTION: fn_02105
float* CRenderer::ProjectViewportCentered(
  const float* position, const float* matrix, float* x, float* y, float* reciprocalDepth, float width, float height
) {
  const float inverse = 1.0f / (((matrix[5] * position[1] + matrix[2] * position[0]) + matrix[8] * position[2]) + matrix[11]);
  *reciprocalDepth = inverse;
  *x = (((((matrix[3] * position[1] + matrix[0] * position[0]) + matrix[6] * position[2]) + matrix[9]) * inverse) * this->viewport.scaleX) + width * 0.5f;
  *y = (((((matrix[4] * position[1] + matrix[1] * position[0]) + matrix[7] * position[2]) + matrix[10]) * *reciprocalDepth) * this->viewport.scaleY) +
       height * 0.5f;
  return y;
}
