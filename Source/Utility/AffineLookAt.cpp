#include "Utility/AffineMatrix.h"

#include <bit>
#include <cmath>

namespace {
  float Length(float squared) {
    return std::sqrt(squared);
  }

}

// OFFSET: 0x00555110
// FUNCTION: fn_02033
void AffineMatrix::LookAt(const float* origin, const float* target, const float* up) {
  // Identity precedes source reads in the original, including aliased inputs.
  this->Identity();
  const float dx = target[0] - origin[0];
  const float dy = target[1] - origin[1];
  const float dz = target[2] - origin[2];
  const float forwardInverse = 1.0f / Length((dy * dy + dx * dx) + dz * dz);
  const float forwardZ = dz * forwardInverse;
  const float forwardY = dy * forwardInverse;
  const float forwardX = dx * forwardInverse;
  const float sideX = forwardZ * up[1] - forwardY * up[2];
  const float sideY = forwardX * up[2] - forwardZ * up[0];
  const float sideZ = forwardY * up[0] - forwardX * up[1];
  const float sideInverse = 1.0f / Length((sideY * sideY + sideX * sideX) + sideZ * sideZ);
  const float rightX = sideX * sideInverse;
  const float rightY = sideY * sideInverse;
  const float rightZ = sideZ * sideInverse;
  const float crossX = rightZ * forwardY - rightY * forwardZ;
  const float crossY = rightX * forwardZ - rightZ * forwardX;
  const float crossZ = rightY * forwardX - rightX * forwardY;
  const float upInverse = 1.0f / Length((crossY * crossY + crossX * crossX) + crossZ * crossZ);
  const float upZ = crossZ * upInverse;
  this->values[2] = forwardX;
  this->values[0] = rightX;
  this->values[7] = upZ;
  const float upX = crossX * upInverse;
  this->values[5] = forwardY;
  this->values[1] = upX;
  const float upY = crossY * upInverse;
  this->values[3] = rightY;
  this->values[6] = rightZ;
  this->values[4] = upY;
  this->values[8] = forwardZ;
  this->values[9] = -((rightX * origin[0] + rightY * origin[1]) + rightZ * origin[2]);
  this->values[10] = -((upX * origin[0] + upY * origin[1]) + upZ * origin[2]);
  this->values[11] = -((forwardX * origin[0] + forwardY * origin[1]) + forwardZ * origin[2]);
}
