#include "Utility/Math/RayTriangle.h"

#include <bit>

// OFFSET: 0x00554060
// FUNCTION: fn_02020
int IntersectRayTriangle(const float* ray, const float* first, const float* second, const float* third, float& u, float& v, float& distance) {
  float edge1[3];
  float edge2[3];
  float pvec[3];
  float tvec[3];
  float qvec[3];

  edge1[0] = second[0] - first[0];
  edge1[1] = second[1] - first[1];
  edge1[2] = second[2] - first[2];
  edge2[0] = third[0] - first[0];
  edge2[1] = third[1] - first[1];
  edge2[2] = third[2] - first[2];

  pvec[0] = ray[4] * edge2[2] - ray[5] * edge2[1];
  pvec[1] = ray[5] * edge2[0] - ray[3] * edge2[2];
  pvec[2] = ray[3] * edge2[1] - ray[4] * edge2[0];

  const float determinant = pvec[1] * edge1[1] + pvec[0] * edge1[0] + pvec[2] * edge1[2];
  if (determinant > -0.00001f && determinant < 0.00001f) {
    return false;
  }

  const float inverse = 1.0f / determinant;
  tvec[0] = ray[0] - first[0];
  tvec[1] = ray[1] - first[1];
  tvec[2] = ray[2] - first[2];

  u = (tvec[1] * pvec[1] + tvec[0] * pvec[0] + tvec[2] * pvec[2]) * inverse;
  if (u < 0.0f || u > 1.0f) {
    return false;
  }

  qvec[0] = tvec[1] * edge1[2] - tvec[2] * edge1[1];
  qvec[1] = tvec[2] * edge1[0] - tvec[0] * edge1[2];
  qvec[2] = tvec[0] * edge1[1] - tvec[1] * edge1[0];

  v = (qvec[1] * ray[4] + qvec[0] * ray[3] + qvec[2] * ray[5]) * inverse;
  if (v < 0.0f || u + v > 1.0f) {
    return false;
  }

  distance = (qvec[1] * edge2[1] + qvec[0] * edge2[0] + qvec[2] * edge2[2]) * inverse;
  return true;
}
