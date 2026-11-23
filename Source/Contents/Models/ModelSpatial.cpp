#include "Contents/Models/ModelSpatial.h"

#include <cmath>

namespace {
  // INLINED
  float Absolute(float value) {
    return std::fabs(value);
  }

  constexpr double tolerance = 0.000001;

  // INLINED
  bool IsSeparated(float projected, float extent, float halfSize) {
    return static_cast<double>(Absolute(projected)) - (static_cast<double>(extent) + static_cast<double>(halfSize)) > tolerance;
  }
}

// OFFSET: 0x00ACAD10
// FUNCTION: fn_17702
bool IntersectsBounds(const OrientedBounds& box, const float* bounds) {
  const float delta[3] = {
    box.center[0] - (bounds[0] + bounds[3]) * 0.5f, box.center[1] - (bounds[1] + bounds[4]) * 0.5f, box.center[2] - (bounds[2] + bounds[5]) * 0.5f
  };
  const float extents[3] = {(bounds[3] - bounds[0]) * 0.5f, (bounds[4] - bounds[1]) * 0.5f, (bounds[5] - bounds[2]) * 0.5f};
  float projected[3];
  float absolute[3][3];

  absolute[0][0] = Absolute(box.axes[0][0]);
  absolute[0][1] = Absolute(box.axes[0][1]);
  absolute[0][2] = Absolute(box.axes[0][2]);
  projected[0] = (box.axes[0][1] * delta[1] + box.axes[0][0] * delta[0]) + box.axes[0][2] * delta[2];
  if (IsSeparated(projected[0], (absolute[0][1] * extents[1] + absolute[0][0] * extents[0]) + absolute[0][2] * extents[2], box.halfSize[0])) {
    return false;
  }

  absolute[1][0] = Absolute(box.axes[1][0]);
  absolute[1][1] = Absolute(box.axes[1][1]);
  absolute[1][2] = Absolute(box.axes[1][2]);
  projected[1] = (box.axes[1][1] * delta[1] + box.axes[1][0] * delta[0]) + box.axes[1][2] * delta[2];
  if (IsSeparated(projected[1], (absolute[1][1] * extents[1] + absolute[1][0] * extents[0]) + absolute[1][2] * extents[2], box.halfSize[1])) {
    return false;
  }

  absolute[2][0] = Absolute(box.axes[2][0]);
  absolute[2][1] = Absolute(box.axes[2][1]);
  absolute[2][2] = Absolute(box.axes[2][2]);
  projected[2] = (box.axes[2][1] * delta[1] + box.axes[2][0] * delta[0]) + box.axes[2][2] * delta[2];
  if (IsSeparated(projected[2], (absolute[2][1] * extents[1] + absolute[2][0] * extents[0]) + absolute[2][2] * extents[2], box.halfSize[2])) {
    return false;
  }

  if (IsSeparated(delta[0], (box.halfSize[1] * absolute[1][0] + box.halfSize[0] * absolute[0][0]) + box.halfSize[2] * absolute[2][0], extents[0])) {
    return false;
  }

  if (IsSeparated(delta[1], (box.halfSize[1] * absolute[1][1] + box.halfSize[0] * absolute[0][1]) + box.halfSize[2] * absolute[2][1], extents[1])) {
    return false;
  }

  if (IsSeparated(delta[2], (box.halfSize[1] * absolute[1][2] + box.halfSize[0] * absolute[0][2]) + box.halfSize[2] * absolute[2][2], extents[2])) {
    return false;
  }

  if (IsSeparated(box.axes[1][0] * projected[2] - box.axes[2][0] * projected[1],
                  box.halfSize[2] * absolute[1][0] + box.halfSize[1] * absolute[2][0],
                  absolute[0][1] * extents[2] + absolute[0][2] * extents[1])) {
    return false;
  }

  if (IsSeparated(box.axes[1][1] * projected[2] - box.axes[2][1] * projected[1],
                  box.halfSize[2] * absolute[1][1] + box.halfSize[1] * absolute[2][1],
                  absolute[0][2] * extents[0] + absolute[0][0] * extents[2])) {
    return false;
  }

  if (IsSeparated(box.axes[1][2] * projected[2] - box.axes[2][2] * projected[1],
                  box.halfSize[2] * absolute[1][2] + box.halfSize[1] * absolute[2][2],
                  absolute[0][0] * extents[1] + absolute[0][1] * extents[0])) {
    return false;
  }

  if (IsSeparated(box.axes[2][0] * projected[0] - box.axes[0][0] * projected[2],
                  absolute[1][1] * extents[2] + absolute[1][2] * extents[1],
                  box.halfSize[0] * absolute[2][0] + box.halfSize[2] * absolute[0][0])) {
    return false;
  }

  if (IsSeparated(box.axes[2][1] * projected[0] - box.axes[0][1] * projected[2],
                  absolute[1][2] * extents[0] + absolute[1][0] * extents[2],
                  box.halfSize[0] * absolute[2][1] + box.halfSize[2] * absolute[0][1])) {
    return false;
  }

  if (IsSeparated(box.axes[2][2] * projected[0] - box.axes[0][2] * projected[2],
                  absolute[1][0] * extents[1] + absolute[1][1] * extents[0],
                  box.halfSize[0] * absolute[2][2] + box.halfSize[2] * absolute[0][2])) {
    return false;
  }

  if (IsSeparated(box.axes[0][0] * projected[1] - box.axes[1][0] * projected[0],
                  box.halfSize[0] * absolute[1][0] + box.halfSize[1] * absolute[0][0],
                  absolute[2][1] * extents[2] + absolute[2][2] * extents[1])) {
    return false;
  }

  if (IsSeparated(box.axes[0][1] * projected[1] - box.axes[1][1] * projected[0],
                  box.halfSize[0] * absolute[1][1] + box.halfSize[1] * absolute[0][1],
                  absolute[2][2] * extents[0] + absolute[2][0] * extents[2])) {
    return false;
  }

  if (IsSeparated(box.axes[0][2] * projected[1] - box.axes[1][2] * projected[0],
                  box.halfSize[0] * absolute[1][2] + box.halfSize[1] * absolute[0][2],
                  absolute[2][0] * extents[1] + absolute[2][1] * extents[0])) {
    return false;
  }

  return true;
}
