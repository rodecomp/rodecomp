#include "Utility/Math/RadialGrid.h"

#include <cstdint>

// OFFSET: 0x00AF61A0
// FUNCTION: fn_18251
std::vector<RadialGridPoint> GenerateRadialGrid(const RadialGridPoint& center, int radius) {
  std::vector<RadialGridPoint> points;
  const std::uint32_t diameter = static_cast<std::uint32_t>(radius) * 2u + 1u;
  points.reserve(diameter * diameter);
  float pointX;
  float pointY;

  for (int x = -radius; x <= radius; ++x) {
    for (int y = -radius; y <= radius; ++y) {
      pointX = static_cast<float>(x) + center.x;
      pointY = static_cast<float>(y) + center.y;
      points.emplace_back(pointX, pointY);
    }
  }

  return points;
}
