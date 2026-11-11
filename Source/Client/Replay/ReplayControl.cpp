#include "Client/Replay/Replay.h"

// OFFSET: 0x00B844C0
// FUNCTION: fn_19316
float Replay::AnimationScale() const {
  if (0.25f == this->speed) {
    return 1.4f;
  }
  if (0.5f == this->speed) {
    return 1.2f;
  }
  if (1.0f == this->speed) {
    return 1.0f;
  }
  if (2.0f == this->speed) {
    return 0.8f;
  }
  if (4.0f == this->speed) {
    return 0.6f;
  }
  if (8.0f == this->speed) {
    return 0.4f;
  }
  if (16.0f == this->speed) {
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
    g_replayAnimationScaling = state <= 3 ? 0 : g_replayAnimationScaling;
  } else {
    this->state = 1;
  }
  g_replayAnimationScaling = state >= 6 && state <= 8 ? 1 : g_replayAnimationScaling;
  return true;
}
