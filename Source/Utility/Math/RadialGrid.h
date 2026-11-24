#ifndef RAGNAROK_SOURCE_UTILITY_MATH_RADIALGRID_H
#define RAGNAROK_SOURCE_UTILITY_MATH_RADIALGRID_H

#include <vector>

struct RadialGridPoint {
  RadialGridPoint() = default;
  RadialGridPoint(float pointX, float pointY) : x(pointX), y(pointY) {}

  /* 0000 */ float x;
  /* 0004 */ float y;
};

std::vector<RadialGridPoint> GenerateRadialGrid(const RadialGridPoint& center, int radius);

#endif
