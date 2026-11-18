#include "Render/Primitives/DepthAlpha.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/TextureQuad.h"

__declspec(noinline) std::uint32_t DepthAlpha(float reciprocalDepth);

// OFFSET: 0x0055D120
// FUNCTION: fn_45881
float DepthToRhw(float depth) {
  return (1500.0f - depth * 1490.0f) / 15000.0f;
}

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

// OFFSET: 0x0055D310
// FUNCTION: fn_02101
__declspec(noinline) float RhwToDepth(float rhw) {
  return ((1.0f / rhw - 10.0f) * 1.0067114f) * rhw;
}
