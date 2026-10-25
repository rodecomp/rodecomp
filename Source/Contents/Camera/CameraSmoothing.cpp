#include "Contents/Camera/CameraState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00ADC150
// FUNCTION: fn_17928
void CameraState::Reset() {
  this->yaw = this->desiredYaw = 0;
  this->distance = this->desiredDistance = 300;
  this->pitch = this->desiredPitch = -45;

  for (unsigned axis = 0; axis < 3; ++axis) {
    this->target[axis] = this->desiredTarget[axis] = 0;
  }
}

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
void CameraState::Smooth(
  const ServiceOptions& options, const float& rendererScale, const float& projectionScale, const float& baseDistance, CameraProjectionOutputs& output
) {
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
  const bool immediate = options.Get(54) != 0;
  float next[3] = {this->desiredTarget[0], this->desiredTarget[1], this->desiredTarget[2]};

  if (!immediate) {
    for (unsigned axis = 0; axis < 3; ++axis) {
      next[axis] = (next[axis] - this->target[axis]) / 10.0f + this->target[axis];
    }
  }

  this->target[2] = next[2];
  this->target[1] = next[1];
  this->target[0] = next[0];
  const int angle = TruncateToInt(-this->pitch);
  const float sine = ClientSine(angle);
  const float cosine = ClientCosine(angle);
  const float tangent = static_cast<float>(static_cast<double>(sine) / cosine);
  const float cotangent = static_cast<float>(static_cast<double>(cosine) / sine);
  const float scaledTangent = tangent * projectionScale;
  output.tangent = static_cast<float>(static_cast<double>(scaledTangent) / rendererScale);
  const float scaledCotangent = cotangent * projectionScale;
  output.cotangent = static_cast<float>(static_cast<double>(scaledCotangent) / rendererScale);
  const float relativeSine = static_cast<float>((static_cast<double>(sine) / ClientSine(45)) * 0.000009f);
  output.scaleY = (baseDistance / this->distance) * relativeSine;
  output.scaleX = (baseDistance / this->distance) * 0.000036f;
}
