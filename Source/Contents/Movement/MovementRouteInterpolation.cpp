#include "Contents/Movement/MovementRouteInterpolation.h"

// OFFSET: 0x00AD92C0
// FUNCTION: fn_17876
int MovementRouteInterpolation::Sample(std::uint32_t tick, float* x, float* y, int* metadata) {
  if (this->points.empty()) {
    return -1;
  }

  const auto first = this->points.begin();
  if (first->tick > tick) {
    *x = static_cast<float>(first->x);
    *y = static_cast<float>(first->y);
    *metadata = (first + 1)->metadata;
    return 0;
  }

  std::uint32_t index = this->segment;
  for (; index < this->points.size() - 1; ++index) {
    if (this->points[index].tick <= tick && tick < this->points[index + 1].tick) {
      const float elapsed = static_cast<float>(static_cast<std::int32_t>(tick - this->points[index].tick));
      float originX;
      float deltaX;
      float deltaY;
      if (index == 0) {
        deltaY = static_cast<float>(this->points[1].y) - this->startY;
        originX = this->startX;
        deltaX = static_cast<float>(this->points[1].x) - originX;
      } else {
        deltaY = static_cast<float>(this->points[index + 1].y - this->points[index].y);
        originX = static_cast<float>(this->points[index].x);
        deltaX = static_cast<float>(this->points[index + 1].x - this->points[index].x);
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
  }

  *metadata = this->points[index].metadata;
  *x = static_cast<float>(this->points[index].x);
  *y = static_cast<float>(this->points[index].y);
  return -1;
}

// OFFSET: 0x00AD9500
// FUNCTION: fn_17877
std::uint32_t MovementRouteInterpolation::Segment() const {
  return this->segment;
}

// OFFSET: 0x00AD9510
// FUNCTION: fn_45898
float MovementRouteInterpolation::StartX() const {
  return this->startX;
}

// OFFSET: 0x00AD9520
// FUNCTION: fn_45899
float MovementRouteInterpolation::StartY() const {
  return this->startY;
}

// OFFSET: 0x00AD9910
// FUNCTION: fn_17887
void MovementRouteInterpolation::ResetSegment() {
  this->segment = 0;
}

// OFFSET: 0x00AD9930
// FUNCTION: fn_45900
void MovementRouteInterpolation::SetSegment(std::uint32_t segment) {
  this->segment = segment;
}

// OFFSET: 0x00AD9940
// FUNCTION: fn_45901
void MovementRouteInterpolation::SetStartX(float startX) {
  this->startX = startX;
}

// OFFSET: 0x00AD9960
// FUNCTION: fn_45902
void MovementRouteInterpolation::SetStartY(float startY) {
  this->startY = startY;
}
