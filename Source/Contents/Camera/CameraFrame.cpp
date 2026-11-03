#include "Contents/Camera/CameraFrame.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"

// INLINED
__forceinline void CameraFrame::Build(const CameraState& camera) {
  constexpr float pi = 3.141592f;
  matrix orbit;
  orbit.MakeXRotation((camera.pitch / 180.0f) * pi);
  orbit.AppendYRotation((camera.yaw / 180.0f) * pi);
  const float distance = -camera.distance;
  this->eye[1] = ((orbit.values[4] * 0.0f + orbit.values[1] * 0.0f) + distance * orbit.values[7]) + orbit.values[10];
  this->eye[2] = ((orbit.values[5] * 0.0f + orbit.values[2] * 0.0f) + distance * orbit.values[8]) + orbit.values[11];
  this->eye[0] = (((orbit.values[3] * 0.0f + orbit.values[0] * 0.0f) + distance * orbit.values[6]) + orbit.values[9]) + camera.target[0];
  this->eye[1] = camera.target[1] + this->eye[1];
  this->eye[2] = camera.target[2] + this->eye[2];
  const float height = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(this->eye[0], this->eye[2]);

  if (this->eye[1] > height) {
    this->eye[1] = height;
  }

  this->view.LookAt(this->eye, camera.target, this->up);
  this->inverse.MakeInverse(this->view);
}

// OFFSET: 0x00ADB560
// FUNCTION: fn_17920
void CameraFrame::Refresh(const CameraState& camera, CRenderer& renderer) {
  this->Build(camera);
  renderer.SetCamera(this->eye, camera.target, this->up);
}

// OFFSET: 0x00ADBF60
// FUNCTION: fn_17927
void CView::Update() {
  const vector3d& position = this->scene->player->render.position;
  this->camera.desiredTarget[0] = position.x;
  this->camera.desiredTarget[1] = position.y;
  this->camera.desiredTarget[2] = position.z;
  this->camera.Smooth();
  this->shake.Apply(this->camera, g_worldViewDrawn);
  this->frame.Build(this->camera);
  g_renderer->SetCamera(this->frame.eye, this->camera.target, this->frame.up);
}
