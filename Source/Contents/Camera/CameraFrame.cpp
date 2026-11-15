#include "Contents/Camera/CameraFrame.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00CC3DE0
// FUNCTION: fn_21155
void CView::SetUp(const vector3d& up) {
  this->frame.up = up;
}

// OFFSET: 0x00ADB560
// FUNCTION: fn_17920
void CView::Refresh() {
  constexpr float pi = 3.141592f;
  matrix orbit;
  orbit.MakeXRotation((this->camera.pitch / 180.0f) * pi);
  orbit.AppendYRotation((this->camera.yaw / 180.0f) * pi);
  const float distance = -this->camera.distance;
  this->frame.eye.y = ((orbit.values[4] * 0.0f + orbit.values[1] * 0.0f) + distance * orbit.values[7]) + orbit.values[10];
  this->frame.eye.z = ((orbit.values[5] * 0.0f + orbit.values[2] * 0.0f) + distance * orbit.values[8]) + orbit.values[11];
  this->frame.eye.x = (((orbit.values[3] * 0.0f + orbit.values[0] * 0.0f) + distance * orbit.values[6]) + orbit.values[9]) + this->camera.target.x;
  this->frame.eye.y = this->camera.target.y + this->frame.eye.y;
  this->frame.eye.z = this->camera.target.z + this->frame.eye.z;
  const float height = g_modeMgr.GetGameMode()->scene->terrain->Height(this->frame.eye.x, this->frame.eye.z);

  if (this->frame.eye.y > height) {
    this->frame.eye.y = height;
  }

  this->frame.view.LookAt(&this->frame.eye.x, &this->camera.target.x, &this->frame.up.x);
  this->frame.inverse.MakeInverse(this->frame.view);
  g_renderer->SetCamera(&this->frame.eye.x, &this->camera.target.x, &this->frame.up.x);
}

// OFFSET: 0x00ADBF60
// FUNCTION: fn_17927
void CView::Update() {
  const vector3d& position = this->scene->player->render.position;
  this->camera.desiredTarget = position;
  this->Smooth();
  this->Apply();
  this->Refresh();
}

// INLINED
void CameraFrame::Forward(const CameraState& camera, float* output) const {
  const float y = camera.target.y - this->eye.y;
  const float x = camera.target.x - this->eye.x;
  const float z = camera.target.z - this->eye.z;

  output[1] = y;
  output[0] = x;
  output[2] = z;
  const float inverse = 1.0f / SquareRoot((y * y + x * x) + z * z);
  output[0] = inverse * output[0];
  const float last = inverse * output[2];
  output[1] = inverse * output[1];
  output[2] = last;
}
