#include "Contents/Camera/Camera.h"
#include "Contents/Camera/CameraState.h"
#include "Utility/Math/AffineMatrix.h"

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
void CView::Apply() {
  if (!this->shake.enabled) {
    return;
  }

  if (timeGetTime() >= this->shake.start + this->shake.duration) {
    this->camera.target[0] = this->camera.desiredTarget[0];
    this->camera.target[1] = this->camera.desiredTarget[1];
    this->camera.target[2] = this->camera.desiredTarget[2];
    this->camera.distance = this->camera.desiredDistance;
    this->shake.enabled = 0;
    this->camera.pitch = this->camera.desiredPitch;

    return;
  }

  if (!g_worldViewDrawn) {
    return;
  }

  g_worldViewDrawn = 0;

  // GLOBAL: gl_24565
  static float horizontal = this->shake.horizontal;
  this->camera.target[0] += (Jitter(10.0f) + horizontal) * ClientCosine(static_cast<int>(this->camera.yaw));
  const float offsetZ = (Jitter(10.0f) + horizontal) * -ClientSine(static_cast<int>(this->camera.yaw));
  horizontal = -horizontal;
  this->camera.target[2] += offsetZ;
  // GLOBAL: gl_24566
  static float distance = this->shake.distance;
  const float distanceDelta = Jitter(10.0f) + distance;
  distance = -distance;
  this->camera.distance = distanceDelta + this->camera.distance;
  // GLOBAL: gl_24567
  static float pitch = this->shake.pitch;
  const float pitchDelta = Jitter(15.0f) + pitch;
  pitch = -pitch;
  this->camera.pitch = pitchDelta + this->camera.pitch;
}
