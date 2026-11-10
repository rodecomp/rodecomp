#include "Contents/Models/ModelSpatial.h"

#include <cmath>

namespace {
  // INLINED
  float Absolute(float value) {
    return std::fabs(value);
  }
}

// OFFSET: 0x00ACAD10
// FUNCTION: fn_17702
bool OrientedBounds::Intersects(const float* bounds) const {
  constexpr double tolerance = 0.000001;
  const float extents[3] = {(bounds[3] - bounds[0]) * 0.5f, (bounds[4] - bounds[1]) * 0.5f, (bounds[5] - bounds[2]) * 0.5f};
  const float delta[3] = {
    this->center[0] - (bounds[0] + bounds[3]) * 0.5f, this->center[1] - (bounds[1] + bounds[4]) * 0.5f, this->center[2] - (bounds[2] + bounds[5]) * 0.5f
  };
  float projected[3];
  float absolute[3][3];

  projected[0] = (this->axes[0][1] * delta[1] + this->axes[0][0] * delta[0]) + this->axes[0][2] * delta[2];
  absolute[0][0] = Absolute(this->axes[0][0]);
  absolute[0][1] = Absolute(this->axes[0][1]);
  absolute[0][2] = Absolute(this->axes[0][2]);
  if (static_cast<double>(Absolute(projected[0])) -
        (static_cast<double>((absolute[0][1] * extents[1] + absolute[0][0] * extents[0]) + absolute[0][2] * extents[2]) + this->halfSize[0]) >
      tolerance) {
    return false;
  }

  projected[1] = (this->axes[1][1] * delta[1] + this->axes[1][0] * delta[0]) + this->axes[1][2] * delta[2];
  absolute[1][0] = Absolute(this->axes[1][0]);
  absolute[1][1] = Absolute(this->axes[1][1]);
  absolute[1][2] = Absolute(this->axes[1][2]);
  if (static_cast<double>(Absolute(projected[1])) -
        (static_cast<double>((absolute[1][1] * extents[1] + absolute[1][0] * extents[0]) + absolute[1][2] * extents[2]) + this->halfSize[1]) >
      tolerance) {
    return false;
  }

  projected[2] = (this->axes[2][1] * delta[1] + this->axes[2][0] * delta[0]) + this->axes[2][2] * delta[2];
  absolute[2][0] = Absolute(this->axes[2][0]);
  absolute[2][1] = Absolute(this->axes[2][1]);
  absolute[2][2] = Absolute(this->axes[2][2]);
  if (static_cast<double>(Absolute(projected[2])) -
        (static_cast<double>((absolute[2][1] * extents[1] + absolute[2][0] * extents[0]) + absolute[2][2] * extents[2]) + this->halfSize[2]) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(delta[0])) -
        (static_cast<double>((this->halfSize[1] * absolute[1][0] + this->halfSize[0] * absolute[0][0]) + this->halfSize[2] * absolute[2][0]) + extents[0]) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(delta[1])) -
        (static_cast<double>((this->halfSize[1] * absolute[1][1] + this->halfSize[0] * absolute[0][1]) + this->halfSize[2] * absolute[2][1]) + extents[1]) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(delta[2])) -
        (static_cast<double>((this->halfSize[1] * absolute[1][2] + this->halfSize[0] * absolute[0][2]) + this->halfSize[2] * absolute[2][2]) + extents[2]) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[1][0] * projected[2] - this->axes[2][0] * projected[1])) -
        (static_cast<double>(this->halfSize[2] * absolute[1][0] + this->halfSize[1] * absolute[2][0]) +
         static_cast<double>(absolute[0][1] * extents[2] + absolute[0][2] * extents[1])) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[1][1] * projected[2] - this->axes[2][1] * projected[1])) -
        (static_cast<double>(this->halfSize[2] * absolute[1][1] + this->halfSize[1] * absolute[2][1]) +
         static_cast<double>(absolute[0][2] * extents[0] + absolute[0][0] * extents[2])) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[1][2] * projected[2] - this->axes[2][2] * projected[1])) -
        (static_cast<double>(this->halfSize[2] * absolute[1][2] + this->halfSize[1] * absolute[2][2]) +
         static_cast<double>(absolute[0][0] * extents[1] + absolute[0][1] * extents[0])) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[2][0] * projected[0] - this->axes[0][0] * projected[2])) -
        (static_cast<double>(absolute[1][1] * extents[2] + absolute[1][2] * extents[1]) +
         static_cast<double>(this->halfSize[0] * absolute[2][0] + this->halfSize[2] * absolute[0][0])) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[2][1] * projected[0] - this->axes[0][1] * projected[2])) -
        (static_cast<double>(absolute[1][2] * extents[0] + absolute[1][0] * extents[2]) +
         static_cast<double>(this->halfSize[0] * absolute[2][1] + this->halfSize[2] * absolute[0][1])) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[2][2] * projected[0] - this->axes[0][2] * projected[2])) -
        (static_cast<double>(absolute[1][0] * extents[1] + absolute[1][1] * extents[0]) +
         static_cast<double>(this->halfSize[0] * absolute[2][2] + this->halfSize[2] * absolute[0][2])) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[0][0] * projected[1] - this->axes[1][0] * projected[0])) -
        (static_cast<double>(this->halfSize[0] * absolute[1][0] + this->halfSize[1] * absolute[0][0]) +
         static_cast<double>(absolute[2][1] * extents[2] + absolute[2][2] * extents[1])) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[0][1] * projected[1] - this->axes[1][1] * projected[0])) -
        (static_cast<double>(this->halfSize[0] * absolute[1][1] + this->halfSize[1] * absolute[0][1]) +
         static_cast<double>(absolute[2][2] * extents[0] + absolute[2][0] * extents[2])) >
      tolerance) {
    return false;
  }

  if (static_cast<double>(Absolute(this->axes[0][2] * projected[1] - this->axes[1][2] * projected[0])) -
        (static_cast<double>(this->halfSize[0] * absolute[1][2] + this->halfSize[1] * absolute[0][2]) +
         static_cast<double>(absolute[2][0] * extents[1] + absolute[2][1] * extents[0])) >
      tolerance) {
    return false;
  }

  return true;
}
