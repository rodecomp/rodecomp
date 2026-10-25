#include "Render/Primitives/DepthAlpha.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"

#include <cmath>

// OFFSET: 0x0055D190
// FUNCTION: fn_02100
std::uint32_t DepthAlpha(float reciprocalDepth, float start, float end) {
  float value = ((1.0f / reciprocalDepth) - 10.0f) / 1490.0f;
  const float range = end - start;

  if (range != 0.0f) {
    value = (value - start) / range;
  }

  const float lower = 0.0f > value ? 0.0f : value;
  const float upper = 1.0f < lower ? 1.0f : lower;
  const float alpha = (1.0f - upper) * 255.0f;
  const std::uint32_t converted = std::isnan(alpha) ? UINT32_MAX : static_cast<std::uint32_t>(alpha);

  return converted << 24;
}

// OFFSET: 0x005694A0
// FUNCTION: fn_02260
void ApplyDepthAlpha(RenderCommand& command, float start, float end) {
  if (command.vertexCount > 0) {
    for (int index = 0; index < command.vertexCount; ++index) {
      command.vertices[index].specular = DepthAlpha(command.vertices[index].rhw, start, end);
    }
  }
}

// OFFSET: 0x005694E0
// FUNCTION: fn_02261
void ApplyDepthAlpha(LightmappedPrimitive& command, float start, float end) {
  if (command.vertexCount > 0) {
    for (int index = 0; index < command.vertexCount; ++index) {
      command.vertices[index].specular = DepthAlpha(command.vertices[index].rhw, start, end);
    }
  }
}
