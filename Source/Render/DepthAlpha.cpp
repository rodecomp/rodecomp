#include "Render/DepthAlpha.h"
#include "Render/LightmappedPrimitive.h"
#include "Render/RenderPrimitive.h"

#include <bit>
#include <emmintrin.h>

// OFFSET: 0x0055D190
// FUNCTION: fn_02100
std::uint32_t DepthAlpha(float reciprocalDepth, float start, float end) {
  float value = ((1.0f / reciprocalDepth) - 10.0f) / 1490.0f;
  const float range = end - start;

  if (range != 0.0f) {
    value = (value - start) / range;
  }

  const __m128 lower = _mm_max_ss(_mm_setzero_ps(), _mm_set_ss(value));
  const float upper = _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(1.0f), lower));
  const float alpha = (1.0f - upper) * 255.0f;
  const auto bits = std::bit_cast<std::uint32_t>(alpha);
  const auto magnitude = (bits & 0x7FFFFFFFu) << 1;
  std::uint32_t converted;

  // Original __ftoui3 unsigned invalid-conversion results, not signed CVTTSS2SI.
  if ((bits & 0x80000000u) == 0 && magnitude >= 0x9F000000u) {
    converted = 0xFFFFFFFFu;
  } else if ((bits & 0x80000000u) != 0 && magnitude >= 0x7F000000u) {
    converted = 0xFFFFFFFFu;
  } else {
    converted = static_cast<std::uint32_t>(_mm_cvttss_si32(_mm_set_ss(alpha)));
  }

  return converted << 24;
}

// OFFSET: 0x005694A0
// FUNCTION: fn_02260
void ApplyDepthAlpha(RenderCommand& command, float start, float end) {
  if (std::bit_cast<std::int32_t>(command.vertexCount) > 0) {
    for (std::uint32_t index = 0; index < command.vertexCount; ++index) {
      command.vertices[index].specular = DepthAlpha(command.vertices[index].rhw, start, end);
    }
  }
}

// OFFSET: 0x005694E0
// FUNCTION: fn_02261
void ApplyDepthAlpha(LightmappedPrimitive& command, float start, float end) {
  if (std::bit_cast<std::int32_t>(command.vertexCount) > 0) {
    for (std::uint32_t index = 0; index < command.vertexCount; ++index) {
      command.vertices[index].specular = DepthAlpha(command.vertices[index].rhw, start, end);
    }
  }
}
