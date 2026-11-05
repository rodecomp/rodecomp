#include "Contents/Camera/Camera.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"

#include <cmath>

// OFFSET: 0x00ADBAC0
// FUNCTION: fn_17924
void CameraFrame::Forward(const CameraState& camera, float* output) const {
  const float y = camera.target[1] - this->eye[1];
  const float x = camera.target[0] - this->eye[0];
  const float z = camera.target[2] - this->eye[2];

  output[1] = y;
  output[0] = x;
  output[2] = z;
  const float inverse = 1.0f / SquareRoot((y * y + x * x) + z * z);
  output[0] = inverse * output[0];
  const float last = inverse * output[2];
  output[1] = inverse * output[1];
  output[2] = last;
}

// OFFSET: 0x00ADBB60
// FUNCTION: fn_17925
matrix CView::AlternateView(float yaw) const {
  constexpr float pi = 3.141592f;
  matrix orbit;
  orbit.MakeXRotation((this->camera.pitch / 180.0f) * pi);
  orbit.AppendYRotation((yaw / 180.0f) * pi);
  const float distance = -this->camera.distance;
  float eye[3];
  eye[0] = (((orbit.values[3] * 0.0f + orbit.values[0] * 0.0f) + distance * orbit.values[6]) + orbit.values[9]) + this->camera.target[0];
  eye[1] = (((orbit.values[4] * 0.0f + orbit.values[1] * 0.0f) + distance * orbit.values[7]) + orbit.values[10]) + this->camera.target[1];
  eye[2] = (((orbit.values[5] * 0.0f + orbit.values[2] * 0.0f) + distance * orbit.values[8]) + orbit.values[11]) + this->camera.target[2];
  const float height = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(eye[0], eye[2]);

  if (eye[1] > height) {
    eye[1] = height;
  }

  matrix output;
  output.LookAt(eye, this->camera.target, this->frame.up);
  return output;
}
