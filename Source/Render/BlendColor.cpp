#include "Render/BlendColor.h"

// OFFSET: 0x0055CFC0
// FUNCTION: fn_02095
std::uint32_t BlendColor(std::uint32_t base, std::uint32_t overlay) {
  const float alpha = static_cast<float>(overlay >> 24) / 255.0f;
  const float inverse = 1.0f - alpha;
  const auto channel = [](std::uint32_t color, unsigned shift) { return static_cast<float>((color >> shift) & 255u); };
  const unsigned red = static_cast<unsigned>(channel(base, 16) * inverse + channel(overlay, 16) * alpha);
  const unsigned green = static_cast<unsigned>(channel(overlay, 8) * alpha + channel(base, 8) * inverse);
  const unsigned blue = static_cast<unsigned>(channel(base, 0) * inverse + channel(overlay, 0) * alpha);
  return (base & 0xFF000000u) | ((red & 255u) << 16) | ((green & 255u) << 8) | (blue & 255u);
}
