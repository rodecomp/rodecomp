#include "Contents/Models/ModelActor.h"

#include <cmath>

namespace {
  // INLINED
  float Absolute(float value) {
    return std::fabs(value);
  }

  // OFFSET: 0x00553850
  // FUNCTION: fn_02019
  bool Intersects(const float* ray, const OrientedBounds& bounds) {
    const float delta[3] = {bounds.center[0] - ray[0], bounds.center[1] - ray[1], bounds.center[2] - ray[2]};
    float near = -999999.0f;
    float far = 999999.0f;

    for (unsigned axis = 0; axis < 3; ++axis) {
      const float* direction = bounds.axes[axis];
      const float distance = (delta[1] * direction[1] + delta[0] * direction[0]) + delta[2] * direction[2];
      const float along = (ray[3] * direction[0] + ray[4] * direction[1]) + ray[5] * direction[2];
      const float radius = bounds.halfSize[axis];

      if (Absolute(along) <= 0.00001f) {
        if (-distance - radius > 0.0f || radius - distance < 0.0f) {
          return false;
        }
      } else {
        const float inverse = 1.0f / along;
        const float first = (radius + distance) * inverse;
        const float second = (distance - radius) * inverse;
        const float lower = first <= second ? first : second;
        const float upper = first <= second ? second : first;

        near = lower > near ? lower : near;
        far = upper < far ? upper : far;

        if (near > far || far < 0.0f) {
          return false;
        }
      }
    }

    return near <= 0.0f ? far <= ray[6] : near <= ray[6];
  }
}

// OFFSET: 0x00AB7810
// FUNCTION: fn_17487
void C3dActor::MarkOccluded(const float* ray) {
  if (this->occlusionEnabled == 1 && Intersects(ray, this->oriented)) {
    this->occluded = 1;
  }
}
