#include "Contents/CameraState.h"
#include "Utility/AffineMatrix.h"

#include <bit>
#include <cstdlib>
#include <emmintrin.h>
#include <windows.h>
// clang-format off
#include <mmsystem.h>
// clang-format on

namespace {
  float Jitter(float divisor) {
    const int remainder = std::rand() % 5;
    const int selected = remainder >= 0 ? remainder : 0;
    return static_cast<float>(selected - 2) / divisor;
  }
}

// OFFSET: 0x00ADCAC0
// FUNCTION: fn_17932
void CameraShake::Start(int enabled, unsigned duration, float horizontal, float distance, float pitch) {
  this->enabled = enabled;
  const unsigned time = timeGetTime();
  this->distance = distance;
  this->start = time;
  this->horizontal = horizontal;
  this->pitch = pitch;
  this->duration = duration == 0 ? 650u : (duration == 1 ? 200u : duration);
}

// OFFSET: 0x00ADCB30
// FUNCTION: fn_17933
void CameraShake::SetAmplitudes(float horizontal, float distance, float pitch) {
  this->horizontal = horizontal;
  this->pitch = pitch;
  this->distance = distance;
}

// OFFSET: 0x00ADC7A0
// FUNCTION: fn_17931
void CameraShake::Apply(CameraState& camera, int& frameLatch) {
  if (!this->enabled) {
    return;
  }

  if (timeGetTime() >= this->start + this->duration) {
    camera.target[0] = camera.desiredTarget[0];
    camera.target[1] = camera.desiredTarget[1];
    camera.target[2] = camera.desiredTarget[2];
    camera.distance = camera.desiredDistance;
    this->enabled = 0;
    camera.pitch = camera.desiredPitch;
    return;
  }

  if (!frameLatch) {
    return;
  }

  frameLatch = 0;
  // Function-static amplitudes are shared by every CView and initialized
  // independently at their first reached phase, not when a shake is started.
  static float horizontal = this->horizontal;
  const float horizontalX = Jitter(10.0f) + horizontal;
  const int yawX = _mm_cvttss_si32(_mm_set_ss(camera.yaw));
  const float x = static_cast<float>(static_cast<double>(ClientCosine(yawX)) * horizontalX);
  camera.target[0] = x + camera.target[0];
  const float horizontalZ = Jitter(10.0f) + horizontal;
  const int yawZ = _mm_cvttss_si32(_mm_set_ss(camera.yaw));
  const float sine = -ClientSine(yawZ);
  horizontal = -horizontal;
  camera.target[2] = horizontalZ * sine + camera.target[2];
  static float distance = this->distance;
  const float distanceDelta = Jitter(10.0f) + distance;
  distance = -distance;
  camera.distance = distanceDelta + camera.distance;
  static float pitch = this->pitch;
  const float pitchDelta = Jitter(15.0f) + pitch;
  pitch = -pitch;
  camera.pitch = pitchDelta + camera.pitch;
}
