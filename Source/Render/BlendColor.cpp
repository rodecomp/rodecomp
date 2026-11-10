#include "Render/BlendColor.h"

// OFFSET: 0x0055CFC0
// FUNCTION: fn_02095
std::uint32_t CRenderer::BlendColor(std::uint32_t base, std::uint32_t overlay) {
  std::uint32_t result = base;
  const float alpha = static_cast<float>(static_cast<int>(overlay >> 24)) / 255.0f;
  const float inverse = 1.0f - alpha;
  auto* channels = reinterpret_cast<unsigned char*>(&result);
  channels[2] = static_cast<unsigned char>(static_cast<int>(static_cast<float>(channels[2]) * inverse + static_cast<float>(static_cast<unsigned char>(overlay >> 16)) * alpha));
  channels[1] = static_cast<unsigned char>(static_cast<int>(static_cast<float>(static_cast<unsigned char>(overlay >> 8)) * alpha + static_cast<float>(channels[1]) * inverse));
  channels[0] = static_cast<unsigned char>(static_cast<int>(static_cast<float>(channels[0]) * inverse + static_cast<float>(static_cast<unsigned char>(overlay)) * alpha));

  return result;
}
