#include "Utility/Math/RayTriangle.h"

#include <bit>

// OFFSET: 0x00554060
// FUNCTION: fn_02020
bool IntersectRayTriangle(const float* ray, const float* first, const float* second, const float* third, float& u, float& v, float& distance) {
  const float edgeX = second[0] - first[0];
  const float edgeY = second[1] - first[1];
  const float edgeZ = second[2] - first[2];
  const float otherY = third[1] - first[1];
  const float otherZ = third[2] - first[2];
  const float otherX = third[0] - first[0];
  const float crossX = otherZ * ray[4] - otherY * ray[5];
  const float crossY = otherX * ray[5] - otherZ * ray[3];
  const float crossZ = otherY * ray[3] - otherX * ray[4];
  const float determinant = (crossY * edgeY + crossX * edgeX) + crossZ * edgeZ;
  constexpr float epsilon = 0.00001f;
  constexpr float negativeEpsilon = -0.00001f;

  if (determinant > negativeEpsilon && epsilon > determinant) {
    return false;
  }

  const float x = ray[0] - first[0];
  const float y = ray[1] - first[1];
  const float z = ray[2] - first[2];
  const float inverse = 1.0f / determinant;
  const float firstCoordinate = ((y * crossY + x * crossX) + z * crossZ) * inverse;
  u = firstCoordinate;

  if (firstCoordinate < 0.0f || firstCoordinate > 1.0f) {
    return false;
  }

  const float qX = edgeZ * y - edgeY * z;
  const float qY = edgeX * z - x * edgeZ;
  const float qZ = x * edgeY - edgeX * y;
  const float secondCoordinate = ((qY * ray[4] + qX * ray[3]) + qZ * ray[5]) * inverse;
  v = secondCoordinate;

  if (secondCoordinate < 0.0f || secondCoordinate + u > 1.0f) {
    return false;
  }

  distance = ((qY * otherY + qX * otherX) + qZ * otherZ) * inverse;
  return true;
}
