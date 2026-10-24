#include "Contents/CameraFrame.h"
#include "Contents/TerrainHeight.h"
#include "Render/Renderer.h"

#include <bit>

// OFFSET: 0x00ADC150
// FUNCTION: fn_17928
void ResetCamera(CameraState& camera, CameraFrame& frame) {
  camera.Reset();
  frame.up[0] = 0;
  frame.up[1] = 1;
  frame.up[2] = 0;
}

// OFFSET: 0x00ADBF60
// FUNCTION: fn_17927
void CameraFrame::Build(const CameraState& camera, const TerrainHeight& terrain) {
  constexpr float pi = 3.141592f;
  AffineMatrix orbit;
  orbit.SetXRotation((camera.pitch / 180.0f) * pi);
  orbit.RotateY((camera.yaw / 180.0f) * pi);
  const float distance = -camera.distance;
  this->eye[1] = ((orbit.values[4] * 0.0f + orbit.values[1] * 0.0f) + distance * orbit.values[7]) + orbit.values[10];
  this->eye[2] = ((orbit.values[5] * 0.0f + orbit.values[2] * 0.0f) + distance * orbit.values[8]) + orbit.values[11];
  this->eye[0] = (((orbit.values[3] * 0.0f + orbit.values[0] * 0.0f) + distance * orbit.values[6]) + orbit.values[9]) +
                 camera.target[0];
  this->eye[1] = camera.target[1] + this->eye[1];
  this->eye[2] = camera.target[2] + this->eye[2];
  const float height = terrain.Height(this->eye[0], this->eye[2]);

  if (this->eye[1] > height) {
    this->eye[1] = height;
  }

  this->view.LookAt(this->eye, camera.target, this->up);
  this->inverse.Inverse(this->view);
}

// OFFSET: 0x00ADB560
// FUNCTION: fn_17920
void CameraFrame::Refresh(const CameraState& camera, const TerrainHeight& terrain, CRenderer& renderer) {
  this->Build(camera, terrain);
  renderer.SetCamera(this->eye, camera.target, this->up);
}

// OFFSET: 0x00ADBF60
// FUNCTION: fn_17927
void UpdateCameraFrame(
  CameraState& camera,
  CameraShake& shake,
  int& frameLatch,
  const float* desiredTarget,
  const ServiceOptions& options,
  const float& rendererScale,
  const float& projectionScale,
  const float& baseDistance,
  CameraProjectionOutputs& outputs,
  const TerrainHeight& terrain,
  CameraFrame& frame,
  CRenderer& renderer
) {
  camera.desiredTarget[0] = desiredTarget[0];
  camera.desiredTarget[1] = desiredTarget[1];
  camera.desiredTarget[2] = desiredTarget[2];
  camera.Smooth(options, rendererScale, projectionScale, baseDistance, outputs);
  shake.Apply(camera, frameLatch);
  frame.Refresh(camera, terrain, renderer);
}
