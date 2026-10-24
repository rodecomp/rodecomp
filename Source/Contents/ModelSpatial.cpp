#include "Contents/ModelSpatial.h"

#include <bit>

namespace {
  float Absolute(float value) {
    return std::bit_cast<float>(std::bit_cast<unsigned>(value) & 0x7FFFFFFFu);
  }
}

// OFFSET: 0x00ACAD10
// FUNCTION: fn_17702
bool OrientedBounds::Intersects(const float* bounds) const {
  constexpr double tolerance = 0.000001;
  const float extents[3] = {
    (bounds[3] - bounds[0]) * 0.5f, (bounds[4] - bounds[1]) * 0.5f, (bounds[5] - bounds[2]) * 0.5f
  };
  const float delta[3] = {
    this->center[0] - (bounds[0] + bounds[3]) * 0.5f, this->center[1] - (bounds[1] + bounds[4]) * 0.5f,
    this->center[2] - (bounds[2] + bounds[5]) * 0.5f
  };
  float projected[3];
  float absolute[3][3];

  for (unsigned axis = 0; axis < 3; ++axis) {
    const float* direction = this->axes[axis];
    projected[axis] = (direction[1] * delta[1] + direction[0] * delta[0]) + direction[2] * delta[2];

    for (unsigned component = 0; component < 3; ++component) {
      absolute[axis][component] = Absolute(direction[component]);
    }

    const float radius =
      (absolute[axis][1] * extents[1] + absolute[axis][0] * extents[0]) + absolute[axis][2] * extents[2];

    if (
      static_cast<double>(Absolute(projected[axis])) - (static_cast<double>(radius) + this->halfSize[axis]) > tolerance
    ) {
      return false;
    }
  }

  for (unsigned component = 0; component < 3; ++component) {
    const float radius = (this->halfSize[1] * absolute[1][component] + this->halfSize[0] * absolute[0][component]) +
                         this->halfSize[2] * absolute[2][component];

    if (
      static_cast<double>(Absolute(delta[component])) - (static_cast<double>(radius) + extents[component]) > tolerance
    ) {
      return false;
    }
  }

  // Preserve the original nine cross-axis tests and the float-pair sums
  // promoted to double before the final radius addition/subtraction.
  const auto separated = [&](float distance, float first, float second) {
    return static_cast<double>(Absolute(distance)) - (static_cast<double>(first) + static_cast<double>(second)) >
           tolerance;
  };

  for (unsigned component = 0; component < 3; ++component) {
    const unsigned next = (component + 1) % 3;
    const unsigned last = (component + 2) % 3;
    const float distance = this->axes[1][component] * projected[2] - this->axes[2][component] * projected[1];
    const float modelRadius = this->halfSize[2] * absolute[1][component] + this->halfSize[1] * absolute[2][component];
    const float boxRadius = absolute[0][next] * extents[last] + absolute[0][last] * extents[next];

    if (separated(distance, modelRadius, boxRadius)) {
      return false;
    }
  }

  for (unsigned component = 0; component < 3; ++component) {
    const unsigned next = (component + 1) % 3;
    const unsigned last = (component + 2) % 3;
    const float distance = this->axes[2][component] * projected[0] - this->axes[0][component] * projected[2];
    const float modelRadius = this->halfSize[0] * absolute[2][component] + this->halfSize[2] * absolute[0][component];
    const float boxRadius = absolute[1][next] * extents[last] + absolute[1][last] * extents[next];

    if (separated(distance, boxRadius, modelRadius)) {
      return false;
    }
  }

  for (unsigned component = 0; component < 3; ++component) {
    const unsigned next = (component + 1) % 3;
    const unsigned last = (component + 2) % 3;
    const float distance = this->axes[0][component] * projected[1] - this->axes[1][component] * projected[0];
    const float modelRadius = this->halfSize[0] * absolute[1][component] + this->halfSize[1] * absolute[0][component];
    const float boxRadius = absolute[2][next] * extents[last] + absolute[2][last] * extents[next];

    if (separated(distance, modelRadius, boxRadius)) {
      return false;
    }
  }

  return true;
}
