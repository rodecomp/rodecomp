#include "Client/Replay.h"

// OFFSET: 0x00B844C0
// FUNCTION: fn_19316
float Replay::AnimationScale() const {
  if (this->speed == 0.25f) {
    return 1.4f;
  }
  if (this->speed == 0.5f) {
    return 1.2f;
  }
  if (this->speed == 1.0f) {
    return 1.0f;
  }
  if (this->speed == 2.0f) {
    return 0.8f;
  }
  if (this->speed == 4.0f) {
    return 0.6f;
  }
  if (this->speed == 8.0f) {
    return 0.4f;
  }
  if (this->speed == 16.0f) {
    return 0.2f;
  }
  return 1.0f;
}

// OFFSET: 0x00B872F0
// FUNCTION: fn_19343
bool Replay::SetState(int state) {
  if (state == 4) {
    this->state = 1;
    return true;
  }
  if (state == 5) {
    this->state = 6;
    return true;
  }
  if (state >= 1 && state <= 8) {
    this->state = state;
    if (state <= 3) {
      g_replayAnimationScaling = 0;
    }
  } else {
    this->state = 1;
  }
  if (state >= 6 && state <= 8) {
    g_replayAnimationScaling = 1;
  }
  return true;
}
