#include "Render/RenderPrimitive.h"
#include "Render/RenderProjection.h"

#include <bit>

// OFFSET: 0x0055D650
// FUNCTION: fn_02106
void RenderProjection::Project(const float* position, const float* matrix, RenderVertex& output) const {
  const float inverseDepth =
    1.0f / (((matrix[5] * position[1] + matrix[2] * position[0]) + matrix[8] * position[2]) + matrix[11]);
  output.rhw = inverseDepth;

  // STATIC: 0x010DD22C
  // GLOBAL: gl_13420
  constexpr float depthScale = 1.0067114f;
  const float depth = ((1.0f / inverseDepth - 10.0f) * depthScale) * inverseDepth;
  const float x =
    ((((matrix[3] * position[1] + position[0] * matrix[0]) + matrix[6] * position[2]) + matrix[9]) * inverseDepth) *
    this->scaleX;
  output.x = x + static_cast<float>(this->centerX);

  const float centerY = static_cast<float>(this->centerY);
  const float y =
    ((((matrix[4] * position[1] + matrix[1] * position[0]) + matrix[7] * position[2]) + matrix[10]) * inverseDepth) *
    this->scaleY;
  output.z = depth;
  output.y = y + centerY;
}
