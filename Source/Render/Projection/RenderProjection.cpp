#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Renderer.h"

#include <bit>

// GLOBAL: gl_25972
float g_projectionDepthNumerator = 1.0f;
// GLOBAL: gl_25973
float g_projectionDepthOffset = 10.0f;
// GLOBAL: gl_13420
float g_projectionDepthScale = 1.0067114f;

// OFFSET: 0x0055D650
// FUNCTION: fn_02106
void CRenderer::Project(const float* position, const float* matrix, RenderVertex& output) const {
  const float numerator = g_projectionDepthNumerator;
  const float inverseDepth = numerator / (((matrix[5] * position[1] + matrix[2] * position[0]) + matrix[8] * position[2]) + matrix[11]);
  output.rhw = inverseDepth;

  const float depth = ((numerator / inverseDepth - g_projectionDepthOffset) * g_projectionDepthScale) * inverseDepth;
  const float x = ((((matrix[3] * position[1] + position[0] * matrix[0]) + matrix[6] * position[2]) + matrix[9]) * inverseDepth) * this->viewport.scaleX;
  output.x = x + static_cast<float>(this->viewport.centerX);

  const float y = ((((matrix[4] * position[1] + matrix[1] * position[0]) + matrix[7] * position[2]) + matrix[10]) * inverseDepth) * this->viewport.scaleY;
  output.y = y + static_cast<float>(this->viewport.centerY);
  output.z = depth;
}

// OFFSET: 0x0055D750
// FUNCTION: fn_02107
void CRenderer::Project(const float* position, const float* matrix, LightmappedVertex& output) const {
  const float numerator = g_projectionDepthNumerator;
  const float inverseDepth = numerator / (((matrix[5] * position[1] + matrix[2] * position[0]) + matrix[8] * position[2]) + matrix[11]);
  output.rhw = inverseDepth;

  const float depth = ((numerator / inverseDepth - g_projectionDepthOffset) * g_projectionDepthScale) * inverseDepth;
  const float x = ((((matrix[3] * position[1] + position[0] * matrix[0]) + matrix[6] * position[2]) + matrix[9]) * inverseDepth) * this->viewport.scaleX;
  output.x = x + static_cast<float>(this->viewport.centerX);

  const float y = ((((matrix[4] * position[1] + matrix[1] * position[0]) + matrix[7] * position[2]) + matrix[10]) * inverseDepth) * this->viewport.scaleY;
  output.y = y + static_cast<float>(this->viewport.centerY);
  output.z = depth;
}
