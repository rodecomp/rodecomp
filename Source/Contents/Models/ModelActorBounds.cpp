#include "Contents/Models/ModelActor.h"
#include "Contents/Models/ModelAnimation.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ClientAngles.h"

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
    return (degrees / g_angleDivisor) * g_piApprox;
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
  void SetComponents(float* vector, float x, float y, float z) {
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
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

  // INLINED
  float Coordinate(const float* point, const float* matrix, unsigned axis) {
    return ((point[0] * matrix[axis] + point[1] * matrix[axis + 3]) + point[2] * matrix[axis + 6]) + matrix[axis + 9];
  }
}

// INLINED
__forceinline void C3dActor::FinishBounds(const float* bounds, bool) {
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

  BoundsCorner* output = this->corners;
  for (unsigned corner = 0; corner < 8; ++corner) {
    const BoundsCorner& point = points[corner];
    float* stored = output->values;
    stored[0] = Coordinate(point.values, this->transform.values, 0);
    stored[1] = Coordinate(point.values, this->transform.values, 1);
    stored[2] = Coordinate(point.values, this->transform.values, 2);

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
    ++output;
  }

  BoundsCorner high;
  TransformCorner(high.values, bounds[3], bounds[4], bounds[5], this->transform.values);
  BoundsCorner low;
  TransformCorner(low.values, bounds[0], bounds[1], bounds[2], this->transform.values);

  for (unsigned axis = 0; axis < 3; ++axis) {
    this->oriented.center[axis] = (high.values[axis] + low.values[axis]) * g_depthAverageHalf;
  }

  SetComponents(
    this->oriented.halfSize, Absolute((bounds[3] - bounds[0]) * this->scale.x) * g_depthAverageHalf,
    Absolute((bounds[4] - bounds[1]) * this->scale.y) * g_depthAverageHalf, Absolute((bounds[5] - bounds[2]) * this->scale.z) * g_depthAverageHalf
  );

  matrix axes;
  axes.SetScale(this->scale.x, this->scale.y, this->scale.z);
  axes.AppendYRotation(Radians(this->rotation.y));
  axes.AppendXRotation(Radians(this->rotation.x));
  axes.AppendZRotation(Radians(this->rotation.z));

  SetAxis(this->oriented.axes[0], axes.values);
  SetAxis(this->oriented.axes[1], axes.values + 3);
  SetAxis(this->oriented.axes[2], axes.values + 6);

  for (std::list<ModelVolume*>::iterator it = this->volumes.begin(); it != this->volumes.end(); ++it) {
    ModelVolume* volume = *it;
    volume->transform.MakeXRotation(Radians(volume->rotation.values[0]));
    volume->transform.AppendYRotation(Radians(volume->rotation.values[1]));
    volume->transform.AppendZRotation(Radians(volume->rotation.values[2]));
    volume->transform.AppendTranslate(volume->position.values);
    volume->inverseTransform.MakeInverse(volume->transform);
    matrix combined;
    combined.Compose(volume->transform, this->transform);

    const float* size = volume->size.values;
    TransformCorner(volume->corners[0].values, -size[0], -size[1], -size[2], combined.values);
    TransformCorner(volume->corners[1].values, size[0], -size[1], -size[2], combined.values);
    TransformCorner(volume->corners[2].values, size[0], size[1], -size[2], combined.values);
    TransformCorner(volume->corners[3].values, -size[0], size[1], -size[2], combined.values);
    TransformCorner(volume->corners[4].values, -size[0], -size[1], size[2], combined.values);
    TransformCorner(volume->corners[5].values, size[0], -size[1], size[2], combined.values);
    TransformCorner(volume->corners[6].values, size[0], size[1], size[2], combined.values);
    TransformCorner(volume->corners[7].values, -size[0], size[1], size[2], combined.values);
  }

  this->dirty = 1;
}

// INLINED
__forceinline void FinishBoundsModern(C3dActor* actor, const float* bounds, bool modern) {
  for (unsigned axis = 0; axis < 3; ++axis) {
    actor->minimum[axis] = 100000;
  }

  for (unsigned axis = 0; axis < 3; ++axis) {
    actor->maximum[axis] = -100000;
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

  BoundsCorner* output = actor->corners;
  for (unsigned corner = 0; corner < 8; ++corner) {
    const BoundsCorner& point = points[corner];
    float* stored = output->values;
    stored[0] = Coordinate(point.values, actor->transform.values, 0);
    stored[1] = Coordinate(point.values, actor->transform.values, 1);
    stored[2] = Coordinate(point.values, actor->transform.values, 2);

    for (unsigned axis = 0; axis < 3; ++axis) {
      if (actor->minimum[axis] > stored[axis]) {
        actor->minimum[axis] = stored[axis];
      }
    }

    for (unsigned axis = 0; axis < 3; ++axis) {
      if (stored[axis] > actor->maximum[axis]) {
        actor->maximum[axis] = stored[axis];
      }
    }
    ++output;
  }

  for (unsigned axis = 0; axis < 3; ++axis) {
    const float high = Coordinate(bounds + 3, actor->transform.values, axis);
    const float low = Coordinate(bounds, actor->transform.values, axis);
    actor->oriented.center[axis] = (high + low) * g_depthAverageHalf;
  }

  actor->oriented.halfSize[2] = Absolute((bounds[5] - bounds[2]) * actor->scale.z) * g_depthAverageHalf;
  actor->oriented.halfSize[1] = Absolute((bounds[4] - bounds[1]) * actor->scale.y) * g_depthAverageHalf;
  actor->oriented.halfSize[0] = Absolute((bounds[3] - bounds[0]) * actor->scale.x) * g_depthAverageHalf;

  matrix axes;
  axes.SetScale(actor->scale.x, actor->scale.y, actor->scale.z);
  axes.AppendYRotation(Radians(actor->rotation.y));
  axes.AppendXRotation(Radians(actor->rotation.x));
  axes.AppendZRotation(Radians(actor->rotation.z));

  SetAxis(actor->oriented.axes[0], axes.values);
  SetAxis(actor->oriented.axes[1], axes.values + 3);
  SetAxis(actor->oriented.axes[2], axes.values + 6);

  for (std::list<ModelVolume*>::iterator it = actor->volumes.begin(); it != actor->volumes.end(); ++it) {
    ModelVolume* volume = *it;
    volume->transform.MakeXRotation(Radians(volume->rotation.values[0]));
    volume->transform.AppendYRotation(Radians(volume->rotation.values[1]));
    volume->transform.AppendZRotation(Radians(volume->rotation.values[2]));
    volume->transform.AppendTranslate(volume->position.values);
    volume->inverseTransform.MakeInverse(volume->transform);
    matrix combined;
    combined.Compose(volume->transform, actor->transform);

    const float* size = volume->size.values;
    TransformCorner(volume->corners[0].values, -size[0], -size[1], -size[2], combined.values);
    TransformCorner(volume->corners[1].values, size[0], -size[1], -size[2], combined.values);
    TransformCorner(volume->corners[2].values, size[0], size[1], -size[2], combined.values);
    TransformCorner(volume->corners[3].values, -size[0], size[1], -size[2], combined.values);
    TransformCorner(volume->corners[4].values, -size[0], -size[1], size[2], combined.values);
    TransformCorner(volume->corners[5].values, size[0], -size[1], size[2], combined.values);
    TransformCorner(volume->corners[6].values, size[0], size[1], size[2], combined.values);
    TransformCorner(volume->corners[7].values, -size[0], size[1], size[2], combined.values);
  }

  actor->dirty = 1;
}

// OFFSET: 0x00AB9660
// FUNCTION: fn_17512
void C3dActorV1::UpdateBounds() {
  std::array<float, 6> bounds = {100000, 100000, 100000, -100000, -100000, -100000};
  matrix identity;
  std::memcpy(identity.values, g_identityMatrix, sizeof(identity.values));
  this->root->AccumulateBounds(&bounds[0], identity);
  this->offset.z = -(bounds[5] + bounds[2]) * g_depthAverageHalf;
  this->offset.x = -(bounds[3] + bounds[0]) * g_depthAverageHalf;
  this->offset.y = -bounds[4];
  this->transform.MakeTranslate(this->offset.x, this->offset.y, this->offset.z);
  this->transform.AppendScale(this->scale.x, this->scale.y, this->scale.z);
  this->transform.AppendYRotation(Radians(this->rotation.y));
  this->transform.AppendXRotation(Radians(this->rotation.x));
  this->transform.AppendZRotation(Radians(this->rotation.z));
  this->transform.AppendTranslate(&this->position.x);
  this->FinishBounds(&bounds[0], false);
}

// OFFSET: 0x00ABA520
// FUNCTION: fn_17513
void C3dActorV2::UpdateBounds() {
  std::array<float, 6> bounds = {100000, 100000, 100000, -100000, -100000, -100000};
  matrix identity;
  std::memcpy(identity.values, g_identityMatrix, sizeof(identity.values));

  for (std::list<C3dNodeV2*>::iterator it = this->roots.begin(); it != this->roots.end(); ++it) {
    (*it)->AccumulateBounds(bounds.data(), identity);
  }

  this->UpdateMatrix();

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

  BoundsCorner* output = this->corners;
  const BoundsCorner* point = points;
  for (unsigned corner = 0; corner < 8; ++corner) {
    float* stored = output->values;
    stored[0] = Coordinate(point->values, this->transform.values, 0);
    stored[1] = Coordinate(point->values, this->transform.values, 1);
    stored[2] = Coordinate(point->values, this->transform.values, 2);

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
    ++output;
    ++point;
  }

  BoundsCorner high;
  TransformCorner(high.values, bounds[3], bounds[4], bounds[5], this->transform.values);
  BoundsCorner low;
  TransformCorner(low.values, bounds[0], bounds[1], bounds[2], this->transform.values);

  for (unsigned axis = 0; axis < 3; ++axis) {
    this->oriented.center[axis] = (high.values[axis] + low.values[axis]) * 0.5f;
  }

  this->oriented.halfSize[2] = Absolute((bounds[5] - bounds[2]) * this->scale.z) * 0.5f;
  this->oriented.halfSize[1] = Absolute((bounds[4] - bounds[1]) * this->scale.y) * 0.5f;
  this->oriented.halfSize[0] = Absolute((bounds[3] - bounds[0]) * this->scale.x) * 0.5f;

  matrix axes;
  axes.SetScale(this->scale.x, this->scale.y, this->scale.z);
  axes.AppendYRotation(Radians(this->rotation.y));
  axes.AppendXRotation(Radians(this->rotation.x));
  axes.AppendZRotation(Radians(this->rotation.z));

  SetAxis(this->oriented.axes[0], axes.values);
  SetAxis(this->oriented.axes[1], axes.values + 3);
  SetAxis(this->oriented.axes[2], axes.values + 6);

  for (std::list<ModelVolume*>::iterator it = this->volumes.begin(); it != this->volumes.end(); ++it) {
    ModelVolume* volume = *it;
    volume->transform.MakeXRotation(Radians(volume->rotation.values[0]));
    volume->transform.AppendYRotation(Radians(volume->rotation.values[1]));
    volume->transform.AppendZRotation(Radians(volume->rotation.values[2]));
    volume->transform.AppendTranslate(volume->position.values);
    volume->inverseTransform.MakeInverse(volume->transform);
    matrix combined;
    combined.Compose(volume->transform, this->transform);

    const float* size = volume->size.values;
    TransformCorner(volume->corners[0].values, -size[0], -size[1], -size[2], combined.values);
    TransformCorner(volume->corners[1].values, size[0], -size[1], -size[2], combined.values);
    TransformCorner(volume->corners[2].values, size[0], size[1], -size[2], combined.values);
    TransformCorner(volume->corners[3].values, -size[0], size[1], -size[2], combined.values);
    TransformCorner(volume->corners[4].values, -size[0], -size[1], size[2], combined.values);
    TransformCorner(volume->corners[5].values, size[0], -size[1], size[2], combined.values);
    TransformCorner(volume->corners[6].values, size[0], size[1], size[2], combined.values);
    TransformCorner(volume->corners[7].values, -size[0], size[1], size[2], combined.values);
  }

  this->dirty = 1;
}
