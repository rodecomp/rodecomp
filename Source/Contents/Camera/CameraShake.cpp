#include "Contents/Camera/Camera.h"
#include "Contents/Camera/CameraState.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

namespace {
  // INLINED
  float Jitter(float divisor) {
    const int remainder = std::rand() % 5;
    const int selected = remainder >= 0 ? remainder : 0;
    return static_cast<float>(selected - 2) / divisor;
  }
}

// OFFSET: 0x00ADCAC0
// FUNCTION: fn_17932
void CView::Start(int enabled, unsigned duration, float horizontal, float distance, float pitch) {
  this->shake.enabled = enabled;
  const unsigned time = timeGetTime();
  this->shake.distance = distance;
  this->shake.start = time;
  this->shake.horizontal = horizontal;
  this->shake.pitch = pitch;
  switch (duration) {
  case 0:
    this->shake.duration = 650;
    break;
  case 1:
    this->shake.duration = 200;
    break;
  default:
    this->shake.duration = duration;
    break;
  }
}

// OFFSET: 0x00ADCB30
// FUNCTION: fn_17933
void CView::SetAmplitudes(float horizontal, float distance, float pitch) {
  this->shake.horizontal = horizontal;
  this->shake.pitch = pitch;
  this->shake.distance = distance;
}

// OFFSET: 0x00ADC7A0
// FUNCTION: fn_17931
void CView::Apply(CameraState& camera, int& frameLatch) {
  if (!this->shake.enabled) {
    return;
  }

  if (timeGetTime() >= this->shake.start + this->shake.duration) {
    camera.target[0] = camera.desiredTarget[0];
    camera.target[1] = camera.desiredTarget[1];
    camera.target[2] = camera.desiredTarget[2];
    camera.distance = camera.desiredDistance;
    this->shake.enabled = 0;
    camera.pitch = camera.desiredPitch;

    return;
  }

  if (!frameLatch) {
    return;
  }

  frameLatch = 0;

  // GLOBAL: gl_24565
  static float horizontal = this->shake.horizontal;
  const float horizontalX = Jitter(10.0f) + horizontal;
  const int yawX = TruncateToInt(camera.yaw);
  const float x = static_cast<float>(static_cast<double>(ClientCosine(yawX)) * horizontalX);
  camera.target[0] = x + camera.target[0];
  const float horizontalZ = Jitter(10.0f) + horizontal;
  const int yawZ = TruncateToInt(camera.yaw);
  const float sine = -ClientSine(yawZ);

  horizontal = -horizontal;
  camera.target[2] = horizontalZ * sine + camera.target[2];
  // GLOBAL: gl_24566
  static float distance = this->shake.distance;
  const float distanceDelta = Jitter(10.0f) + distance;
  distance = -distance;
  camera.distance = distanceDelta + camera.distance;
  // GLOBAL: gl_24567
  static float pitch = this->shake.pitch;
  const float pitchDelta = Jitter(15.0f) + pitch;
  pitch = -pitch;
  camera.pitch = pitchDelta + camera.pitch;
}
