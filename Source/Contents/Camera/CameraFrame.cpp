#include "Contents/Camera/CameraFrame.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00ADB560
// FUNCTION: fn_17920
void CView::Refresh() {
  constexpr float pi = 3.141592f;
  matrix orbit;
  orbit.MakeXRotation((this->camera.pitch / 180.0f) * pi);
  orbit.AppendYRotation((this->camera.yaw / 180.0f) * pi);
  const float distance = -this->camera.distance;
  this->frame.eye[1] = ((orbit.values[4] * 0.0f + orbit.values[1] * 0.0f) + distance * orbit.values[7]) + orbit.values[10];
  this->frame.eye[2] = ((orbit.values[5] * 0.0f + orbit.values[2] * 0.0f) + distance * orbit.values[8]) + orbit.values[11];
  this->frame.eye[0] = (((orbit.values[3] * 0.0f + orbit.values[0] * 0.0f) + distance * orbit.values[6]) + orbit.values[9]) + this->camera.target[0];
  this->frame.eye[1] = this->camera.target[1] + this->frame.eye[1];
  this->frame.eye[2] = this->camera.target[2] + this->frame.eye[2];
  const float height = g_modeMgr.GetGameMode()->scene->terrain->Height(this->frame.eye[0], this->frame.eye[2]);

  if (this->frame.eye[1] > height) {
    this->frame.eye[1] = height;
  }

  this->frame.view.LookAt(this->frame.eye, this->camera.target, this->frame.up);
  this->frame.inverse.MakeInverse(this->frame.view);
  g_renderer->SetCamera(this->frame.eye, this->camera.target, this->frame.up);
}

// OFFSET: 0x00ADBF60
// FUNCTION: fn_17927
void CView::Update() {
  const vector3d& position = this->scene->player->render.position;
  this->camera.desiredTarget[0] = position.x;
  this->camera.desiredTarget[1] = position.y;
  this->camera.desiredTarget[2] = position.z;
  this->Smooth();
  this->Apply();
  this->Refresh();
}

// INLINED
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
