#ifndef RAGNAROK_SOURCE_UTILITY_MATH_AFFINEMATRIX_H
#define RAGNAROK_SOURCE_UTILITY_MATH_AFFINEMATRIX_H

#include <cstddef>
#include <cstring>

extern const float g_identityMatrix[12];
extern float g_radTable[722];
extern float g_sinTable[722];
extern float g_cosTable[722];

void CreateTrigonometricTable();

float ClientSin(float radians);
float ClientCos(float radians);
float GetRadian(int degrees);
float ClientSine(int degrees);
float ClientCosine(int degrees);
float SquareRoot(float value);

struct ModelRotationKey;

struct matrix {
  /* 0000 */ float values[12];

  void Identity();
  int IsIdentity() const;
  void MakeTransform(const float* position, const float* rotaxis, float rotangle);
  void MakeTransform(const float* rotaxis, float rotangle);
  void MakeXRotation(float radians);
  void MakeYRotation(float radians);
  void MakeZRotation(float radians);
  void AppendXRotation(float radians);
  void AppendYRotation(float radians);
  void AppendZRotation(float radians);
  void MakeTranslate(float x, float y, float z);
  void SetScale(float x, float y, float z);
  void AppendScale(float x, float y, float z);
  void AppendTranslate(const float* position);
  void Compose(const matrix& first, const matrix& second);
  void MakeInverse(const matrix& source);
  void MakeRotation(const ModelRotationKey& key);
  ModelRotationKey GetRotation() const;
  void LookAt(const float* origin, const float* target, const float* up);
};

struct RotationQuaternion {
  /* 0000 */ float w;
  /* 0004 */ float x;
  /* 0008 */ float y;
  /* 000C */ float z;
};

void SlerpQuaternion(RotationQuaternion& result, double amount, const RotationQuaternion& first, const RotationQuaternion& second);
void MatrixToQuaternion(RotationQuaternion& result, const matrix& rotation);

// INLINED
inline void matrix::Identity() {
  std::memcpy(this->values, g_identityMatrix, sizeof(g_identityMatrix));
}

static_assert(sizeof(matrix) == 48);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(matrix, values) == 0x0000);
#endif

#endif
