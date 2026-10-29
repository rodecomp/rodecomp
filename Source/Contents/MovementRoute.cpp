#include "Contents/MovementRouteInterpolation.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00AD7F70
// FUNCTION: fn_17861
MovementRouteInterpolation::MovementRouteInterpolation() : segment(0), startX(0), startY(0) {
}

// OFFSET: 0x00AD87A0
// FUNCTION: fn_17867
void BuildMovementRoute(std::uint32_t tick, const PathNode* last, int stepDuration, MovementRouteInterpolation& route) {
  std::uint32_t count = 0;
  for (const PathNode* node = last; node; node = node->parent) {
    ++count;
  }

  route.points.resize(count);
  if (static_cast<int>(count) > 1) {
    const PathNode* node = last;
    for (std::uint32_t index = count; index != 0;) {
      --index;
      route.points[index].x = node->x;
      route.points[index].y = node->y;
      route.points[index].metadata = node->metadata;
      node = node->parent;
    }

    const int diagonalDuration = static_cast<int>(static_cast<float>(stepDuration) * 1.414f);
    route.points[0].tick = tick;
    const float deltaY = static_cast<float>(route.points[1].y) - route.startY;
    const float deltaX = static_cast<float>(route.points[1].x) - route.startX;
    const float initialDuration = SquareRoot(deltaY * deltaY + deltaX * deltaX) * static_cast<float>(stepDuration);
    route.points[1].tick = tick + static_cast<std::uint32_t>(initialDuration);
    for (std::uint32_t index = 2; index < count; ++index) {
      const int duration = route.points[index].metadata & 1 ? diagonalDuration : stepDuration;
      route.points[index].tick = route.points[index - 1].tick + duration;
    }
  }
}

// OFFSET: 0x00AD8D20
// FUNCTION: fn_17869
bool MovementRouteInterpolation::Point(int index, std::int32_t* x, std::int32_t* y) const {
  if (this->points.empty() || index < 0 || static_cast<unsigned>(index) >= this->points.size()) {
    return false;
  }

  *x = this->points[index].x;
  *y = this->points[index].y;
  return true;
}

// OFFSET: 0x00AD8D70
// FUNCTION: fn_17870
std::uint32_t* MovementRouteInterpolation::EndTick(std::uint32_t* tick) const {
  *tick = this->points.back().tick;
  return tick;
}

// OFFSET: 0x00AD8D90
// FUNCTION: fn_17871
int MovementRouteInterpolation::EndMetadata() const {
  if (this->points.empty()) {
    return -1;
  }

  return this->points.back().metadata;
}

// OFFSET: 0x00AD8F20
// FUNCTION: fn_17874
int MovementRouteInterpolation::NextPoint(std::uint32_t tick, std::uint32_t* nextTick, float* x, float* y) const {
  if (this->points.empty() || this->points.front().tick == 0) {
    return -1;
  }

  if (this->points.front().tick > tick) {
    *nextTick = this->points[1].tick;
    *x = static_cast<float>(this->points[1].x);
    *y = static_cast<float>(this->points[1].y);
    return 0;
  }

  int index = 0;
  const int last = static_cast<int>(this->points.size()) - 1;
  while (index < last) {
    if (this->points[index].tick <= tick && tick < this->points[index + 1].tick) {
      *nextTick = this->points[index + 1].tick;
      *x = static_cast<float>(this->points[index + 1].x);
      *y = static_cast<float>(this->points[index + 1].y);
      return index;
    }

    ++index;
  }

  *nextTick = this->points[index].tick;
  *x = static_cast<float>(this->points[index].x);
  *y = static_cast<float>(this->points[index].y);
  return -1;
}
