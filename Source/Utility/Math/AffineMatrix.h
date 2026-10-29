#ifndef RAGNAROK_SOURCE_UTILITY_MATH_AFFINEMATRIX_H
#define RAGNAROK_SOURCE_UTILITY_MATH_AFFINEMATRIX_H

#include <cstddef>

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

struct matrix {
  /* 0000 */ float values[12];

  void Identity();
  int IsIdentity() const;
  void MakeTransform(const float* rotaxis, float rotangle);
  void MakeXRotation(float radians);
  void SetYRotation(float radians);
  void SetZRotation(float radians);
  void RotateX(float radians);
  void AppendYRotation(float radians);
  void RotateZ(float radians);
  void MakeTranslate(float x, float y, float z);
  void SetScale(float x, float y, float z);
  void Scale(float x, float y, float z);
  void Translate(const float* position);
  void Compose(const matrix& first, const matrix& second);
  void MakeInverse(const matrix& source);
  void LookAt(const float* origin, const float* target, const float* up);
};

static_assert(sizeof(matrix) == 48);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(matrix, values) == 0x0000);
#endif

#endif
