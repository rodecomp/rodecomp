#include "Utility/Math/RadialGrid.h"
#include "UI/Layout/WindowCoordinates.h"

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

// OFFSET: 0x00AF64A0
// FUNCTION: fn_18253
bool WindowBoundsOverlap(const int first[4], const int second[4]) {
  return first[2] > second[0] && first[0] < second[2] && first[3] > second[1] && first[1] < second[3];
}
