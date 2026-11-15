#include "Contents/Camera/Camera.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"

#include <cmath>

// OFFSET: 0x00ADBAC0
// FUNCTION: fn_17924
void CView::Forward(float* output) const {
  const float y = this->camera.target.y - this->frame.eye.y;
  const float x = this->camera.target.x - this->frame.eye.x;
  const float z = this->camera.target.z - this->frame.eye.z;

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
  const float point[3] = {0.0f, 0.0f, -this->camera.distance};
  float eye[3];
  eye[0] = (((orbit.values[3] * point[1] + orbit.values[0] * point[0]) + point[2] * orbit.values[6]) + orbit.values[9]) + this->camera.target.x;
  eye[1] = (((orbit.values[4] * point[1] + orbit.values[1] * point[0]) + point[2] * orbit.values[7]) + orbit.values[10]) + this->camera.target.y;
  eye[2] = (((orbit.values[5] * point[1] + orbit.values[2] * point[0]) + point[2] * orbit.values[8]) + orbit.values[11]) + this->camera.target.z;
  const float height = g_modeMgr.GetGameMode()->scene->terrain->Height(eye[0], eye[2]);

  if (eye[1] > height) {
    eye[1] = height;
  }

  matrix output;
  output.LookAt(eye, &this->camera.target.x, &this->frame.up.x);
  return output;
}
