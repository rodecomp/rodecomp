#include "Contents/MovementRouteInterpolation.h"

// FUNCTION: fn_17876
int MovementRouteInterpolation::Sample(std::uint32_t tick, float* x, float* y, int* metadata) {
  if (this->points.empty()) {
    return -1;
  }

  if (this->points.front().tick > tick) {
    *x = static_cast<float>(this->points.front().x);
    *y = static_cast<float>(this->points.front().y);
    *metadata = this->points[1].metadata;
    return 0;
  }

  std::uint32_t index = this->segment;
  const std::uint32_t last = static_cast<std::uint32_t>(this->points.size()) - 1u;
  while (index < last) {
    if (this->points[index].tick <= tick && tick < this->points[index + 1].tick) {
      break;
    }

    ++index;
  }

  if (index >= last) {
    *metadata = this->points[index].metadata;
    *x = static_cast<float>(this->points[index].x);
    *y = static_cast<float>(this->points[index].y);
    return -1;
  }

  const float elapsed = static_cast<float>(static_cast<std::int32_t>(tick - this->points[index].tick));
  float originX;
  float deltaX;
  float deltaY;
  if (index == 0) {
    originX = this->startX;
    deltaX = static_cast<float>(this->points[index + 1].x) - originX;
    deltaY = static_cast<float>(this->points[index + 1].y) - this->startY;
  } else {
    originX = static_cast<float>(this->points[index].x);
    deltaX = static_cast<float>(this->points[index + 1].x - this->points[index].x);
    deltaY = static_cast<float>(this->points[index + 1].y - this->points[index].y);
  }

  const float duration = static_cast<float>(this->points[index + 1].tick - this->points[index].tick);
  const float offsetY = elapsed * deltaY;
  *x = originX + (deltaX * elapsed) / duration;
  const float yDuration = static_cast<float>(this->points[index + 1].tick - this->points[index].tick);
  const float originY = index == 0 ? this->startY : static_cast<float>(this->points[index].y);
  *y = originY + offsetY / yDuration;

  const std::uint32_t midpoint = this->points[index].tick + ((this->points[index + 1].tick - this->points[index].tick) >> 1);
  *metadata = tick < midpoint ? this->points[index].metadata : this->points[index + 1].metadata;
  this->segment = index;
  return static_cast<int>(index);
}

// FUNCTION: fn_17877
std::uint32_t MovementRouteInterpolation::Segment() const {
  return this->segment;
}
