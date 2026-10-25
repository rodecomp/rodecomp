#ifndef RAGNAROK_SOURCE_UTILITY_MATH_AFFINEMATRIX_H
#define RAGNAROK_SOURCE_UTILITY_MATH_AFFINEMATRIX_H

extern const float g_identityMatrix[12];

float ClientSin(float radians);
float ClientCos(float radians);
float ClientRadians(int degrees);
float ClientSine(int degrees);
float ClientCosine(int degrees);

struct AffineMatrix {
  float values[12];

  void Identity();
  void SetXRotation(float radians);
  void SetYRotation(float radians);
  void SetZRotation(float radians);
  void RotateX(float radians);
  void RotateY(float radians);
  void RotateZ(float radians);
  void SetTranslation(float x, float y, float z);
  void SetScale(float x, float y, float z);
  void Scale(float x, float y, float z);
  void Translate(const float* position);
  void Compose(const AffineMatrix& first, const AffineMatrix& second);
  void Inverse(const AffineMatrix& source);
  void LookAt(const float* origin, const float* target, const float* up);
};

static_assert(sizeof(AffineMatrix) == 48);

#endif
