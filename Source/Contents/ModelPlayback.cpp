#include "Contents/ModelActor.h"

#include <bit>
#include <emmintrin.h>
#include <windows.h>

namespace {
  unsigned UnsignedFrame(float value) {
    const unsigned bits = std::bit_cast<unsigned>(value);
    const unsigned magnitude = (bits & 0x7FFFFFFFu) << 1;

    if (bits & 0x80000000u) {
      return magnitude >= 0x7F000000u ? 0xFFFFFFFFu : static_cast<unsigned>(_mm_cvttss_si32(_mm_set_ss(value)));
    }

    if (magnitude >= 0x9F000000u) {
      return 0xFFFFFFFFu;
    }

    return magnitude >= 0x9E000000u ? (magnitude << 7) | 0x80000000u
                                    : static_cast<unsigned>(_mm_cvttss_si32(_mm_set_ss(value)));
  }

  unsigned UnsignedFrame(double value) {
    if (!(value < 0x1p32) || value <= -1.0) {
      return 0xFFFFFFFFu;
    }

    return value < 0.0 ? 0u : static_cast<unsigned>(value);
  }
}

// OFFSET: 0x00AB7930
// FUNCTION: fn_17493
void C3dActorV1::Update() {
  if (this->playback != 1 && this->playback != 2) {
    return;
  }

  const unsigned tick = GetTickCount();

  if (tick == this->lastTick) {
    return;
  }

  this->frame = _mm_cvttss_si32(_mm_set_ss(this->speed * 100.0f + static_cast<float>(this->frame)));

  if (this->frame >= this->animationLength) {
    this->frame = this->playback == 1 ? std::bit_cast<int>(static_cast<unsigned>(this->animationLength) - 1u) : 0;
  }

  this->Animate(static_cast<double>(this->frame));
  this->lastTick = tick;
}

// OFFSET: 0x00AB7A10
// FUNCTION: fn_17494
void C3dActorV2::Update() {
  if (this->playback != 1 && this->playback != 2) {
    return;
  }

  const unsigned tick = GetTickCount();
  unsigned frame = UnsignedFrame(static_cast<float>(tick - this->startTick) * this->speed);

  if (static_cast<double>(frame) >= this->duration) {
    if (this->playback == 1) {
      frame = UnsignedFrame(this->duration - 1.0);
    } else {
      this->startTick = tick;
      frame = 0;
    }
  }

  this->Animate(static_cast<double>(frame));
}
