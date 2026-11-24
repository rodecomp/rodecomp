#include "Utility/Math/RayTriangle.h"

#include "Render/Projection/RenderProjection.h"

// INLINED
static void Subtract(float (&out)[3], const float* left, const float* right) {
  out[0] = left[0] - right[0];
  out[1] = left[1] - right[1];
  out[2] = left[2] - right[2];
}

// INLINED
static void Cross(float (&out)[3], const float* left, const float* right) {
  out[0] = left[1] * right[2] - left[2] * right[1];
  out[1] = left[2] * right[0] - left[0] * right[2];
  out[2] = left[0] * right[1] - left[1] * right[0];
}

// INLINED
static float Dot(const float* left, const float* right) {
  return left[0] * right[0] + left[1] * right[1] + left[2] * right[2];
}

// OFFSET: 0x00554060
// FUNCTION: fn_02020
int IntersectRayTriangle(const float* ray, const float* first, const float* second, const float* third, float& u, float& v, float& distance) {
  float edge1[3];
  float edge2[3];
  float pvec[3];
  float tvec[3];
  float qvec[3];

  Subtract(edge1, second, first);
  Subtract(edge2, third, first);

  Cross(pvec, ray + 3, edge2);

  const float determinant = Dot(pvec, edge1);
  if (determinant > -0.00001f && determinant < 0.00001f) {
    return false;
  }

  const float unit = g_projectionDepthNumerator;
  const float inverse = unit / determinant;
  Subtract(tvec, ray, first);

  const float barycentricU = Dot(tvec, pvec) * inverse;
  u = barycentricU;
  if (barycentricU < 0.0f || barycentricU > unit) {
    return false;
  }

  Cross(qvec, tvec, edge1);

  v = Dot(qvec, ray + 3) * inverse;
  if (v < 0.0f || v + u > unit) {
    return false;
  }

  distance = Dot(qvec, edge2) * inverse;
  return true;
}
