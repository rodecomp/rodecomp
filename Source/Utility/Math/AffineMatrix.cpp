#include "Utility/Math/AffineMatrix.h"

#include <bit>
#include <cmath>
#include <cstring>

// STATIC: 0x01312438
// GLOBAL: gl_21913
const float g_identityMatrix[12] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};

namespace {
  // INLINED
  void Rotate(float* values, const float* rotation) {
    for (unsigned row = 0; row < 4; ++row) {
      const float x = values[row * 3];
      const float y = values[row * 3 + 1];
      const float z = values[row * 3 + 2];
      const float first = (x * rotation[0] + y * rotation[3]) + z * rotation[6];
      const float second = (x * rotation[1] + y * rotation[4]) + z * rotation[7];
      const float third = (x * rotation[2] + y * rotation[5]) + z * rotation[8];

      values[row * 3] = row == 3 ? first + rotation[9] : first;
      values[row * 3 + 1] = row == 3 ? second + rotation[10] : second;
      values[row * 3 + 2] = row == 3 ? third + rotation[11] : third;
    }
  }
}

// INLINED
void matrix::Identity() {
  std::memcpy(this->values, g_identityMatrix, sizeof(g_identityMatrix));
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
  rotation.MakeZRotation(radians);
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
  const float* a = first.values;
  const float* b = second.values;
  float* output = this->values;

  output[0] = (a[0] * b[0] + b[3] * a[1]) + b[6] * a[2];
  output[3] = (a[4] * b[3] + a[3] * b[0]) + a[5] * b[6];
  output[6] = (a[7] * b[3] + a[6] * b[0]) + a[8] * b[6];
  output[9] = ((a[10] * b[3] + a[9] * b[0]) + a[11] * b[6]) + b[9];

  output[1] = (b[4] * a[1] + b[1] * a[0]) + b[7] * a[2];
  output[4] = (a[4] * b[4] + a[3] * b[1]) + a[5] * b[7];
  output[7] = (a[7] * b[4] + a[6] * b[1]) + a[8] * b[7];
  output[10] = ((a[10] * b[4] + a[9] * b[1]) + a[11] * b[7]) + b[10];

  output[2] = (b[5] * a[1] + b[2] * a[0]) + a[2] * b[8];
  output[5] = (b[5] * a[4] + b[2] * a[3]) + a[5] * b[8];
  output[8] = (b[5] * a[7] + b[2] * a[6]) + a[8] * b[8];
  output[11] = ((b[5] * a[10] + b[2] * a[9]) + a[11] * b[8]) + b[11];
}

// OFFSET: 0x00554920
// FUNCTION: fn_02027
void matrix::MakeInverse(const matrix& source) {
  const float* a = source.values;
  float* output = this->values;
  const float minor = a[4] * a[8] - a[5] * a[7];
  const float inverse = 1.0f / ((minor * a[0] - (a[3] * a[8] - a[6] * a[5]) * a[1]) + (a[3] * a[7] - a[6] * a[4]) * a[2]);
  const float negative = -inverse;
  output[0] = minor * inverse;
  const float firstY = (a[1] * a[8] - a[7] * a[2]) * negative;
  output[1] = firstY;
  const float firstZ = (a[1] * a[5] - a[2] * a[4]) * inverse;
  output[2] = firstZ;
  const float secondX = (a[3] * a[8] - a[6] * a[5]) * negative;
  output[3] = secondX;
  const float secondY = (a[0] * a[8] - a[2] * a[6]) * inverse;
  output[4] = secondY;
  const float secondZ = (a[0] * a[5] - a[2] * a[3]) * negative;
  output[5] = secondZ;
  const float thirdX = (a[7] * a[3] - a[6] * a[4]) * inverse;
  output[6] = thirdX;
  const float thirdY = (a[7] * a[0] - a[1] * a[6]) * negative;
  output[7] = thirdY;
  const float thirdZ = (a[4] * a[0] - a[1] * a[3]) * inverse;
  output[8] = thirdZ;

  output[9] = -((secondX * a[10] + (minor * inverse) * a[9]) + thirdX * a[11]);
  output[10] = -((firstY * a[9] + secondY * a[10]) + thirdY * a[11]);
  output[11] = -((secondZ * a[10] + firstZ * a[9]) + thirdZ * a[11]);
}

// FUNCTION: fn_02026
int matrix::IsIdentity() const {
  const auto absolute = [](float value) {
    return std::bit_cast<float>(std::bit_cast<unsigned>(value) & 0x7FFFFFFFu);
  };
  const float tolerance = 1.0e-5f;
  if (absolute(this->values[0] - 1.0f) > tolerance || absolute(this->values[4] - 1.0f) > tolerance ||
      absolute(this->values[8] - 1.0f) > tolerance || absolute(this->values[1]) > tolerance ||
      absolute(this->values[2]) > tolerance || absolute(this->values[3]) > tolerance ||
      absolute(this->values[5]) > tolerance || absolute(this->values[6]) > tolerance ||
      absolute(this->values[7]) > tolerance || absolute(this->values[9]) > tolerance ||
      absolute(this->values[10]) > tolerance || absolute(this->values[11]) > tolerance) {
    return 0;
  }
  return 1;
}

// OFFSET: 0x00554F70
// FUNCTION: fn_02031
void matrix::MakeTransform(const float* rotaxis, float rotangle) {
  const float omega = rotangle / 2.0f;
  const float s = static_cast<float>(sin(static_cast<double>(omega)));
  const float qx = rotaxis[0] * s;
  const float qy = rotaxis[1] * s;
  const float qz = rotaxis[2] * s;
  const float qw = static_cast<float>(cos(static_cast<double>(omega)));

  const float x2 = qx + qx;
  const float y2 = qy + qy;
  const float z2 = qz + qz;
  const float wx = qw * x2;
  const float wy = qw * y2;
  const float wz = qw * z2;
  const float xx = qx * x2;
  const float xy = qx * y2;
  const float xz = qx * z2;
  const float yy = qy * y2;
  const float yz = qy * z2;
  const float zz = qz * z2;

  this->values[0] = 1.0f - (yy + zz);
  this->values[1] = xy + wz;
  this->values[2] = xz - wy;
  this->values[3] = xy - wz;
  this->values[4] = 1.0f - (xx + zz);
  this->values[5] = yz + wx;
  this->values[6] = xz + wy;
  this->values[7] = yz - wx;
  this->values[8] = 1.0f - (xx + yy);
  this->values[9] = 0.0f;
  this->values[10] = 0.0f;
  this->values[11] = 0.0f;
}
