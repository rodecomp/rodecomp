#ifndef RAGNAROK_SOURCE_UTILITY_RADIALGRID_H
#define RAGNAROK_SOURCE_UTILITY_RADIALGRID_H

#include <vector>

struct RadialGridPoint {
  /* 0000 */ float x;
  /* 0004 */ float y;
};

std::vector<RadialGridPoint> GenerateRadialGrid(const RadialGridPoint& center, int radius);

#endif
