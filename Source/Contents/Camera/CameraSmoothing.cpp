#include "Contents/Camera/Camera.h"
#include "Contents/Camera/CameraState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00ADB070
// FUNCTION: fn_17916
void CameraState::AddDistance(float delta) {
  this->desiredDistance = delta + this->desiredDistance;
}

// OFFSET: 0x00ADB090
// FUNCTION: fn_17917
void CameraState::AddPitch(float delta) {
  this->desiredPitch = delta + this->desiredPitch;
}

// OFFSET: 0x00ADB0B0
// FUNCTION: fn_17918
void CameraState::AddYaw(float delta) {
  this->desiredYaw = delta + this->desiredYaw;
}

// OFFSET: 0x00ADBCD0
// FUNCTION: fn_17926
void CameraState::Smooth() {
  while (this->desiredYaw >= 360.0f) {
    this->desiredYaw = this->desiredYaw - 360.0f;
  }

  while (this->desiredYaw < 0.0f) {
    this->desiredYaw = this->desiredYaw + 360.0f;
  }

  float difference = this->desiredYaw - this->yaw;

  while (difference >= 180.0f) {
    difference = difference - 360.0f;
  }

  while (-180.0f > difference) {
    difference = difference + 360.0f;
  }

  this->yaw = difference / 10.0f + this->yaw;

  while (this->yaw >= 360.0f) {
    this->yaw = this->yaw - 360.0f;
  }

  while (this->yaw < 0.0f) {
    this->yaw = this->yaw + 360.0f;
  }

  this->pitch = (this->desiredPitch - this->pitch) / 10.0f + this->pitch;
  this->distance = (this->desiredDistance - this->distance) / 10.0f + this->distance;
  const bool immediate = g_serviceOptions.Get(54) != 0;
  float next[3] = {this->desiredTarget[0], this->desiredTarget[1], this->desiredTarget[2]};

  if (!immediate) {
    for (unsigned axis = 0; axis < 3; ++axis) {
      next[axis] = (next[axis] - this->target[axis]) / 10.0f + this->target[axis];
    }
  }

  this->target[2] = next[2];
  this->target[1] = next[1];
  this->target[0] = next[0];

  const float tangent = ClientSine(static_cast<int>(-this->pitch)) / ClientCosine(static_cast<int>(-this->pitch));
  const float cotangent = ClientCosine(static_cast<int>(-this->pitch)) / ClientSine(static_cast<int>(-this->pitch));

  const float scaledTangent = tangent * g_projectionScale;
  g_tangent = scaledTangent / g_renderer->UiScaleY();
  const float scaledCotangent = cotangent * g_projectionScale;
  g_cotangent = scaledCotangent / g_renderer->UiScaleY();

  const float relativeSine = ClientSine(static_cast<int>(-this->pitch)) / ClientSine(45) * 0.000009f;
  g_cameraScaleY = (g_cameraBaseDistance / this->distance) * relativeSine;
  g_cameraScaleX = (g_cameraBaseDistance / this->distance) * 0.000036f;
}
