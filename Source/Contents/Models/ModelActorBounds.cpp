#include "Contents/Models/ModelActor.h"
#include "Utility/Math/AffineMatrix.h"

#include <array>
#include <cmath>
#include <cstring>

namespace {
  // INLINED
  float Absolute(float value) {
    return std::fabs(value);
  }

  // INLINED
  float Radians(float degrees) {
    return (degrees / 180.0f) * 3.141592f;
  }

  // INLINED
  float InverseLength(float squared) {
    return 1.0f / SquareRoot(squared);
  }

  // INLINED
  __forceinline void TransformCorner(float* corner, float x, float y, float z, const float* matrix) {
    corner[0] = ((x * matrix[0] + y * matrix[3]) + z * matrix[6]) + matrix[9];
    corner[1] = ((x * matrix[1] + y * matrix[4]) + z * matrix[7]) + matrix[10];
    corner[2] = ((x * matrix[2] + y * matrix[5]) + z * matrix[8]) + matrix[11];
  }

  // INLINED
  __forceinline void SetAxis(float* direction, const float* row) {
    direction[0] = row[0];
    direction[1] = row[1];
    direction[2] = row[2];
    const float inverse = InverseLength((direction[0] * direction[0] + direction[1] * direction[1]) + direction[2] * direction[2]);
    direction[0] = inverse * direction[0];
    direction[1] = inverse * direction[1];
    direction[2] = inverse * direction[2];
  }

  struct BoundsCorner {
    float values[3];

    // INLINED
    BoundsCorner() {
      values[0] = 0.0f;
      values[1] = 0.0f;
      values[2] = 0.0f;
    }

    // INLINED
    __forceinline void Set(float x, float y, float z) {
      values[0] = x;
      values[1] = y;
      values[2] = z;
    }
  };

  // INLINED
  float Coordinate(const float* point, const float* matrix, unsigned axis) {
    return ((point[0] * matrix[axis] + point[1] * matrix[axis + 3]) + point[2] * matrix[axis + 6]) + matrix[axis + 9];
  }
}

// INLINED
__forceinline void C3dActor::FinishBounds(const float* bounds, bool modern) {
  for (unsigned axis = 0; axis < 3; ++axis) {
    this->minimum[axis] = 100000;
  }

  for (unsigned axis = 0; axis < 3; ++axis) {
    this->maximum[axis] = -100000;
  }

  BoundsCorner points[8];
  points[0].Set(bounds[0], bounds[1], bounds[2]);
  points[1].Set(bounds[3], bounds[1], bounds[2]);
  points[2].Set(bounds[0], bounds[4], bounds[2]);
  points[3].Set(bounds[3], bounds[4], bounds[2]);
  points[4].Set(bounds[0], bounds[1], bounds[5]);
  points[5].Set(bounds[3], bounds[1], bounds[5]);
  points[6].Set(bounds[0], bounds[4], bounds[5]);
  points[7].Set(bounds[3], bounds[4], bounds[5]);

  for (unsigned corner = 0; corner < 8; ++corner) {
    float* stored = this->corners[corner];
    stored[0] = Coordinate(points[corner].values, this->transform.values, 0);
    stored[1] = Coordinate(points[corner].values, this->transform.values, 1);
    stored[2] = Coordinate(points[corner].values, this->transform.values, 2);

    for (unsigned axis = 0; axis < 3; ++axis) {
      if (this->minimum[axis] > stored[axis]) {
        this->minimum[axis] = stored[axis];
      }
    }

    for (unsigned axis = 0; axis < 3; ++axis) {
      if (stored[axis] > this->maximum[axis]) {
        this->maximum[axis] = stored[axis];
      }
    }
  }

  for (unsigned axis = 0; axis < 3; ++axis) {
    const float low = Coordinate(bounds, this->transform.values, axis);
    const float high = Coordinate(bounds + 3, this->transform.values, axis);
    this->oriented.center[axis] = (modern || axis == 2 ? high + low : low + high) * 0.5f;
  }

  for (unsigned axis = 0; axis < 3; ++axis) {
    this->oriented.halfSize[axis] = Absolute((bounds[axis + 3] - bounds[axis]) * this->scale[axis]) * 0.5f;
  }

  matrix axes;
  axes.SetScale(this->scale[0], this->scale[1], this->scale[2]);
  axes.AppendYRotation(Radians(this->rotation[1]));
  axes.AppendXRotation(Radians(this->rotation[0]));
  axes.AppendZRotation(Radians(this->rotation[2]));

  SetAxis(this->oriented.axes[0], axes.values);
  SetAxis(this->oriented.axes[1], axes.values + 3);
  SetAxis(this->oriented.axes[2], axes.values + 6);

  for (std::list<ModelVolume*>::iterator it = this->volumes.begin(); it != this->volumes.end(); ++it) {
    ModelVolume* volume = *it;
    volume->transform.MakeXRotation(Radians(volume->rotation[0]));
    volume->transform.AppendYRotation(Radians(volume->rotation[1]));
    volume->transform.AppendZRotation(Radians(volume->rotation[2]));
    volume->transform.AppendTranslate(volume->position);
    volume->inverseTransform.MakeInverse(volume->transform);
    matrix combined;
    combined.Compose(volume->transform, this->transform);

    const float* size = volume->size;
    TransformCorner(volume->corners[0], -size[0], -size[1], -size[2], combined.values);
    TransformCorner(volume->corners[1], size[0], -size[1], -size[2], combined.values);
    TransformCorner(volume->corners[2], size[0], size[1], -size[2], combined.values);
    TransformCorner(volume->corners[3], -size[0], size[1], -size[2], combined.values);
    TransformCorner(volume->corners[4], -size[0], -size[1], size[2], combined.values);
    TransformCorner(volume->corners[5], size[0], -size[1], size[2], combined.values);
    TransformCorner(volume->corners[6], size[0], size[1], size[2], combined.values);
    TransformCorner(volume->corners[7], -size[0], size[1], size[2], combined.values);
  }

  this->dirty = 1;
}

// OFFSET: 0x00AB9660
// FUNCTION: fn_17512
void C3dActorV1::UpdateBounds() {
  std::array<float, 6> bounds = {100000, 100000, 100000, -100000, -100000, -100000};
  matrix identity;
  std::memcpy(identity.values, g_identityMatrix, sizeof(identity.values));
  this->root->AccumulateBounds(bounds.data(), identity.values);
  this->offset[0] = -(bounds[3] + bounds[0]) * 0.5f;
  this->offset[1] = -bounds[4];
  this->offset[2] = -(bounds[5] + bounds[2]) * 0.5f;
  this->transform.MakeTranslate(this->offset[0], this->offset[1], this->offset[2]);
  this->transform.AppendScale(this->scale[0], this->scale[1], this->scale[2]);
  this->transform.AppendYRotation(Radians(this->rotation[1]));
  this->transform.AppendXRotation(Radians(this->rotation[0]));
  this->transform.AppendZRotation(Radians(this->rotation[2]));
  this->transform.AppendTranslate(this->position);
  this->FinishBounds(bounds.data(), false);
}

// OFFSET: 0x00ABA520
// FUNCTION: fn_17513
void C3dActorV2::UpdateBounds() {
  std::array<float, 6> bounds = {100000, 100000, 100000, -100000, -100000, -100000};
  matrix identity;
  std::memcpy(identity.values, g_identityMatrix, sizeof(identity.values));

  for (std::list<C3dNodeV2*>::iterator it = this->roots.begin(); it != this->roots.end(); ++it) {
    (*it)->AccumulateBounds(bounds.data(), identity.values);
  }

  this->UpdateMatrix();
  this->FinishBounds(bounds.data(), true);
}
