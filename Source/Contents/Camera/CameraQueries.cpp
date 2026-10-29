#include "Contents/Camera/CameraFrame.h"
#include "Contents/Ground/TerrainHeight.h"

#include <cmath>

namespace {
  // INLINED
  float InverseLength(float squared) {
    const float root = std::sqrt(squared);
    return static_cast<float>(1.0 / static_cast<double>(root));
  }
}

// OFFSET: 0x00ADBAC0
// FUNCTION: fn_17924
void CameraFrame::Forward(const CameraState& camera, float* output) const {
  const float y = camera.target[1] - this->eye[1];
  const float x = camera.target[0] - this->eye[0];
  const float z = camera.target[2] - this->eye[2];

  output[1] = y;
  output[0] = x;
  output[2] = z;
  const float inverse = InverseLength((y * y + x * x) + z * z);
  output[0] = inverse * output[0];
  const float last = inverse * output[2];
  output[1] = inverse * output[1];
  output[2] = last;
}

// OFFSET: 0x00ADBB60
// FUNCTION: fn_17925
void CameraFrame::AlternateView(const CameraState& camera, float yaw, const TerrainHeight& terrain, AffineMatrix& output) const {
  constexpr float pi = 3.141592f;
  AffineMatrix orbit;
  orbit.MakeXRotation((camera.pitch / 180.0f) * pi);
  orbit.AppendYRotation((yaw / 180.0f) * pi);
  const float distance = -camera.distance;
  float eye[3];
  eye[0] = (((orbit.values[3] * 0.0f + orbit.values[0] * 0.0f) + distance * orbit.values[6]) + orbit.values[9]) + camera.target[0];
  eye[1] = (((orbit.values[4] * 0.0f + orbit.values[1] * 0.0f) + distance * orbit.values[7]) + orbit.values[10]) + camera.target[1];
  eye[2] = (((orbit.values[5] * 0.0f + orbit.values[2] * 0.0f) + distance * orbit.values[8]) + orbit.values[11]) + camera.target[2];
  const float height = terrain.Height(eye[0], eye[2]);

  if (eye[1] > height) {
    eye[1] = height;
  }

  output.LookAt(eye, camera.target, this->up);
}
