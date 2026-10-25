#include "Contents/Models/ModelActor.h"

#include <cmath>
#include <cstring>

namespace {
  float Absolute(float value) {
    return std::fabs(value);
  }

  float Radians(float degrees) {
    return (degrees / 180.0f) * 3.141592f;
  }

  float InverseLength(float squared) {
    const float root = std::sqrt(squared);
    return static_cast<float>(1.0 / static_cast<double>(root));
  }

  float Coordinate(const float* point, const float* matrix, unsigned axis) {
    return ((point[0] * matrix[axis] + point[1] * matrix[axis + 3]) + point[2] * matrix[axis + 6]) + matrix[axis + 9];
  }
}

// OFFSET: 0x00AB9660
// FUNCTION: fn_17512
void C3dActorV1::UpdateBounds() {
  float bounds[6] = {100000, 100000, 100000, -100000, -100000, -100000};
  AffineMatrix identity;
  identity.Identity();
  this->root->AccumulateBounds(bounds, identity.values);
  this->offset[0] = -(bounds[3] + bounds[0]) * 0.5f;
  this->offset[1] = -bounds[4];
  this->offset[2] = -(bounds[5] + bounds[2]) * 0.5f;
  this->transform.SetTranslation(this->offset[0], this->offset[1], this->offset[2]);
  this->transform.Scale(this->scale[0], this->scale[1], this->scale[2]);
  this->transform.RotateY(Radians(this->rotation[1]));
  this->transform.RotateX(Radians(this->rotation[0]));
  this->transform.RotateZ(Radians(this->rotation[2]));
  this->transform.Translate(this->position);
  this->FinishBounds(bounds, false);
}

// OFFSET: 0x00ABA520
// FUNCTION: fn_17513
void C3dActorV2::UpdateBounds() {
  float bounds[6] = {100000, 100000, 100000, -100000, -100000, -100000};
  AffineMatrix identity;
  identity.Identity();

  for (C3dNodeV2* root : this->roots) {
    root->AccumulateBounds(bounds, identity.values);
  }

  this->UpdateTransform();
  this->FinishBounds(bounds, true);
}

void C3dActor::FinishBounds(const float* bounds, bool modern) {
  for (unsigned axis = 0; axis < 3; ++axis) {
    this->minimum[axis] = 100000;
    this->maximum[axis] = -100000;
  }

  for (unsigned corner = 0; corner < 8; ++corner) {
    const float point[3] = {
      bounds[(corner & 1u) ? 3 : 0], bounds[(corner == 2 || corner == 3 || corner == 6 || corner == 7) ? 4 : 1], bounds[corner >= 4 ? 5 : 2]
    };

    for (unsigned axis = 0; axis < 3; ++axis) {
      const float value = Coordinate(point, this->transform.values, axis);
      this->corners[corner][axis] = value;

      if (this->minimum[axis] > value) {
        this->minimum[axis] = value;
      }

      if (value > this->maximum[axis]) {
        this->maximum[axis] = value;
      }
    }
  }

  for (unsigned axis = 0; axis < 3; ++axis) {
    const float low = Coordinate(bounds, this->transform.values, axis);
    const float high = Coordinate(bounds + 3, this->transform.values, axis);
    this->oriented.center[axis] = (modern || axis == 2 ? high + low : low + high) * 0.5f;
    this->oriented.halfSize[axis] = Absolute((bounds[axis + 3] - bounds[axis]) * this->scale[axis]) * 0.5f;
  }

  AffineMatrix axes;
  axes.SetScale(this->scale[0], this->scale[1], this->scale[2]);
  axes.RotateY(Radians(this->rotation[1]));
  axes.RotateX(Radians(this->rotation[0]));
  axes.RotateZ(Radians(this->rotation[2]));

  for (unsigned axis = 0; axis < 3; ++axis) {
    float* direction = this->oriented.axes[axis];
    direction[0] = axes.values[axis * 3];
    direction[1] = axes.values[axis * 3 + 1];
    direction[2] = axes.values[axis * 3 + 2];
    const float inverse = InverseLength((direction[0] * direction[0] + direction[1] * direction[1]) + direction[2] * direction[2]);
    direction[0] = inverse * direction[0];
    direction[1] = inverse * direction[1];
    direction[2] = inverse * direction[2];
  }

  for (ModelVolume* volume : this->volumes) {
    AffineMatrix rotation;
    rotation.SetXRotation(Radians(volume->rotation[0]));
    rotation.RotateY(Radians(volume->rotation[1]));
    rotation.RotateZ(Radians(volume->rotation[2]));
    rotation.Translate(volume->position);
    std::memcpy(volume->unknown_0084, rotation.values, sizeof(rotation.values));
    AffineMatrix inverse;
    inverse.Inverse(rotation);
    std::memcpy(volume->unknown_0084 + 48, inverse.values, sizeof(inverse.values));
    AffineMatrix combined;
    combined.Compose(rotation, this->transform);

    for (unsigned corner = 0; corner < 8; ++corner) {
      const float point[3] = {
        (corner == 1 || corner == 2 || corner == 5 || corner == 6) ? volume->size[0] : -volume->size[0],
        (corner == 2 || corner == 3 || corner == 6 || corner == 7) ? volume->size[1] : -volume->size[1], corner >= 4 ? volume->size[2] : -volume->size[2]
      };

      for (unsigned axis = 0; axis < 3; ++axis) {
        volume->corners[corner][axis] = Coordinate(point, combined.values, axis);
      }
    }
  }

  this->dirty = 1;
}
