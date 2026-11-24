#include "Utility/Math/AffineMatrix.h"

#include "Contents/Map/MapSkyMgr.h"
#include "Render/Projection/RenderProjection.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <xmmintrin.h>

// STATIC: 0x01312438
// GLOBAL: gl_21913
const float g_identityMatrix[12] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};

namespace {
  // INLINED
  __forceinline void Rotate(float* values, const float* rotation) {
    float x[4];
    float y[4];
    float z[4];
    for (unsigned row = 0; row < 4; ++row) {
      x[row] = values[row * 3];
      y[row] = values[row * 3 + 1];
      z[row] = values[row * 3 + 2];
    }

    for (unsigned row = 0; row < 4; ++row) {
      values[row * 3] = (x[row] * rotation[0] + y[row] * rotation[3]) + z[row] * rotation[6];
      values[row * 3 + 1] = (x[row] * rotation[1] + y[row] * rotation[4]) + z[row] * rotation[7];
      values[row * 3 + 2] = (x[row] * rotation[2] + y[row] * rotation[5]) + z[row] * rotation[8];
    }

    values[9] += rotation[9];
    values[10] += rotation[10];
    values[11] += rotation[11];
  }

  // INLINED
  __forceinline float FastSquareRoot(float value) {
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(value)));
  }
}

// OFFSET: 0x00555360
// FUNCTION: fn_02034
void matrix::MakeXRotation(float radians) {
  this->Identity();

  const float sine = ClientSin(radians);
  const float cosine = ClientCos(radians);
  this->values[4] = cosine;
  this->values[8] = cosine;
  this->values[5] = sine;
  this->values[7] = -sine;
}

// OFFSET: 0x005553D0
// FUNCTION: fn_02035
void matrix::MakeYRotation(float radians) {
  this->Identity();

  const float sine = ClientSin(radians);
  const float cosine = ClientCos(radians);
  this->values[0] = cosine;
  this->values[8] = cosine;
  this->values[2] = -sine;
  this->values[6] = sine;
}

// OFFSET: 0x00555440
// FUNCTION: fn_02036
void matrix::MakeZRotation(float radians) {
  this->Identity();

  const float sine = ClientSin(radians);
  const float cosine = ClientCos(radians);
  this->values[0] = cosine;
  this->values[4] = cosine;
  this->values[3] = -sine;
  this->values[1] = sine;
}

// OFFSET: 0x00552F70
// FUNCTION: fn_02016
void matrix::AppendXRotation(float radians) {
  matrix rotation;
  rotation.MakeXRotation(radians);
  Rotate(this->values, rotation.values);
}

// OFFSET: 0x00553270
// FUNCTION: fn_02017
void matrix::AppendYRotation(float radians) {
  matrix rotation;
  rotation.MakeYRotation(radians);
  Rotate(this->values, rotation.values);
}

// OFFSET: 0x00553570
// FUNCTION: fn_02018
void matrix::AppendZRotation(float radians) {
  matrix rotation;
  rotation.Identity();
  const float sine = sinf(radians);
  const float cosine = cosf(radians);
  rotation.values[0] = cosine;
  rotation.values[4] = cosine;
  rotation.values[3] = -sine;
  rotation.values[1] = sine;
  Rotate(this->values, rotation.values);
}

// OFFSET: 0x005550C0
// FUNCTION: fn_02032
void matrix::MakeTranslate(float x, float y, float z) {
  std::memcpy(this->values, g_identityMatrix, sizeof(g_identityMatrix));
  this->values[9] = x;
  this->values[10] = y;
  this->values[11] = z;
}

// OFFSET: 0x00554B40
// FUNCTION: fn_02028
void matrix::SetScale(float x, float y, float z) {
  std::memcpy(this->values, g_identityMatrix, sizeof(g_identityMatrix));
  this->values[0] = x;
  this->values[4] = y;
  this->values[8] = z;
}

// OFFSET: 0x005529C0
// FUNCTION: fn_02014
void matrix::AppendScale(float x, float y, float z) {
  matrix scale;
  scale.SetScale(x, y, z);
  Rotate(this->values, scale.values);
}

// OFFSET: 0x00552CA0
// FUNCTION: fn_02015
void matrix::AppendTranslate(const float* position) {
  matrix translation;
  translation.MakeTranslate(position[0], position[1], position[2]);
  Rotate(this->values, translation.values);
}

// OFFSET: 0x005554B0
// FUNCTION: fn_02037
void matrix::Compose(const matrix& first, const matrix& second) {
  values[0] = first.values[0] * second.values[0] + first.values[1] * second.values[3] + first.values[2] * second.values[6];
  values[3] = first.values[3] * second.values[0] + first.values[4] * second.values[3] + first.values[5] * second.values[6];
  values[6] = first.values[6] * second.values[0] + first.values[7] * second.values[3] + first.values[8] * second.values[6];
  values[9] = first.values[9] * second.values[0] + first.values[10] * second.values[3] + first.values[11] * second.values[6] + second.values[9];
  values[1] = first.values[0] * second.values[1] + first.values[1] * second.values[4] + first.values[2] * second.values[7];
  values[4] = first.values[3] * second.values[1] + first.values[4] * second.values[4] + first.values[5] * second.values[7];
  values[7] = first.values[6] * second.values[1] + first.values[7] * second.values[4] + first.values[8] * second.values[7];
  values[10] = first.values[9] * second.values[1] + first.values[10] * second.values[4] + first.values[11] * second.values[7] + second.values[10];
  values[2] = first.values[0] * second.values[2] + first.values[1] * second.values[5] + first.values[2] * second.values[8];
  values[5] = first.values[3] * second.values[2] + first.values[4] * second.values[5] + first.values[5] * second.values[8];
  values[8] = first.values[6] * second.values[2] + first.values[7] * second.values[5] + first.values[8] * second.values[8];
  values[11] = first.values[9] * second.values[2] + first.values[10] * second.values[5] + first.values[11] * second.values[8] + second.values[11];
}

// OFFSET: 0x00554920
// FUNCTION: fn_02027
void matrix::MakeInverse(const matrix& source) {
  const float minor = source.values[4] * source.values[8] - source.values[5] * source.values[7];
  const float inverse = 1.0f / ((minor * source.values[0] - (source.values[3] * source.values[8] - source.values[6] * source.values[5]) * source.values[1]) + (source.values[3] * source.values[7] - source.values[6] * source.values[4]) * source.values[2]);
  const float negative = -inverse;
  const float firstX = minor * inverse;
  values[0] = firstX;
  const float firstY = (source.values[1] * source.values[8] - source.values[7] * source.values[2]) * negative;
  values[1] = firstY;
  const float firstZ = (source.values[1] * source.values[5] - source.values[2] * source.values[4]) * inverse;
  values[2] = firstZ;
  const float secondX = (source.values[3] * source.values[8] - source.values[6] * source.values[5]) * negative;
  values[3] = secondX;
  const float secondY = (source.values[0] * source.values[8] - source.values[2] * source.values[6]) * inverse;
  values[4] = secondY;
  const float secondZ = (source.values[0] * source.values[5] - source.values[2] * source.values[3]) * negative;
  values[5] = secondZ;
  const float thirdX = (source.values[7] * source.values[3] - source.values[6] * source.values[4]) * inverse;
  values[6] = thirdX;
  const float thirdY = (source.values[7] * source.values[0] - source.values[1] * source.values[6]) * negative;
  values[7] = thirdY;
  const float thirdZ = (source.values[4] * source.values[0] - source.values[1] * source.values[3]) * inverse;
  values[8] = thirdZ;

  values[9] = -(firstX * source.values[9] + secondX * source.values[10] + thirdX * source.values[11]);
  float y = firstY * source.values[9];
  y += secondY * source.values[10];
  y += thirdY * source.values[11];
  values[10] = -y;
  values[11] = -((secondZ * source.values[10] + firstZ * source.values[9]) + thirdZ * source.values[11]);
}

// OFFSET: 0x00554620
// FUNCTION: fn_02026
int matrix::IsIdentity() const {
  const float tolerance = 1.0e-5f;
  if (std::fabs(this->values[0] - 1.0f) > tolerance || std::fabs(this->values[4] - 1.0f) > tolerance ||
      std::fabs(this->values[8] - 1.0f) > tolerance || std::fabs(this->values[1]) > tolerance ||
      std::fabs(this->values[2]) > tolerance || std::fabs(this->values[3]) > tolerance ||
      std::fabs(this->values[5]) > tolerance || std::fabs(this->values[6]) > tolerance ||
      std::fabs(this->values[7]) > tolerance || std::fabs(this->values[9]) > tolerance ||
      std::fabs(this->values[10]) > tolerance || std::fabs(this->values[11]) > tolerance) {
    return 0;
  }
  return 1;
}

// OFFSET: 0x00554E10
// FUNCTION: fn_45850
void matrix::MakeTransform(const float* position, const float* rotaxis, float rotangle) {
  const float omega = rotangle * g_depthAverageHalf;
  const float s = static_cast<float>(sin(static_cast<double>(omega)));
  const float qy = rotaxis[1] * s;
  const float qx = rotaxis[0] * s;
  const float qz = rotaxis[2] * s;
  const float qw = static_cast<float>(cos(static_cast<double>(omega)));

  const float y2 = qy + qy;
  const float x2 = qx + qx;
  const float z2 = qz + qz;
  const float wz = qw * z2;
  const float wx = qw * x2;
  const float wy = qw * y2;
  const float xx = qx * x2;
  const float xy = qx * y2;
  const float xz = qx * z2;
  const float yy = qy * y2;
  const float yz = qy * z2;
  const float zz = qz * z2;

  const float projectionDepthNumerator = g_projectionDepthNumerator;
  this->values[0] = projectionDepthNumerator - (yy + zz);
  this->values[1] = xy + wz;
  this->values[3] = xy - wz;
  this->values[2] = xz - wy;
  this->values[6] = xz + wy;
  this->values[4] = projectionDepthNumerator - (xx + zz);
  this->values[5] = yz + wx;
  this->values[7] = yz - wx;
  this->values[8] = projectionDepthNumerator - (xx + yy);
  this->values[9] = position[0];
  this->values[10] = position[1];
  this->values[11] = position[2];
}

// OFFSET: 0x00554F70
// FUNCTION: fn_02031
void matrix::MakeTransform(const float* rotaxis, float rotangle) {
  const float omega = rotangle * g_depthAverageHalf;
  const float s = sinf(omega);
  const float qy = s * rotaxis[1];
  const float qx = s * rotaxis[0];
  const float qz = s * rotaxis[2];
  const float qw = cosf(omega);

  const float y2 = qy + qy;
  const float x2 = qx + qx;
  const float z2 = qz + qz;
  const float wx = qw * x2;
  const float wy = qw * y2;
  const float wz = qw * z2;
  const float xx = qx * x2;
  const float xz = qx * z2;
  const float xy = qx * y2;
  const float yy = qy * y2;
  const float yz = qy * z2;
  const float zz = qz * z2;

  this->values[9] = 0.0f;
  this->values[10] = 0.0f;
  this->values[11] = 0.0f;
  const float projectionDepthNumerator = g_projectionDepthNumerator;
  this->values[0] = projectionDepthNumerator - (yy + zz);
  this->values[1] = xy + wz;
  this->values[3] = xy - wz;
  this->values[2] = xz - wy;
  this->values[6] = xz + wy;
  this->values[4] = projectionDepthNumerator - (xx + zz);
  this->values[5] = yz + wx;
  this->values[7] = yz - wx;
  this->values[8] = projectionDepthNumerator - (xx + yy);
}

// OFFSET: 0x005544C0
// FUNCTION: fn_46340
void SlerpQuaternion(RotationQuaternion& result, double amount, const RotationQuaternion& first, const RotationQuaternion& second) {
  double dot = second.w * first.w + second.x * first.x + second.y * first.y + second.z * first.z;
  RotationQuaternion end;
  if (0.0 > dot) {
    dot = -dot;
    end = {-second.w, -second.x, -second.y, -second.z};
  } else {
    end = second;
  }

  double firstWeight = 1.0 - amount;
  double secondWeight = amount;
  if (1.0 - dot > 0.00001f) {
    const double angle = std::acos(dot);
    const float sine = sinf(static_cast<float>(angle));
    const float firstSine = sinf(static_cast<float>(firstWeight * angle));
    const float secondSine = sinf(static_cast<float>(amount * angle));
    secondWeight = static_cast<double>(secondSine) / sine;
    firstWeight = static_cast<double>(firstSine) / sine;
  }

  const float firstScale = static_cast<float>(firstWeight);
  const float secondScale = static_cast<float>(secondWeight);
  float start[4];
  float finish[4];
  std::memcpy(start, &first, sizeof(start));
  std::memcpy(finish, &end, sizeof(finish));
  for (int component = 0; component < 4; ++component) {
    start[component] = firstScale * start[component] + secondScale * finish[component];
  }
  std::memcpy(&result, start, sizeof(result));
}

// OFFSET: 0x005546F0
// FUNCTION: fn_46341
void MatrixToQuaternion(RotationQuaternion& result, const matrix& rotation) {
  const float* m = rotation.values;
  const float z = -m[0] - m[4] + m[8] + g_projectionDepthNumerator;
  const float x = m[0] - m[4] - m[8] + g_projectionDepthNumerator;
  const float y = m[4] - m[0] - m[8] + g_projectionDepthNumerator;
  const float w = m[4] + m[0] + m[8] + g_projectionDepthNumerator;
  const float wz = z > w ? z : w;
  const float wyz = y > wz ? y : wz;
  const float largest = x > wyz ? x : wyz;
  const float divisor = FastSquareRoot(largest) * 2.0f;

  if (x == largest) {
    result.x = largest / divisor;
    result.y = (m[3] + m[1]) / divisor;
    result.z = (m[6] + m[2]) / divisor;
    result.w = (m[7] - m[5]) / divisor;
  } else if (y == largest) {
    const float xValue = (m[3] + m[1]) / divisor;
    result.y = largest / divisor;
    result.x = xValue;
    result.z = (m[7] + m[5]) / divisor;
    result.w = (m[2] - m[6]) / divisor;
  } else if (z == largest) {
    result.x = (m[6] + m[2]) / divisor;
    const float yValue = m[7] + m[5];
    result.z = largest / divisor;
    result.y = yValue / divisor;
    result.w = (m[3] - m[1]) / divisor;
  } else if (w == largest) {
    result.x = (m[7] - m[5]) / divisor;
    result.y = (m[2] - m[6]) / divisor;
    const float zValue = m[3] - m[1];
    result.w = largest / divisor;
    result.z = zValue / divisor;
  }

  if (result.w < 0.0f) {
    result = {-result.w, -result.x, -result.y, -result.z};
  }
}
