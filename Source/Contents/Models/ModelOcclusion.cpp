#include "Contents/Models/ModelActor.h"

#include <cmath>

namespace {
  // INLINED
  float Absolute(float value) {
    return std::fabs(value);
  }

  // INLINED
  __forceinline int ClipSlab(const float* ray, const float* delta, const float* direction, float radius, float& near, float& far) {
    const float distance = (delta[1] * direction[1] + delta[0] * direction[0]) + delta[2] * direction[2];
    const float along = (ray[3] * direction[0] + ray[4] * direction[1]) + ray[5] * direction[2];

    if (Absolute(along) > 0.00001f) {
      const float inverse = 1.0f / along;
      const float first = (radius + distance) * inverse;
      const float second = (distance - radius) * inverse;

      if (first > second) {
        near = second > near ? second : near;
        far = first < far ? first : far;
      } else {
        near = first > near ? first : near;
        far = second < far ? second : far;
      }

      if (near > far || far < 0.0f) {
        return 0;
      }
    } else if (-distance - radius > 0.0f || radius - distance < 0.0f) {
      return 0;
    }

    return 1;
  }
}

// OFFSET: 0x00553850
// FUNCTION: fn_02019
__declspec(noinline) int Intersects(const float* ray, const OrientedBounds& bounds) {
  const float delta[3] = {bounds.center[0] - ray[0], bounds.center[1] - ray[1], bounds.center[2] - ray[2]};
  float near = -999999.0f;
  float far = 999999.0f;

  if (!ClipSlab(ray, delta, bounds.axes[0], bounds.halfSize[0], near, far)) {
    return 0;
  }

  if (!ClipSlab(ray, delta, bounds.axes[1], bounds.halfSize[1], near, far)) {
    return 0;
  }

  if (!ClipSlab(ray, delta, bounds.axes[2], bounds.halfSize[2], near, far)) {
    return 0;
  }

  if (near > 0.0f) {
    if (near > ray[6]) {
      return 0;
    }
  } else if (far > ray[6]) {
    return 0;
  }

  return 1;
}

// OFFSET: 0x00AB7810
// FUNCTION: fn_17487
void C3dActor::MarkOccluded(const float* ray) {
  if (this->occlusionEnabled == 1 && Intersects(ray, this->oriented)) {
    this->occluded = 1;
  }
}
