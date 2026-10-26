#include "Utility/RadialGrid.h"

#include <cstdint>

// FUNCTION: fn_18251
std::vector<RadialGridPoint> GenerateRadialGrid(const RadialGridPoint& center, int radius) {
  std::vector<RadialGridPoint> points;
  const std::uint32_t diameter = static_cast<std::uint32_t>(radius) * 2u + 1u;
  points.reserve(diameter * diameter);

  for (int x = -radius; x <= radius; ++x) {
    for (int y = -radius; y <= radius; ++y) {
      points.push_back({static_cast<float>(x) + center.x, static_cast<float>(y) + center.y});
    }
  }

  return points;
}
