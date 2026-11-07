#include "Render/Primitives/DepthAlpha.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/TextureQuad.h"

__declspec(noinline) std::uint32_t DepthAlpha(float reciprocalDepth);

// OFFSET: 0x0055D190
// FUNCTION: fn_02100
std::uint32_t DepthAlpha(float reciprocalDepth) {
  float value = ((1.0f / reciprocalDepth) - 10.0f) / 1490.0f;
  const float range = g_depthEnd - g_depthStart;

  if (range != 0.0f) {
    value = (value - g_depthStart) / range;
  }

  const float lower = 0.0f > value ? 0.0f : value;
  const float upper = 1.0f < lower ? 1.0f : lower;
  const float alpha = (1.0f - upper) * 255.0f;

  return static_cast<std::uint32_t>(alpha) << 24;
}

// OFFSET: 0x005694A0
// FUNCTION: fn_02260
void RenderCommand::ApplyDepthAlpha() {
  if (this->vertexCount > 0) {
    for (int index = 0; index < this->vertexCount; ++index) {
      this->vertices[index].specular = DepthAlpha(this->vertices[index].rhw);
    }
  }
}

// OFFSET: 0x005694E0
// FUNCTION: fn_02261
void LightmappedPrimitive::ApplyDepthAlpha() {
  if (this->vertexCount > 0) {
    for (int index = 0; index < this->vertexCount; ++index) {
      this->vertices[index].specular = DepthAlpha(this->vertices[index].rhw);
    }
  }
}
