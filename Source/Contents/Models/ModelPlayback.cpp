#include "Contents/Models/ModelActor.h"
#include "Utility/Math/ScalarConversion.h"

#include <windows.h>

namespace {
  // INLINED
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

  this->frame = TruncateToInt(this->speed * 100.0f + static_cast<float>(this->frame));

  if (this->frame >= this->animationLength) {
    this->frame = this->playback == 1 ? this->animationLength - 1 : 0;
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
