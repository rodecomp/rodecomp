#include "Contents/Camera/Camera.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Camera/CameraState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00ADB070
// FUNCTION: fn_17916
void CView::AddDistance(float delta) {
  this->camera.desiredDistance = delta + this->camera.desiredDistance;
}

// OFFSET: 0x00ADB090
// FUNCTION: fn_17917
void CView::AddPitch(float delta) {
  this->camera.desiredPitch = delta + this->camera.desiredPitch;
}

// OFFSET: 0x00ADB0B0
// FUNCTION: fn_17918
void CView::AddYaw(float delta) {
  this->camera.desiredYaw = delta + this->camera.desiredYaw;
}

// OFFSET: 0x00ADBCD0
// FUNCTION: fn_17926
void CView::Smooth() {
  while (this->camera.desiredYaw >= 360.0f) {
    this->camera.desiredYaw = this->camera.desiredYaw - 360.0f;
  }

  while (this->camera.desiredYaw < 0.0f) {
    this->camera.desiredYaw = this->camera.desiredYaw + 360.0f;
  }

  float difference = this->camera.desiredYaw - this->camera.yaw;

  while (difference >= 180.0f) {
    difference = difference - 360.0f;
  }

  while (-180.0f > difference) {
    difference = difference + 360.0f;
  }

  this->camera.yaw = difference / 10.0f + this->camera.yaw;

  while (this->camera.yaw >= 360.0f) {
    this->camera.yaw = this->camera.yaw - 360.0f;
  }

  while (this->camera.yaw < 0.0f) {
    this->camera.yaw = this->camera.yaw + 360.0f;
  }

  this->camera.pitch = (this->camera.desiredPitch - this->camera.pitch) / 10.0f + this->camera.pitch;
  this->camera.distance = (this->camera.desiredDistance - this->camera.distance) / 10.0f + this->camera.distance;
  const bool immediate = g_serviceOptions.Get(TT_FIXED_CAMERA_ON_OFF) != 0;
  float next[3] = {this->camera.desiredTarget.x, this->camera.desiredTarget.y, this->camera.desiredTarget.z};

  if (!immediate) {
    for (unsigned axis = 0; axis < 3; ++axis) {
      next[axis] = (next[axis] - (&this->camera.target.x)[axis]) / 10.0f + (&this->camera.target.x)[axis];
    }
  }

  this->camera.target.z = next[2];
  this->camera.target.y = next[1];
  this->camera.target.x = next[0];

  const float tangent = ClientSine(static_cast<int>(-this->camera.pitch)) / ClientCosine(static_cast<int>(-this->camera.pitch));
  const float cotangent = ClientCosine(static_cast<int>(-this->camera.pitch)) / ClientSine(static_cast<int>(-this->camera.pitch));

  const float scaledTangent = tangent * g_projectionScale;
  g_tangent = scaledTangent / g_renderer->UiScaleY();
  const float scaledCotangent = cotangent * g_projectionScale;
  g_cotangent = scaledCotangent / g_renderer->UiScaleY();

  const float relativeSine = ClientSine(static_cast<int>(-this->camera.pitch)) / ClientSine(45) * 0.000009f;
  g_cameraScaleY = (g_cameraBaseDistance / this->camera.distance) * relativeSine;
  g_cameraScaleX = (g_cameraBaseDistance / this->camera.distance) * 0.000036f;
}
